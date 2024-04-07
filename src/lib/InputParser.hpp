/**
 * @file InputParser.hpp
 * @author Finn Lasse Buessen
 * @brief 将输入字符串解析为数值.可以解析简单的乘法和 sqrt() 表达式. 
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
	 * @brief 将输入字符串解析为Double.解决简单的乘法、除法和 sqrt() 表达式. 
	 * 输入可能涉及 x*x、x/x、sqrt(x) 及其更长组合形式的项,其中 x 是十进制数. 
	 * 
	 * @param input 输入字符串. 
	 * @return double 解析后的数值. 
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
	 * @brief 将输入字符串解析为float.解决简单的乘法、除法和 sqrt() 表达式. 
	 * 输入可能涉及 x*x、x/x 和 sqrt(x) 形式的项,其中 x 是十进制数. 
	 * 
	 * @param input 输入字符串. 
	 * @return float 解析后的数值. 
	 */
	inline float stringToFloat(const std::string &input)
	{
		return float(stringToDouble(input));
	}
}