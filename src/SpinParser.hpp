/**
 * @file SpinParser.hpp
 * @author Finn Lasse Buessen
 * @brief Pf-FRG solver implementation. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "lib/Log.hpp"
#include "lib/Timestamp.hpp"
#include "lib/LoadManager.hpp"
#include "lib/Exception.hpp"
#include "FrgCommon.hpp"
#include "CommandLineOptions.hpp"
#include "TaskFileParser.hpp"

class FrgCore;

/**
 * @brief 计算状态描述符.
 */
struct ComputationStatus
{
	/**
	 * @brief 计算状态标识符.
	 */
	enum struct Identifier : int
	{
		New = 0, ///< 当前任务是新的计算. 
		Running = 1, ///< 任务的计算正在进行中，并且已经编写了一个检查点. 
		Postprocessing = 2, ///< 任务已完成，但仍有待执行测量. 
		Finished = 3 ///< 任务已完成. 
	};
	
	Identifier statusIdentifier; ///< 计算状态标识符;
	Timestamp::Time startTime; ///< 计算开始时间. 
	Timestamp::Time checkpointTime; ///< 计算上次检查点时间. 
	Timestamp::Time endTime; ///< 计算结束时间. 
};

struct Fileset
{
	std::string taskFile; ///< 任务文件的路径. 
	std::string obsFile; ///< 观测文件的路径. 
	std::string dataFile; ///< 用于延迟测量的数据文件的路径. 
	std::string checkpointFile; ///< 检查点文件的路径. 
};

/**
 * @brief pf-FRG流动方程求解的主要对象和接口. 
 * @details SpinParser 对象为求解 pf-FRG 流动方程提供了中心接口. 
 * 它被设计为一个单例，通过 SpinParser::spinParser() 函数自动创建和检索. 
 * 然后通过 SpinParser::run() 函数启动流方程的解,该函数随启动参数 argc 和 argv 一起提供.
 * 一旦 run() 函数被触发，SpinParser 将读取任务文件，准备指定的晶格自旋模型，并启动用于求解相应流方程的数值核心.
 */
class SpinParser
{
public:
	/**
	 * @brief 检索 SpinParser 单例. 
	 * 
	 * @return SpinParser* 指向单一实例对象的指针. 
	 */
	static SpinParser *spinParser();

	/**
	 * @brief 启动 SpinParser. 
	 * 
	 * @param argc 启动参数 argc，由操作系统提供. 
	 * @param argv 启动参数 argv，由操作系统提供. 
	 * @return int 成功时返回 0，如果发生错误则返回 1. 
	 */
	int run(int argc, char **argv);

	/**
	 * @brief 查询当前实例是否为MPI主级别.
	 *
	 * @return bool 如果当前实例是 MPI 主级别，则返回 true。否则，返回 false. 
	 */
	bool isMasterRank() const;

	/**
	 * @brief 获取当前计算状态. 
	 *
	 * @return ComputationStatus 计算状态描述符.
	 */
	ComputationStatus getComputationStatus() const;

	/**
	 * @brief 获取输出文件的文件名. 
	 */
	Fileset getFileset() const;

	/**
	 * @brief 检索内部命令行解析器. 
	 * 
	 * @return CommandLineOptions* 内部命令行解析器. 
	 */
	CommandLineOptions *getCommandLineOptions() const;

	/**
	 * @brief 检索内部任务文件解析器. 
	 * 
	 * @return TaskFileParser* 内部任务文件解析器. 
	 */
	TaskFileParser *getTaskFileParser() const;

	/**
	 * @brief 检索内部负载管理器. 
	 * 
	 * @return HMP::LoadManager* 内部负载管理器.
	 */
	HMP::LoadManager *getLoadManager() const;

	/**
	 * @brief 检索内部数字核心.
	 * 
	 * @return FrgCore* 内部数字核心.
	 */
	FrgCore *getFrgCore() const;

protected:
	/**
	 * @brief 构造一个新的 SpinParser 对象. 
	 */
	SpinParser();

	/**
	 * @brief 销毁 SpinParser 对象. 
	 */
	~SpinParser();

	/**
	 * @brief 运行数值核心并应用微分方程求解器. 
	 */
	void runCore();

	/**
	 * @brief 将当前状态写入检查点文件. 
	 */
	void writeCheckpoint();

	static SpinParser *_spinParserInstance; ///< SpinParser 的单例实例. 
	bool _isMasterRank; ///< 如果当前实例是 MPI 主级别，则为 true，否则为 false. 
	ComputationStatus _computationStatus; ///< 计算状态. 
	Fileset _fileset; ///< 输出文件名. 

	CommandLineOptions *_commandLineOptions; ///< 内部命令行解析器. 
	TaskFileParser *_taskFileParser; ///< 内部任务文件解析器. 
	HMP::LoadManager *_loadManager; ///< 内部负载管理器. 
	FrgCore *_frgCore; ///< 内部数字核心. 
};