/**
 * @file SU2VertexSingleParticle.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) 模型的单粒子顶点实现。
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <istream>
#include "lib/Assert.hpp"
#include "FrgCommon.hpp"

/**
 * @brief SU(2) 模型的单粒子顶点实现.
 */
struct SU2VertexSingleParticle
{
public:
	/**
	 * @brief 构造一个新的 SU2Vertex Single Particle 对象并将所有值初始化为零. 
	 */
	SU2VertexSingleParticle()
	{
		//设置尺寸
		size = FrgCommon::frequency().size;

		//分配和初始化内存
		_data = new float[size];
		for (int i = 0; i < size; ++i) _data[i] = 0.0f;
	}

	/**
	 * @brief 销毁 SU2Vertex 单粒子对象. 
	 */
	~SU2VertexSingleParticle()
	{
		delete[] _data;
	}

	/**
	 * @brief 在 [0,SU2VertexSingleParticle::size) 范围内展开线性迭代器. 
	 * 
	 * @param[in] iterator 迭代器进行扩展. 
	 * @param[out] w 迭代器描述的频率参数. 
	 */
	void expandIterator(const int iterator, float &w) const
	{
		ASSERT(iterator >= 0 && iterator < size);

		w = FrgCommon::frequency()._data[iterator];
	}

	/**
	 * @brief 通过 [0,SU2Vertex Single Particle::size) 范围内的线性迭代器通过引用直接访问顶点值. 
	 * 
	 * @param iterator 线性迭代器. 
	 * @return float& 顶点值. 
	 */
	float &getValueRef(const int iterator) const
	{
		ASSERT(iterator >= 0 && iterator < size);

		return _data[iterator];
	}

	/**
	 * @brief 通过对频率离散化执行线性插值来访问任意频率值的顶点值. 
	 * 
	 * @param w 频率参数. 
	 * @return float 顶点值. 
	 */
	float getValue(float w) const
	{
		int lower, upper;
		float bias;
		float sign = 1.0f;

		if (w < 0)
		{
			w = -w;
			sign = -1.0f;
		}

		FrgCommon::frequency().interpolateOffset(w, lower, upper, bias);
		return sign * ((1 - bias) * _directAccess(lower) + bias * _directAccess(upper));
	}

	/**
	 * @brief 访问给定频率网格点的顶点值. 
	 * 
	 * @param wOffset 频率网格上的线性偏移. 
	 * @return float& 顶点值. 
	 */
	float &_directAccess(const int wOffset) const
	{
		ASSERT(wOffset >= 0 && wOffset < FrgCommon::frequency().size);

		return _data[wOffset];
	}

	int size; ///< 顶点元素总数. 
	float *_data; ///< 顶点数据. 
};