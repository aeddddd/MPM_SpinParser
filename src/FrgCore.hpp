/**
 * @file FrgCore.hpp
 * @author Finn Lasse Buessen
 * @brief Numerics core for pf-FRG calculations. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <vector>
#include "EffectiveAction.hpp"
#include "Measurement.hpp"
#include "SpinModel.hpp"
#include "SpinParser.hpp"

class SpinParser;

/**
 * @brief pf-FRG 数值核心的虚拟实现. 
 * @details FrgCore 代表 pf-FRG 计算的中央数值单元. 
 * 它定义了微分方程求解器、测量协议和流动方程的具体实现之间的接口. 
 * FrgCore还定义了读写检查点的接口. 
 *
 * 每个特定的不同对称约束形式的流动方程组均源自此类. 
 * 自定义流方程是通过子类化 FrgCore 并提供虚拟成员函数的实现来实现的. 
 * 
 * FrgCore 的新实例由 FrgCoreFactory::new FrgCore() 例程创建.
 */
class FrgCore
{
	friend class SpinParser;
public:
	/**
	 * @brief 调用所有关联的测量协议。 
	 */
	void takeMeasurements() const
	{
		if (SpinParser::spinParser()->getComputationStatus().statusIdentifier == ComputationStatus::Identifier::Postprocessing)
		{
			//执行延迟测量
			for (auto m : _measurements)
			{
				if (_flowingFunctional->cutoff <= m->maxCutoff() && _flowingFunctional->cutoff >= m->minCutoff())
				{
					if (SpinParser::spinParser()->getCommandLineOptions()->deferMeasurements() || m->isDeferred()) m->takeMeasurement(*_flowingFunctional, SpinParser::spinParser()->isMasterRank());
				}
			}
		}
		else
		{
			//执行非延迟测量
			for (auto m : _measurements)
			{
				if (_flowingFunctional->cutoff <= m->maxCutoff() && _flowingFunctional->cutoff >= m->minCutoff())
				{
					if (!SpinParser::spinParser()->getCommandLineOptions()->deferMeasurements() && !m->isDeferred()) m->takeMeasurement(*_flowingFunctional, SpinParser::spinParser()->isMasterRank());
				}
			}

			//如果指定了延迟测量，则写入顶点输出
			bool postprocessingRequired = false;
			if (SpinParser::spinParser()->getCommandLineOptions()->deferMeasurements()) postprocessingRequired = true;
			for (auto m : _measurements) if (m->isDeferred()) postprocessingRequired = true;


			if (postprocessingRequired && SpinParser::spinParser()->isMasterRank()) _flowingFunctional->writeCheckpoint(SpinParser::spinParser()->getFileset().dataFile, true);
		}
	}

	/**
	 * @brief 流动方程求解中单个 RG 步骤的虚拟实现. 
	 * @details 该方法的具体实现预计将计算 FrgCore::flowing Function 中当前配置的流量方程，并将结果填充到 FrgCore::flow 中. 
	 * 预计不会进行任何进一步的修改. 
	 * 
	 * @see FrgCore::finalizeStep()
	 */
	virtual void computeStep() = 0;

	/**
	 * @brief 虚拟实现流动方程求解中单个 RG 步骤的最终确定. 
	 * @details 该方法的具体实现预计会更新FrgCore::flowingFunctional的值, 
	 * 基于流动FrgCore::flow 的值和指定的新频率截止值. 
	 * 
	 * @param newCutoff 新的截止值. 
	 */
	virtual void finalizeStep(float newCutoff) = 0;

	/**
	 * @brief 检索流动泛函.
	 *
	 * @return EffectiveAction* 流动泛函.
	 */
	EffectiveAction *flowingFunctional() const
	{
		return _flowingFunctional;
	}

	/**
	 * @brief 检索顶点流.
	 *
	 * @return EffectiveAction* 顶点流.
	 */
	EffectiveAction *flow() const
	{
		return _flow;
	}

	/**
	 * @brief 检索测量列表.
	 *
	 * @return std::vector<Measurement *> 测量列表.
	 */
	std::vector<Measurement *> measurements() const
	{
		return _measurements;
	}

protected:
	/**
	 * @brief 构造一个新的FrgCore，它拥有指定测量的所有权.
	 * @see Measurement
	 *
	 * @param measurements 求解流动方程期间调用的测量协议列表.
	 */
	FrgCore(const std::vector<Measurement *> &measurements) : _flowingFunctional(nullptr), _flow(nullptr), _measurements(measurements) {};

	/**
	 * @brief 销毁FrgCore对象并删除任何关联的测量协议.
	 */
	virtual ~FrgCore()
	{
		while (_measurements.size() > 0)
		{
			delete _measurements.back();
			_measurements.pop_back();
		}
	}

	EffectiveAction *_flowingFunctional; ///< 表示有效动作的当前状态. 
	EffectiveAction *_flow; ///< 与有效操作的当前状态相关的 RG 流的表示. 
	std::vector<Measurement *> _measurements; ///< 在整个流量方程求解过程中调用的测量协议列表. 
};