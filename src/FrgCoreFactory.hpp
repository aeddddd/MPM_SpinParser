/**
 * @file FrgCoreFactory.hpp
 * @author Finn Lasse Buessen
 * @brief Factory function to create a new FrgCore. 
 * @details 对于自定义 FrgCore 的实现，应该扩展 FrgCore Factory::new FrgCore 例程，以便它可以重新识别自定义 FrgCore 的标识符并相应地创建其实例. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <string>
#include <vector>
#include <map>
#include "FrgCore.hpp"

namespace FrgCoreFactory
{
	/**
	 * @brief 测量协议的抽象规范. 
	 */
	struct MeasurementSpecification
	{
		std::string identifier; ///< 测量类型的字符串形式标识符，如任务文件中所指定. 
		std::string output; ///< 测量结果的输出文件. 
		float minCutoff; ///< 要调用的最小截止值. 
		float maxCutoff; ///< 要调用的最大截止值. 
		bool defer; ///< 推迟标志。如果设置为 true，则仅在后处理阶段调用测量. 
		std::vector<std::pair<std::string, std::string>> options; ///< 任务文件中指定的字符串形式协议修饰符. 
	};

	/**
	 * @brief 为给定的对称标识符、自旋模型和测量协议创建新的FrgCore. 
	 * 
	 * @param identifier 字符串形式的对称标识符，如任务文件中指定. 
	 * @param model 用于确定流动初始条件的自旋模型. 
	 * @param measurements  在核心执行期间调用的测量协议. 
	 * @param options 任务文件中指定的字符串形式核心修饰符. 
	 * @return FrgCore* 指向新FrgCore对象的指针. 
	 */
	FrgCore *newFrgCore(const std::string &identifier, const SpinModel &model, const std::vector<MeasurementSpecification> &measurements, const std::map<std::string, std::string> &options);
}