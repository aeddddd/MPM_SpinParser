/**
 * @file SU2MeasurementCorrelation.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) 模型的相关性测量.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "Measurement.hpp"

/**
 * @brief SU(2) 模型的相关性测量.
 */
class SU2MeasurementCorrelation : public Measurement
{
public:
	/**
	 * @brief 构造一个新的 SU2Measurement Correlation 对象来测量相关性. 
	 * @see Measurement
	 * 
	 * @param outfile 文件名 将结果文件写入的位置. 
	 * @param minCutoff 调用测量协议的最小截止值. 
	 * @param maxCutoff 最大截止值，低于该值则调用测量协议. 
	 * @param defer 如果设置为 true，测量将推迟到后处理阶段. 
	 */
	SU2MeasurementCorrelation(const std::string &outfile, const float minCutoff, const float maxCutoff, const bool defer);
	
	/**
	 * @brief 销毁 SU2Measurement 相关对象. 
	 */
	~SU2MeasurementCorrelation();

	/**
	 * @brief 进行测量. 
	 * @see Measurement::takeMeasurement()
	 *  
	 * @param state 执行测量的有效动作对象. 
	 * @param isMasterTask 如果设置为 true，则函数调用应负责写入输出文件. 
	 */
	void takeMeasurement(const EffectiveAction &state, const bool isMasterTask) const override;

private: 
	/**
	 * @brief 计算频率列表中线性迭代器的相关性. 
	 * 
	 * @param iterator 频率迭代器. 
	 */
	void _calculateCorrelation(const int iterator) const;

	/**
	 * @brief 写入输出文件中包含的元信息. 
	 * @details 如果输出文件尚不存在，则创建一个新文件. 
	 * 如果HDF5组`observable Group/meta`已经存在，则跳过写入过程.  
	 * 
	 * @param observableGroup 输出 HDF5 组的名称. 
	 */
	void _writeOutfileHeader(const std::string &observableGroup) const;

	/**
	 * @brief 写入输出文件的相关数据集.如果当前截止值的日期集已存在，则跳过写入过程. 
	 * 
	 * @param observableGroup 输出 HDF5 组的名称. 
	 * @param correlation 要写入的相关数据. 
	 */
	void _writeOutfileCorrelation(const std::string &observableGroup, const float *correlation) const;

	float _currentCutoff; ///< 计算相关性的截止值. 
	float *_correlationsDD; ///< 用于密度相关测量的缓冲区. 
	float *_correlationsZZ; ///< 自旋相关测量的缓冲区. 
	int _memoryStepLattice; ///< 相关缓冲区中的内存跨度. 
};