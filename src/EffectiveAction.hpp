/**
 * @file EffectiveAction.hpp
 * @author Finn Lasse Buessen
 * @brief Virtual implementation of a flowing effective action. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <hdf5.h>
#include "lib/Log.hpp"
#include "lib/Exception.hpp"

/**
 * @brief 虚拟实施流畅有效的行动. 
 * @details 具体实现应该根据需要实现数据结构——例如两点顶点和四点顶点信息. 
 * 该对象提供由 FrgCore 实现和 Spin Parser 访问的虚拟接口. 
 */
struct EffectiveAction
{
public:
	/**
	 * @brief 构造一个新的Effective Action对象.
	 */
	EffectiveAction() : cutoff(0.0f) {};

	/**
	 * @brief 虚拟析构函数.
	 */
	virtual ~EffectiveAction() {};

	/**
	 * @brief 将所有内部数据写入指定文件路径的检查点,并返回写入的检查点的标识符.检查点标识符是一个非负整数,枚举输出文件中的所有检查点数据集,从零开始.
	 * 
	 * @param dataFilePath 检查点文件路径. 
	 * @param append 如果设置为 false,则覆盖现有检查点.否则,如果不存在具有相同截止值的先前检查点,则附加检查点.如果具有相同截止值的检查点已存在,则不执行任何操作.
	 *
	 * @return int 写入的检查点的标识符.如果写入过程被跳过,则返回-1. 
	 */
	virtual int writeCheckpoint(const std::string &dataFilePath, const bool append = false) const = 0;

	/**
	 * @brief 从指定文件路径处具有指定检查点标识符的检查点读取内部数据. 
	 * 如果检查点标识符设置为-1,则读取最近的检查点.
	 * 
	 * @param datafilePath 检查点文件路径. 
	 * @param checkpointId 要读取的检查点的标识符. 
	 * @return bool 如果检查点读取成功则返回 true；否则返回 false. 
	 */
	virtual bool readCheckpoint(const std::string &datafilePath, const int checkpointId = -1) = 0;

	/**
	 * @brief 指示顶点是否已经发散到 NaN. 
	 *
	 * @return bool 如果顶点已发散,则返回 true,否则返回 false. 
	 */
	virtual bool isDiverged() const = 0;

	float cutoff; ///< RG 截止值. 
};