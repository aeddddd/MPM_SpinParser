/**
 * @file CommandLineOptions.hpp
 * @author Finn Lasse Buessen
 * @brief Parser for command line arguments. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <string>

/**
 * @brief ���������󣬿����ṩ argc/argv ��Ϣ��Ȼ�󽫽�����ֵ���������Ա������. 
 */
class CommandLineOptions
{
public:
	/**
	 * @brief �����µ� CommandLineOptions ���󲢷��� argc/argv ��Ϣ. 
	 * 
	 * @param argc ���ݸ�������õĲ�������. 
	 * @param argv ���ݸ�������õĲ�������. 
	 */
	CommandLineOptions(int argc, char **argv);

	/**
	 * @brief ������--help����־����. 
	 * 
	 * @return bool ��������ˡ�--help����־���򷵻� true�����򣬷��� false.
	 */
	bool help() const;

	/**
	 * @brief ������--verbose����־����. 
	 * 
	 * @return bool ��������ˡ�--verbose����־���򷵻� true�����򷵻� false.
	 */
	bool verbose() const;

	/**
	 * @brief ������--checkpoint Time����־��ֵ. 
	 * 
	 * @return int ��--checkpoint Time����־��ֵ. 
	 */
	int checkpointTime() const;

	/**
	 * @brief ������--force Restart����־����. 
	 * 
	 * @return bool ��������ˡ�--force Restart����־���򷵻� true�����򷵻� false.
	 */
	bool forceRestart() const;

	/**
	 * @brief ������--defer����־����. 
	 * 
	 * @return bool ��������ˡ�--defer����־���򷵻� true�����򷵻� false.
	 */
	bool deferMeasurements() const;

	/**
	 * @brief ������--debug Lattice����־����. 
	 * 
	 * @return bool ��������ˡ�--debug Lattice����־���򷵻� true�����򷵻� false.
	 */
	bool debugLattice() const;

	/**
	 * @brief ������--task File����־��ֵ.
	 * 
	 * @return std::string ��--task File����־��ֵ.
	 */
	std::string taskFile() const;

	/**
	 * @brief ������--resource Path����־��ֵ.
	 * 
	 * @return std::string ��--resource Path����־��ֵ.
	 */
	std::string resourcePath() const;

protected:
	bool _help; ///< ���ð�����־��--help��.
	bool _verbose; ///< ��������ϸ��־��--verbose��.
	int _checkpointTime; ///< ��--checkpoint Time��������ֵ. 
	bool _forceRestart; ///< ǿ�Ʊ�־��--force Restart��������. 
	bool _deferMeasurements; ///< �ӳٱ�־��--defer��������. 
	bool _debugLattice; ///< �����˾�����Ա�־��--debug Lattice��. 
	std::string _taskFile; ///< ��--task File��������ֵ. 
	std::string _resourcePath; ///< ��--resource Path��������ֵ. 
};