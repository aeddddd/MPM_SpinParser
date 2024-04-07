/**
 * @file SU2VertexSingleParticle.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) ģ�͵ĵ����Ӷ���ʵ�֡�
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <istream>
#include "lib/Assert.hpp"
#include "FrgCommon.hpp"

/**
 * @brief SU(2) ģ�͵ĵ����Ӷ���ʵ��.
 */
struct SU2VertexSingleParticle
{
public:
	/**
	 * @brief ����һ���µ� SU2Vertex Single Particle ���󲢽�����ֵ��ʼ��Ϊ��. 
	 */
	SU2VertexSingleParticle()
	{
		//���óߴ�
		size = FrgCommon::frequency().size;

		//����ͳ�ʼ���ڴ�
		_data = new float[size];
		for (int i = 0; i < size; ++i) _data[i] = 0.0f;
	}

	/**
	 * @brief ���� SU2Vertex �����Ӷ���. 
	 */
	~SU2VertexSingleParticle()
	{
		delete[] _data;
	}

	/**
	 * @brief �� [0,SU2VertexSingleParticle::size) ��Χ��չ�����Ե�����. 
	 * 
	 * @param[in] iterator ������������չ. 
	 * @param[out] w ������������Ƶ�ʲ���. 
	 */
	void expandIterator(const int iterator, float &w) const
	{
		ASSERT(iterator >= 0 && iterator < size);

		w = FrgCommon::frequency()._data[iterator];
	}

	/**
	 * @brief ͨ�� [0,SU2Vertex Single Particle::size) ��Χ�ڵ����Ե�����ͨ������ֱ�ӷ��ʶ���ֵ. 
	 * 
	 * @param iterator ���Ե�����. 
	 * @return float& ����ֵ. 
	 */
	float &getValueRef(const int iterator) const
	{
		ASSERT(iterator >= 0 && iterator < size);

		return _data[iterator];
	}

	/**
	 * @brief ͨ����Ƶ����ɢ��ִ�����Բ�ֵ����������Ƶ��ֵ�Ķ���ֵ. 
	 * 
	 * @param w Ƶ�ʲ���. 
	 * @return float ����ֵ. 
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
	 * @brief ���ʸ���Ƶ�������Ķ���ֵ. 
	 * 
	 * @param wOffset Ƶ�������ϵ�����ƫ��. 
	 * @return float& ����ֵ. 
	 */
	float &_directAccess(const int wOffset) const
	{
		ASSERT(wOffset >= 0 && wOffset < FrgCommon::frequency().size);

		return _data[wOffset];
	}

	int size; ///< ����Ԫ������. 
	float *_data; ///< ��������. 
};