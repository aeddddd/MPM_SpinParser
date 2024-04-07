/**
 * @file InputParser.hpp
 * @author Finn Lasse Buessen
 * @brief �������ַ�������Ϊ��ֵ.���Խ����򵥵ĳ˷��� sqrt() ���ʽ. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <iostream>
#include <sstream>
#include "boost/regex.hpp"
#include "lib/Log.hpp"

namespace InputParser
{
	/**
	 * @brief �������ַ�������ΪDouble.����򵥵ĳ˷��������� sqrt() ���ʽ. 
	 * ��������漰 x*x��x/x��sqrt(x) ������������ʽ����,���� x ��ʮ������. 
	 * 
	 * @param input �����ַ���. 
	 * @return double ���������ֵ. 
	 */
	inline double stringToDouble(const std::string &input)
	{
		//debug output
		boost::regex number("[-\\.\\d]+");
		bool debugFlag = !boost::regex_match(input, number);
		std::string parsedString(input);

		boost::regex squareRoot("sqrt\\(([\\.\\d]+)\\)");
		boost::regex multDiv("([\\.\\d]+)([\\*/])([\\.\\d]+)");
		boost::smatch match;
		std::stringstream replacement;
		replacement << std::fixed << std::setprecision(20);
		//parse sqrt
		while (boost::regex_search(parsedString, match, squareRoot))
		{
			replacement.str("");
			replacement << match.prefix() << sqrt(std::stod(match.str(1))) << match.suffix();
			parsedString = replacement.str();
		}
		//parse multiplication and division
		while (boost::regex_search(parsedString, match, multDiv))
		{
			replacement.str("");
			replacement << match.prefix();
			if (match.str(2) == "*") replacement << std::stod(match.str(1)) * std::stod(match.str(3));
			else replacement << std::stod(match.str(1)) / std::stod(match.str(3));
			replacement << match.suffix();
			parsedString = replacement.str();
		}

		if (debugFlag) Log::log << Log::LogLevel::Debug << "parsed input string " << input << " to " << parsedString << Log::endl;
		return std::stod(parsedString);
	}

	/**
	 * @brief �������ַ�������Ϊfloat.����򵥵ĳ˷��������� sqrt() ���ʽ. 
	 * ��������漰 x*x��x/x �� sqrt(x) ��ʽ����,���� x ��ʮ������. 
	 * 
	 * @param input �����ַ���. 
	 * @return float ���������ֵ. 
	 */
	inline float stringToFloat(const std::string &input)
	{
		return float(stringToDouble(input));
	}
}