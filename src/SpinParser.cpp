/**
 * @file SpinParser.cpp
 * @author Finn Lasse Buessen
 * @brief Pf-FRG solver implementation. 
 * 
 * @copyright Copyright (c) 2020
 */

#include <boost/filesystem.hpp>
#include "SpinParser.hpp"
#include "CommandLineOptions.hpp"
#include "TaskFileParser.hpp"
#include "FrgCore.hpp"
#ifndef DISABLE_MPI
#include "mpi.h"
#endif

#pragma region singleton definitions / object lifecycle
SpinParser *SpinParser::_spinParserInstance = nullptr;

SpinParser *SpinParser::spinParser()
{
	if (_spinParserInstance == nullptr) _spinParserInstance = new SpinParser;
	return _spinParserInstance;
}

FrgCore *SpinParser::getFrgCore() const
{
	return _frgCore;
}

SpinParser::SpinParser()
{
	int rank = 0;
	#ifndef DISABLE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	#endif
	_isMasterRank = (rank == 0) ? true : false;

	_commandLineOptions = nullptr;
	_taskFileParser = nullptr;
	_loadManager = HMP::newLoadManager();
	_frgCore = nullptr;
}

SpinParser::~SpinParser()
{
	delete _commandLineOptions;
	delete _frgCore;
}
#pragma endregion

int SpinParser::run(int argc, char **argv)
{
	try
	{
		//读取命令行开关
		_commandLineOptions = new CommandLineOptions(argc, argv);

		//如果设置了--help标志则中止程序
		if (_commandLineOptions->help()) return 0;

		//设置日志等级
		if (_isMasterRank) Log::log << Log::setDisplayLogLevel(_commandLineOptions->verbose() ? Log::LogLevel::Debug : Log::LogLevel::Info);

		//设置路径
		_fileset.taskFile = _commandLineOptions->taskFile();
		_fileset.obsFile = boost::filesystem::path(_fileset.taskFile).replace_extension("obs").string();
		_fileset.dataFile = boost::filesystem::path(_fileset.taskFile).replace_extension("data").string();
		_fileset.checkpointFile = boost::filesystem::path(_fileset.taskFile).replace_extension("checkpoint").string();

		//通过任务文件解析器设置 FrgCore
		_taskFileParser = new TaskFileParser(_fileset.taskFile, FrgCommon::_frequency, FrgCommon::_cutoff, FrgCommon::_lattice, _frgCore, _computationStatus);

		//停止程序仅请求点阵调试输出
		if (_commandLineOptions->debugLattice())
		{
			Log::log << Log::LogLevel::Info << "晶格调试输出完成.正在关闭." << Log::endl;
			return 0;
		}

		//运行核心
		Log::log << Log::LogLevel::Info << "启动 FRG 数字核心" << Log::endl;
		boost::posix_time::ptime startTime = boost::posix_time::microsec_clock::local_time();
		runCore();//调用运行核心函数
		Log::log << Log::LogLevel::Info << "关闭核心.计算时间 " << std::fixed << std::setprecision(2) << (boost::posix_time::microsec_clock::local_time() - startTime).total_microseconds() / 1000000.0 << " seconds. " << Log::endl;
	}
	catch (std::exception &e)
	{
		Log::log << Log::LogLevel::Error << "捕获异常: " << e.what() << Log::endl;
		return 1;
	}
	return 0;
}

bool SpinParser::isMasterRank() const
{
	return _isMasterRank;
}

ComputationStatus SpinParser::getComputationStatus() const
{
	return _computationStatus;
}

Fileset SpinParser::getFileset() const
{
	return _fileset;
}

CommandLineOptions *SpinParser::getCommandLineOptions() const
{
	return _commandLineOptions;
}

TaskFileParser *SpinParser::getTaskFileParser() const
{
	return _taskFileParser;
}

HMP::LoadManager *SpinParser::getLoadManager() const
{
	return _loadManager;
}

