/**
 * @file TaskFileParser.hpp
 * @author Finn Lasse Buessen
 * @brief Task file parser routine. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <string>
#include <set>
#include <boost/property_tree/ptree.hpp>

struct FrequencyDiscretization;
struct CutoffDiscretization;
struct Lattice;
struct ComputationStatus;
class FrgCore;

/**
 * @brief 任务文件解析器例程.该对象绑定到单个任务文件,最初读取并解析该文件,随后可以对其进行写入操作
 */
class TaskFileParser
{
public:
	/**
	 * @brief 构造一个新的TaskFileParser对象并解析指定的任务文件,对输入xml文件进行处理
	 * @details 在解析过程中,TaskFileParser将分配和初始化（使用任务文件中指定的参数）FrequencyDiscretization、CutoffDiscretization和Lattice对象；
	 * 将这些对象的指针分别存储在frequency、cutoff和lattice中。
	 * 此外,它还分配了任务文件中指定的FrgCore（包括FrgCore::measurements）,并将其写入frgCore。
	 * 还返回与任务文件相关联的计算状态。
	 * 
	 * @param[in] taskFilePath 要解析的任务文件的路径,使用 const 固定.
	 * @param[out] frequency 新生成的FrequencyDiscretization(频率离散化)
	 * @param[out] cutoff 新生成的CutoffDiscretization(截断离散化)
	 * @param[out] lattice 新生成的Lattice(晶格)
	 * @param[out] frgCore 新生成的FRG核心
	 * @param[out] computationStatus 与任务文件相关联的计算状态。
	 */
	TaskFileParser(const std::string &taskFilePath, FrequencyDiscretization *&frequency, CutoffDiscretization *&cutoff, Lattice *&lattice, FrgCore *&frgCore, ComputationStatus &computationStatus);

	/**
	 * @brief 将计算状态写入任务文件. 
	 * 
	 * @param finalize 如果设置为true,则将任务状态设置为'finished'. 判断任务是否完成
	 * @param updateCheckpoint 如果设置为true,则最后一个检查点时间将设置为当前时间. 更新检查点.
	 */
	void writeTaskFile(const ComputationStatus &computationStatus);

protected: 
	/**
	 * @brief 确保给定的属性树包含具有已知的必需或可选元素的特定节点.如果要求未满足,则抛出Exception::Type::InitializationError
	 * 
	 * @param tree 要测试的属性树. 
	 * @param node 要测试的节点. 
	 * @param requiredChildren 节点的必需子项列表. 
	 * @param requiredAttributes 节点的必需属性列表. 
	 * @param optionalChildren 节点的可选子项列表,默认为空. 
	 * @param optionalAttributes  节点的可选属性列表,默认为空.
	 */
	void _validateProperties(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &requiredChildren, const std::set<std::string> &requiredAttributes, const std::set<std::string> &optionalChildren = {}, const std::set<std::string> &optionalAttributes = {}) const;

	/**
	 * @brief 确保给定的属性树包含特定节点,并测试该节点是否包含所需子项的列表.如果不满足要求,则引发 Exception::Type::InitializationError。
	 * 
	 * @param tree 要测试的属性树. 
	 * @param node 要测试的节点. 
	 * @param requiredChildren 节点所需的子节点列表. 
	 * @param treePath 属性路径,在可能会在抛出的异常中作为节点名称的前缀.  
	 */
	void _validateRequiredChildren(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &requiredChildren, const std::string &treePath = "") const;
	
	/**
	 * @brief 确保给定的属性树包含特定节点，并测试该节点是否包含必需属性的列表。如果不满足要求，则引发 Exception::Type::InitializationError.
	 * 
	 * @param tree 要测试的属性树. 
	 * @param node 要测试的节点. 
	 * @param requiredAttributes 节点必需属性列表. 
	 * @param treePath 属性路径,在可能会在抛出的异常中作为节点名称的前缀.  
	 */
	void _validateRequiredAttributes(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &requiredAttributes, const std::string &treePath = "") const;
	
	/**
	 * @brief 确保给定的属性树包含特定节点，并测试该节点是否仅包含可选值列表中的子项。如果不满足要求，则引发 Exception::Type::InitializationError.
	 * 
	 * @param tree 要测试的属性树. 
	 * @param node 要测试的节点. 
	 * @param optionalChildren 节点的可选子节点列表. 
	 * @param treePath 属性路径,在可能会在抛出的异常中作为节点名称的前缀.  
	 */
	void _validateOptionalChildren(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &optionalChildren, const std::string &treePath = "") const;
	
	/**
	 * @brief 确保给定的属性树包含特定节点，并测试该节点是否仅包含可选值列表中的属性。如果不满足要求，则引发 Exception::Type::InitializationError.
	 * 
	 * @param tree 要测试的属性树. 
	 * @param node 要测试的节点. 
	 * @param optionalAttributes 节点的可选属性列表. 
	 * @param treePath 属性路径,在可能会在抛出的异常中作为节点名称的前缀.  
	 */
	void _validateOptionalAttributes(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &optionalAttributes, const std::string &treePath = "") const;

	boost::property_tree::ptree _taskFile; ///< 分析任务文件的内部属性树表示形式. 
};