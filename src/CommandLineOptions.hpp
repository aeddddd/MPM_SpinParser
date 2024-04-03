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
 * @brief 解析器对象，可以提供 argc/argv 信息，然后将解析的值保存在其成员变量中. 
 */
class CommandLineOptions
{
public:
	/**
	 * @brief 构造新的 CommandLineOptions 对象并分析 argc/argv 信息. 
	 * 
	 * @param argc 传递给程序调用的参数计数. 
	 * @param argv 传递给程序调用的参数向量. 
	 */
	CommandLineOptions(int argc, char **argv);

	/**
	 * @brief 检索“--help”标志设置. 
	 * 
	 * @return bool 如果设置了“--help”标志，则返回 true。否则，返回 false.
	 */
	bool help() const;

	/**
	 * @brief 检索“--verbose”标志设置. 
	 * 
	 * @return bool 如果设置了“--verbose”标志，则返回 true。否则返回 false.
	 */
	bool verbose() const;

	/**
	 * @brief 检索“--checkpoint Time”标志的值. 
	 * 
	 * @return int “--checkpoint Time”标志的值. 
	 */
	int checkpointTime() const;

	/**
	 * @brief 检索“--force Restart”标志设置. 
	 * 
	 * @return bool 如果设置了“--force Restart”标志，则返回 true。否则返回 false.
	 */
	bool forceRestart() const;

	/**
	 * @brief 检索“--defer”标志设置. 
	 * 
	 * @return bool 如果设置了“--defer”标志，则返回 true。否则返回 false.
	 */
	bool deferMeasurements() const;

	/**
	 * @brief 检索“--debug Lattice”标志设置. 
	 * 
	 * @return bool 如果设置了“--debug Lattice”标志，则返回 true。否则返回 false.
	 */
	bool debugLattice() const;

	/**
	 * @brief 检索“--task File”标志的值.
	 * 
	 * @return std::string “--task File”标志的值.
	 */
	std::string taskFile() const;

	/**
	 * @brief 检索“--resource Path”标志的值.
	 * 
	 * @return std::string “--resource Path”标志的值.
	 */
	std::string resourcePath() const;

protected:
	bool _help; ///< 设置帮助标志“--help”.
	bool _verbose; ///< 设置了详细标志“--verbose”.
	int _checkpointTime; ///< “--checkpoint Time”参数的值. 
	bool _forceRestart; ///< 强制标志“--force Restart”已设置. 
	bool _deferMeasurements; ///< 延迟标志“--defer”已设置. 
	bool _debugLattice; ///< 设置了晶格调试标志“--debug Lattice”. 
	std::string _taskFile; ///< “--task File”参数的值. 
	std::string _resourcePath; ///< “--resource Path”参数的值. 
};