void SpinParser::runCore()
{
	if (_computationStatus.statusIdentifier == ComputationStatus::Identifier::New || _computationStatus.statusIdentifier == ComputationStatus::Identifier::Running)
	{
		//读取是否为新运算或已经存在检查点,如果我们要继续之前的计算的话,读取检查点记录
		CutoffIterator cutoff = FrgCommon::cutoff().begin();//读入截断值
		if (_computationStatus.statusIdentifier == ComputationStatus::Identifier::Running)
		{
			_frgCore->_flowingFunctional->readCheckpoint(_fileset.checkpointFile);
			cutoff = FrgCommon::cutoff().find(_frgCore->_flowingFunctional->cutoff);
		}

		//run calculation
		if (_computationStatus.statusIdentifier == ComputationStatus::Identifier::New) _computationStatus.startTime = Timestamp::time();
		_computationStatus.checkpointTime = Timestamp::time();
		while (cutoff != FrgCommon::cutoff().last())
		{
			//compute flow and measurements
			Log::log << Log::LogLevel::Debug << "Begin computation of flow." << Log::endl;
			_frgCore->computeStep();
			Log::log << Log::LogLevel::Debug << "Begin computation of measurements." << Log::endl;
			_frgCore->takeMeasurements();

			//check if flow has diverged
			Log::log << Log::LogLevel::Debug << "Begin computation of vertex." << Log::endl;
			if (_frgCore->_flow->isDiverged())
			{
				Log::log << Log::LogLevel::Info << "Vertex has diverged. Stopping calculation." << Log::endl;
				break;
			}

			//perform integration step
			++cutoff;
			_frgCore->finalizeStep(*cutoff);

			//print progress and write checkpoint
			Log::log << Log::LogLevel::Info << "Current cutoff is at " << std::fixed << std::setprecision(6) << _frgCore->_flowingFunctional->cutoff << Log::endl;
			if (Timestamp::isOlder(_computationStatus.checkpointTime, _commandLineOptions->checkpointTime()))
			{
				_computationStatus.checkpointTime = Timestamp::time();
				_computationStatus.statusIdentifier = ComputationStatus::Identifier::Running;
				writeCheckpoint();
			}
		}

		//perform final measurement
		_frgCore->takeMeasurements();

		//finalize calculation and write last checkpoint
		bool postprocessingRequired = false;
		if (_commandLineOptions->deferMeasurements()) postprocessingRequired = true;
		for (auto m : _frgCore->_measurements) if (m->isDeferred()) postprocessingRequired = true;
		
		if (postprocessingRequired) _computationStatus.statusIdentifier = ComputationStatus::Identifier::Postprocessing;
		else
		{
			_computationStatus.endTime = Timestamp::time();
			_computationStatus.statusIdentifier = ComputationStatus::Identifier::Finished;
		}
		_computationStatus.checkpointTime = Timestamp::time();
		writeCheckpoint();
	}
	else if (_computationStatus.statusIdentifier == ComputationStatus::Identifier::Postprocessing)
	{
		Log::log << Log::LogLevel::Info << "Entering post-processing stage." << Log::endl;

		int n = 0;
		while (_frgCore->_flowingFunctional->readCheckpoint(_fileset.dataFile, n++))
		{
			Log::log << Log::LogLevel::Info << "Post-processing measurements at cutoff " + std::to_string(_frgCore->_flowingFunctional->cutoff) << Log::endl;
			_frgCore->takeMeasurements();
		}

		_computationStatus.endTime = Timestamp::time();
		_computationStatus.statusIdentifier = ComputationStatus::Identifier::Finished;
		_taskFileParser->writeTaskFile(_computationStatus);

		Log::log << Log::LogLevel::Info << "Post-processing done." << Log::endl;
	}
	else if (_computationStatus.statusIdentifier == ComputationStatus::Identifier::Finished)
	{
		Log::log << Log::LogLevel::Info << "Nothing left to do. Task had been finished already." << Log::endl;
	}

}

void SpinParser::writeCheckpoint()
{
	if (_isMasterRank)
	{
		Log::log << Log::LogLevel::Info << "Writing checkpoint." << Log::endl;
		_frgCore->_flowingFunctional->writeCheckpoint(_fileset.checkpointFile);
		_taskFileParser->writeTaskFile(_computationStatus);
	}
}