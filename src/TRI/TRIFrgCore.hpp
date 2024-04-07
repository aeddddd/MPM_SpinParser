/**
 * @file TRIFrgCore.hpp
 * @author Finn Lasse Buessen
 * @brief FrgCore implementation for time reversal invariant models.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "FrgCore.hpp"

/**
 * @brief 时间反转不变模型的 FrgCore 实现.
 */
class TRIFrgCore : public FrgCore
{
public:
	/**
	 * @brief 构造一个新的 TRIFrgCore,使用指定的自旋模型进行初始化并添加测量. 
	 * 
	 * @param spinModel 自旋模型初始化有效动作. 
	 * @param measurements 要添加的测量值. 
	 * @param options 任务文件中提供的核心选项的字符串形式列表. 
	 */
	TRIFrgCore(const SpinModel &spinModel, const std::vector<Measurement *> &measurements, const std::map<std::string, std::string> &options);
	
	/**
	 * @brief 销毁 TRIFrgCore 对象. 
	 */
	~TRIFrgCore();

	/**
	 * @brief 计算流动方程. 
	 */
	void computeStep() override;

	/**
	 * @brief 完成这次流动方程的计算. 
	 * 
	 * @param newCutoff 推断流动的新截止点. 
	 */
	void finalizeStep(const float newCutoff) override;

	float normalization; ///< 能量归一化因子. 

private:
	int dataStacks[6]; ///<对 LoadManager::DataStack 的引用. 

	/**
	 * @brief 计算特定线性迭代器的单粒子顶点流,该迭代器通过 TRIVertexSingleParticle::expand Iterator() 进行扩展.
	 * 
	 * @param iterator 线性迭代器. 
	 */
	void _calculateVertexSingleParticle(const int iterator);

	/**
	 * @brief 计算特定线性迭代器的双粒子顶点流,该迭代器通过 TRIVertexTwoParticle::expand Iterator() 展开.
	 * 
	 * @param iterator 线性迭代器. 
	 */
	void _calculateVertexTwoParticle(const int iterator);
};