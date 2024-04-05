/**
 * @file Timestamp.hpp
 * @author Finn Lasse Buessen
 * @brief Provide formatted timestamp strings. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <boost/date_time.hpp>

namespace Timestamp
{
	/**
	 * @brief 用于保存时间和日期信息的数据结构. 
	 */
	typedef boost::posix_time::ptime Time;
	typedef boost::posix_time::time_duration Duration;

	/**
	 * @brief 获取当前时间. 
	 * 
	 * @return Time 当前时间. 
	 */
	inline Time time()
	{
		return boost::posix_time::second_clock::local_time();
	}

	/**
	 * @brief 获取当前时间加上未来的一些偏移量. 
	 * 
	 * @param offset 偏移持续时间（以秒为单位）. 
	 * @return Time 当前时间加上偏移量. 
	 */
	inline Time time(const int offset)
	{
		return boost::posix_time::second_clock::local_time() + boost::posix_time::time_duration(0, 0, offset, 0);
	}

	/**
	 * @brief 从格式化的时间戳字符串中获取时间对象. 
	 * 
	 * @param timestamp 格式化时间戳字符串. 
	 * @return Time 时间戳描述的时间. 
	 */
	inline Time time(const std::string &timestamp)
	{
		return boost::posix_time::time_from_string(timestamp);
	}

	/**
	 * @brief 检查时间对象是否早于给定的秒数.
	 *
		@param time 时间对象.
		@param offset 年龄阈值（以秒为单位）.
		@return bool 如果时间对象早于指定的秒数，则返回 true。否则返回 false.
	 */
	inline bool isOlder(const Time &time, const int offset)
	{
		return (Timestamp::time() - time).total_seconds() > offset;
	}

	/**
	 * @brief 检索指定时间的时间戳字符串. 
	 * 
	 * @param time 指定时间. 
	 * @return std::string 时间戳字符串. 
	 */
	inline std::string timestamp(const Time &time)
	{
		return boost::posix_time::to_simple_string(time);
	}

	/**
	 * @brief 检索当前时间的时间戳字符串.
	 *
	 * @return std::string 时间戳字符串.
	 */
	inline std::string timestamp()
	{
		return timestamp(time());
	}
}
