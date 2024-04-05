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
	 * @brief ���ڱ���ʱ���������Ϣ�����ݽṹ. 
	 */
	typedef boost::posix_time::ptime Time;
	typedef boost::posix_time::time_duration Duration;

	/**
	 * @brief ��ȡ��ǰʱ��. 
	 * 
	 * @return Time ��ǰʱ��. 
	 */
	inline Time time()
	{
		return boost::posix_time::second_clock::local_time();
	}

	/**
	 * @brief ��ȡ��ǰʱ�����δ����һЩƫ����. 
	 * 
	 * @param offset ƫ�Ƴ���ʱ�䣨����Ϊ��λ��. 
	 * @return Time ��ǰʱ�����ƫ����. 
	 */
	inline Time time(const int offset)
	{
		return boost::posix_time::second_clock::local_time() + boost::posix_time::time_duration(0, 0, offset, 0);
	}

	/**
	 * @brief �Ӹ�ʽ����ʱ����ַ����л�ȡʱ�����. 
	 * 
	 * @param timestamp ��ʽ��ʱ����ַ���. 
	 * @return Time ʱ���������ʱ��. 
	 */
	inline Time time(const std::string &timestamp)
	{
		return boost::posix_time::time_from_string(timestamp);
	}

	/**
	 * @brief ���ʱ������Ƿ����ڸ���������.
	 *
		@param time ʱ�����.
		@param offset ������ֵ������Ϊ��λ��.
		@return bool ���ʱ���������ָ�����������򷵻� true�����򷵻� false.
	 */
	inline bool isOlder(const Time &time, const int offset)
	{
		return (Timestamp::time() - time).total_seconds() > offset;
	}

	/**
	 * @brief ����ָ��ʱ���ʱ����ַ���. 
	 * 
	 * @param time ָ��ʱ��. 
	 * @return std::string ʱ����ַ���. 
	 */
	inline std::string timestamp(const Time &time)
	{
		return boost::posix_time::to_simple_string(time);
	}

	/**
	 * @brief ������ǰʱ���ʱ����ַ���.
	 *
	 * @return std::string ʱ����ַ���.
	 */
	inline std::string timestamp()
	{
		return timestamp(time());
	}
}
