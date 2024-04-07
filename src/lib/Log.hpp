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
	 * @brief ָ��������˵���־����.�κ���־���𻹰����ϵ������������.
	 */
	enum struct LogLevel
	{
		None = 0, ///< �����. 
		Error = 1, ///< ��ӡ������Ϣ. 
		Warning = 2, ///< ��ӡ����. 
		Info = 3, ///< ��ӡ��Ϣ. 
		Debug = 4 ///< ��ӡ�������. 
	};

	class Logstream;

	/**
	 * @brief �����ڲ��ṹ����־������������. 
	 */
	typedef std::function<Logstream & (Logstream &)> StructManipulator;

	/**
	 * @brief Logstream manipulator type without internal structure.
	 */
	typedef Logstream &(*Manipulator)(Logstream &);

	/**
	 * @brief ���ڼ�������˵���־������.
	 * @details Logstream �������ѡ������־�����ṩ�������.
	 * ���˺�����д�� stdout.
	 * ���ɵ��κ����������ʱ���,�����Դ��� Logstream ����������ʱ��.
	 */
	class Logstream
	{
		friend Logstream &endl(Logstream &ls);
		friend StructManipulator setDisplayLogLevel(const Log::LogLevel logLevel);
		friend StructManipulator setLogLevel(const Log::LogLevel logLevel);
	public:
		/**
		 * @brief ����һ���µ� Logstream ����,��־����Ϊ Log::Log Level::Info.
		 */
		Logstream(std::ostream &logTarget) : _logTarget(logTarget), _constructionTime(boost::posix_time::microsec_clock::local_time())
		{
			_carriageReturn = true;
			_displayLogLevel = Log::LogLevel::Info;
			_streamLogLevel = Log::LogLevel::Info;
		};

		/**
		 * @brief ����������Ϣ����������.�������κ�ʵ�� stdout ��������������־����.
		 *
		 * @tparam ��־��Ϣ������.
		 * @param ls Ŀ�� Logstream ����.
		 * @param rhs Ҫд�����־��Ϣ.
		 * @return Logstream& �ο� Logstream ������.
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
		 * @brief �������ù��˼���� Log Level �������������.
		 *
		 * @param rhs �µ���־����ֵ.
		 * @return Logstream& �ο�����.
		 */
		Logstream &operator<<(const Log::LogLevel &rhs)
		{
			_streamLogLevel = rhs;
			return *this;
		}

		/**
		 * @brief ����ͨ���������Ӧ������־��.
		 *
		 * @param rhs ��־��������.
		 * @return Logstream& �ο�����.
		 */
		Logstream &operator<<(const Manipulator &rhs)
		{
			return rhs(*this);
		}

		/**
		 * @brief ���ṹ�����������Ӧ������־��.
		 *
		 * @param rhs ��־��������.
		 * @return Logstream& �ο�����.
		 */
		Logstream &operator<<(const StructManipulator &rhs)
		{
			return rhs(*this);
		}

	private:
		std::ostream &_logTarget; ///< �����. 
		boost::posix_time::ptime _constructionTime; ///< Logstream����Ĵ���ʱ��. 

		bool _carriageReturn; ///< �������Ϊ true,���ӡ��һ����־��Ϣ������ʱ���. 
		Log::LogLevel _displayLogLevel; ///< ѡ�����������. @see Log::LogLevel
		Log::LogLevel _streamLogLevel; ///< ����־��Ϣ�ĵ�ǰ��־����. 
	};

	#pragma region Manipulators
	/**
	 * @brief ������η���ӡ����.
	 *
	 * @param ls Ŀ�� Logstream ����.
	 * @return Logstream& �ο�����.
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
	 * @brief ����������η���������ʾ��־���������.
	 *
	 * @param logLevel ���˵ȼ�.
	 * @return StructManipulator ��־��������.
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
	 * @brief ����������η��Ը�����־����.
	 *
	 * @param logLevel �µ���־����.
	 * @return StructManipulator ��־��������.
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

	extern Logstream log; ///< ȫ�� Logstream ʵ��. 
}