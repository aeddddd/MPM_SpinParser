/**
 * @file Log.hpp
 * @author Finn Lasse Buessen
 * @brief Lightweight logging interface with output filtering.
 *
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <iostream>
#include <functional>
#include <boost/date_time.hpp>


namespace Log
{
	/**
	 * @brief 指定输出过滤的日志级别.任何日志级别还包括较低输出级别的输出.
	 */
	enum struct LogLevel
	{
		None = 0, ///< 无输出. 
		Error = 1, ///< 打印错误消息. 
		Warning = 2, ///< 打印警告. 
		Info = 3, ///< 打印信息. 
		Debug = 4 ///< 打印调试输出. 
	};

	class Logstream;

	/**
	 * @brief 具有内部结构的日志流操纵器类型. 
	 */
	typedef std::function<Logstream & (Logstream &)> StructManipulator;

	/**
	 * @brief Logstream manipulator type without internal structure.
	 */
	typedef Logstream &(*Manipulator)(Logstream &);

	/**
	 * @brief 用于简单输出过滤的日志流对象.
	 * @details Logstream 对象根据选定的日志级别提供输出过滤.
	 * 过滤后的输出写入 stdout.
	 * 生成的任何输出都带有时间戳,测量自创建 Logstream 对象以来的时间.
	 */
	class Logstream
	{
		friend Logstream &endl(Logstream &ls);
		friend StructManipulator setDisplayLogLevel(const Log::LogLevel logLevel);
		friend StructManipulator setLogLevel(const Log::LogLevel logLevel);
	public:
		/**
		 * @brief 构造一个新的 Logstream 对象,日志过滤为 Log::Log Level::Info.
		 */
		Logstream(std::ostream &logTarget) : _logTarget(logTarget), _constructionTime(boost::posix_time::microsec_clock::local_time())
		{
			_carriageReturn = true;
			_displayLogLevel = Log::LogLevel::Info;
			_streamLogLevel = Log::LogLevel::Info;
		};

		/**
		 * @brief 任意类型消息的输出运算符.将接受任何实现 stdout 的输出运算符的日志对象.
		 *
		 * @tparam 日志消息的类型.
		 * @param ls 目标 Logstream 对象.
		 * @param rhs 要写入的日志消息.
		 * @return Logstream& 参考 Logstream 操作数.
		 */
		template <class T> friend Logstream &operator<<(Logstream &ls, const T &rhs)
		{
			if (ls._displayLogLevel < ls._streamLogLevel) return ls;
			if (ls._streamLogLevel == Log::LogLevel::Warning) ls._logTarget << "\033[31m";
			if (ls._carriageReturn)
			{
				float dt = float((boost::posix_time::microsec_clock::local_time() - ls._constructionTime).total_microseconds());
				ls._logTarget << "[" << std::fixed << std::setprecision(6) << dt / 1000000.0f << "][";
				if (ls._streamLogLevel == LogLevel::Debug) ls._logTarget << "D";
				else if (ls._streamLogLevel == LogLevel::Info) ls._logTarget << "I";
				else if (ls._streamLogLevel == LogLevel::Warning) ls._logTarget << "W";
				else if (ls._streamLogLevel == LogLevel::Error) ls._logTarget << "E";
				ls._logTarget << "] ";
				ls._carriageReturn = false;
			}
			ls._logTarget << rhs;
			if (ls._streamLogLevel == Log::LogLevel::Warning) ls._logTarget << "\033[0m";

			return ls;
		}

		/**
		 * @brief 用于设置过滤级别的 Log Level 对象的输出运算符.
		 *
		 * @param rhs 新的日志级别值.
		 * @return Logstream& 参考自身.
		 */
		Logstream &operator<<(const Log::LogLevel &rhs)
		{
			_streamLogLevel = rhs;
			return *this;
		}

		/**
		 * @brief 将普通输出操纵器应用于日志流.
		 *
		 * @param rhs 日志流操纵器.
		 * @return Logstream& 参考自身.
		 */
		Logstream &operator<<(const Manipulator &rhs)
		{
			return rhs(*this);
		}

		/**
		 * @brief 将结构化输出操纵器应用于日志流.
		 *
		 * @param rhs 日志流操纵器.
		 * @return Logstream& 参考自身.
		 */
		Logstream &operator<<(const StructManipulator &rhs)
		{
			return rhs(*this);
		}

	private:
		std::ostream &_logTarget; ///< 输出流. 
		boost::posix_time::ptime _constructionTime; ///< Logstream对象的创建时间. 

		bool _carriageReturn; ///< 如果设置为 true,则打印下一条日志消息并带有时间戳. 
		Log::LogLevel _displayLogLevel; ///< 选定的输出过滤. @see Log::LogLevel
		Log::LogLevel _streamLogLevel; ///< 新日志消息的当前日志级别. 
	};

	#pragma region Manipulators
	/**
	 * @brief 输出修饰符打印新行.
	 *
	 * @param ls 目标 Logstream 对象.
	 * @return Logstream& 参考自身.
	 */
	inline Logstream &endl(Logstream &ls)
	{
		if (ls._displayLogLevel >= ls._streamLogLevel)
		{
			ls._logTarget << std::endl;
			ls._carriageReturn = true;
		}
		return ls;
	}

	/**
	 * @brief 生成输出修饰符以设置显示日志级别过滤器.
	 *
	 * @param logLevel 过滤等级.
	 * @return StructManipulator 日志流操纵器.
	 */
	inline StructManipulator setDisplayLogLevel(const Log::LogLevel logLevel)
	{
		auto manipulator = [logLevel](Logstream &ls)->Logstream &
		{
			ls._displayLogLevel = logLevel;
			return ls;
		};
		return manipulator;
	}

	/**
	 * @brief 生成输出修饰符以更改日志级别.
	 *
	 * @param logLevel 新的日志级别.
	 * @return StructManipulator 日志流操纵器.
	 */
	inline StructManipulator setLogLevel(const Log::LogLevel logLevel)
	{
		auto manipulator = [logLevel](Logstream &ls)->Logstream &
		{
			ls._streamLogLevel = logLevel;
			return ls;
		};
		return manipulator;
	}
	#pragma endregion

	extern Logstream log; ///< 全局 Logstream 实例. 
}