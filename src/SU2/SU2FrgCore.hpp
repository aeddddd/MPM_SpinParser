/**
 * @file SU2FrgCore.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) 模型的 FRG 核心实现.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "FrgCore.hpp"

/**
 * @brief SU(2) 模型的 FRG 核心实现.
 */
class SU2FrgCore : public FrgCore
{
public:
	/**
	 * @brief 构造一个新的 SU2FrgCore,使用指定的自旋模型进行初始化并添加测量. 
	 * 
	 * @param spinModel 自旋模型初始化有效动作. 
	 * @param measurements 要添加的测量值. 
	 * @param options 任务文件中提供的核心选项的字符串形式列表. 
	 */
	SU2FrgCore(const SpinModel &spinModel, const std::vector<Measurement *> &measurements, const std::map<std::string, std::string> &options);
	
	/**
	 * @brief 销毁SU2FrgCore对象. 
	 */
	~SU2FrgCore();

	/**
	 * @brief 计算流动方程. 
	 */
	void computeStep() override;
	
	/**
	 * @brief 完成流动方程的计算. 
	 * 
	 * @param newCutoff 用于推断流动的新截止点. 
	 */
	void finalizeStep(const float newCutoff) override;

	float spinLength; ///< S的值,决定自旋长度. 
	float normalization; ///< 能量归一化因子. 

private:
	int dataStacks[8]; ///< 对LoadManager::DataStack的引用. 

	/**
	 * @brief 计算特定线性迭代器的单粒子顶点流,该迭代器通过 SU2VertexSingleParticle::expand Iterator() 进行扩展.
	 * 
	 * @param iterator 线性迭代器. 
	 */
	void _calculateVertexSingleParticle(const int iterator);

	/**
	 * @brief 计算特定线性迭代器的双粒子顶点流,该迭代器通过 SU2VertexTwoParticle::expandIterator() 进行扩展.
	 * 
	 * @param iterator 线性迭代器. 
	 */
	void _calculateVertexTwoParticle(const int iterator);
};