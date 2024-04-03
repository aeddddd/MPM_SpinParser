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
 * @brief ����״̬������.
 */
struct ComputationStatus
{
	/**
	 * @brief ����״̬��ʶ��.
	 */
	enum struct Identifier : int
	{
		New = 0, ///< ��ǰ�������µļ���. 
		Running = 1, ///< ����ļ������ڽ����У������Ѿ���д��һ������. 
		Postprocessing = 2, ///< ��������ɣ������д�ִ�в���. 
		Finished = 3 ///< ���������. 
	};
	
	Identifier statusIdentifier; ///< ����״̬��ʶ��;
	Timestamp::Time startTime; ///< ���㿪ʼʱ��. 
	Timestamp::Time checkpointTime; ///< �����ϴμ���ʱ��. 
	Timestamp::Time endTime; ///< �������ʱ��. 
};

struct Fileset
{
	std::string taskFile; ///< �����ļ���·��. 
	std::string obsFile; ///< �۲��ļ���·��. 
	std::string dataFile; ///< �����ӳٲ����������ļ���·��. 
	std::string checkpointFile; ///< �����ļ���·��. 
};

/**
 * @brief pf-FRG��������������Ҫ����ͽӿ�. 
 * @details SpinParser ����Ϊ��� pf-FRG ���������ṩ�����Ľӿ�. 
 * �������Ϊһ��������ͨ�� SpinParser::spinParser() �����Զ������ͼ���. 
 * Ȼ��ͨ�� SpinParser::run() �������������̵Ľ�,�ú������������� argc �� argv һ���ṩ.
 * һ�� run() ������������SpinParser ����ȡ�����ļ���׼��ָ���ľ�������ģ�ͣ����������������Ӧ�����̵���ֵ����.
 */
class SpinParser
{
public:
	/**
	 * @brief ���� SpinParser ����. 
	 * 
	 * @return SpinParser* ָ��һʵ�������ָ��. 
	 */
	static SpinParser *spinParser();

	/**
	 * @brief ���� SpinParser. 
	 * 
	 * @param argc �������� argc���ɲ���ϵͳ�ṩ. 
	 * @param argv �������� argv���ɲ���ϵͳ�ṩ. 
	 * @return int �ɹ�ʱ���� 0��������������򷵻� 1. 
	 */
	int run(int argc, char **argv);

	/**
	 * @brief ��ѯ��ǰʵ���Ƿ�ΪMPI������.
	 *
	 * @return bool �����ǰʵ���� MPI �������򷵻� true�����򣬷��� false. 
	 */
	bool isMasterRank() const;

	/**
	 * @brief ��ȡ��ǰ����״̬. 
	 *
	 * @return ComputationStatus ����״̬������.
	 */
	ComputationStatus getComputationStatus() const;

	/**
	 * @brief ��ȡ����ļ����ļ���. 
	 */
	Fileset getFileset() const;

	/**
	 * @brief �����ڲ������н�����. 
	 * 
	 * @return CommandLineOptions* �ڲ������н�����. 
	 */
	CommandLineOptions *getCommandLineOptions() const;

	/**
	 * @brief �����ڲ������ļ�������. 
	 * 
	 * @return TaskFileParser* �ڲ������ļ�������. 
	 */
	TaskFileParser *getTaskFileParser() const;

	/**
	 * @brief �����ڲ����ع�����. 
	 * 
	 * @return HMP::LoadManager* �ڲ����ع�����.
	 */
	HMP::LoadManager *getLoadManager() const;

	/**
	 * @brief �����ڲ����ֺ���.
	 * 
	 * @return FrgCore* �ڲ����ֺ���.
	 */
	FrgCore *getFrgCore() const;

protected:
	/**
	 * @brief ����һ���µ� SpinParser ����. 
	 */
	SpinParser();

	/**
	 * @brief ���� SpinParser ����. 
	 */
	~SpinParser();

	/**
	 * @brief ������ֵ���Ĳ�Ӧ��΢�ַ��������. 
	 */
	void runCore();

	/**
	 * @brief ����ǰ״̬д������ļ�. 
	 */
	void writeCheckpoint();

	static SpinParser *_spinParserInstance; ///< SpinParser �ĵ���ʵ��. 
	bool _isMasterRank; ///< �����ǰʵ���� MPI ��������Ϊ true������Ϊ false. 
	ComputationStatus _computationStatus; ///< ����״̬. 
	Fileset _fileset; ///< ����ļ���. 

	CommandLineOptions *_commandLineOptions; ///< �ڲ������н�����. 
	TaskFileParser *_taskFileParser; ///< �ڲ������ļ�������. 
	HMP::LoadManager *_loadManager; ///< �ڲ����ع�����. 
	FrgCore *_frgCore; ///< �ڲ����ֺ���. 
};