/**
 * @file CutoffDiscretization.hpp
 * @author Finn Lasse Buessen
 * @brief Representation of a discretized frequency cutoff axis. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <vector>
#include "lib/Exception.hpp"

#pragma region CutoffIterator
/**
 * @brief ��ɢ��ֵֹ�ĵ�����. 
 */
struct CutoffIterator
{
public:
	/**
	 * @brief ����һ��ָ���ض�ֵ���� Cutoff Iterator ����. 
	 * 
	 * @param p ָ���ʼֵ��ָ��. 
	 */
	CutoffIterator(float* p) : _pointer(p) {};

	/**
	 * @brief ������������. 
	 * 
	 * @return float ��ֵֹ. 
	 */
	float operator*() const
	{
		return *_pointer;
	}

	/**
	 * @brief �������Ƚ�
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool ����������򷵻� true�����򷵻� false.  
	 */
	bool operator==(const CutoffIterator& rhs) const
	{
		return _pointer == rhs._pointer;
	}
	
	/**
	 * @brief ���������Ƚ�
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool ����������ȷ��� true�����򷵻� false.   
	 */
	bool operator!=(const CutoffIterator& rhs) const
	{
		return _pointer != rhs._pointer;
	}
	
	/**
	 * @brief ǰ׺����. 
	 * 
	 * @return ��ֹ�������Ͷ����������. 
	 */
	CutoffIterator& operator++()
	{
		++_pointer;
		return *this;
	}

private:
	float *_pointer; ///< ��������ǰָ���ָ��. 
};
#pragma endregion

/**
 * @brief Ƶ�����ֹ��ɢ���ı�ʾ. 
 */
struct CutoffDiscretization
{
public:
	/**
	 * @brief �ӽ�ֵֹ�б���һ���µĽ�ֹ��ɢ������. 
	 * 
	 * @param values ������ɢ���Ľ�ֵֹ�б�. 
	 */
	CutoffDiscretization(const std::vector<float> &values)
	{
		//ȷ����ɢ�������㹻��Ľ�ֵֹ
		if (values.size() < 2) throw Exception(Exception::Type::ArgumentError, "��ֹ��ɢ�����������������Ƶ��ֵ");

		_size = int(values.size());
		_data = new float[values.size()];
		memcpy(_data, values.data(), values.size() * sizeof(float));
	}

	/**
	 * @brief ���ٽ�ֹ��ɢ������
 	*/
	~CutoffDiscretization()
	{
		delete[] _data;
	}

	/**
	 * @brief ��������������һ����ɢ��ֵ. 
	 * 
	 * @return CutoffIterator ��һ����ɢֵ�ĵ�����. 
	 */
	CutoffIterator begin() const
	{
		return CutoffIterator(_data);
	}

	/**
	 * @brief ���������������һ����ɢ��ֵ. 
	 * 
	 * @return CutoffIterator ���������һ����ɢֵ. 
	 */
	CutoffIterator last() const
	{
		return CutoffIterator(_data + _size - 1);
	}

	/**
	 * @brief ���������������+1��ɢ��ֵ. 
	 * 
	 * @return CutoffIterator ���������+1��ɢ��ֵ. 
	 */
	CutoffIterator end() const
	{
		return CutoffIterator(_data + _size);
	}

	/**
	 * @brief ����ָ���ض���ֵֹ�ĵ�����. 
	 * 
	 * @param cutoff ��ֹ����ֵ. 
	 * @return CutoffIterator ָ��ָ����ֵֹ��������ڣ��ĵ�����������ָ��last+1. 
	 */
	CutoffIterator find(const float cutoff) const
	{
		for (int i = 0; i < _size; ++i)
		{
			if (_data[i] == cutoff) return CutoffIterator(_data + i);
		}
		return end();
	}

private:
	int _size; ///< ��ɢ���еĽ�ֵֹ����. 
	float *_data; ///< ��ɢֵ���ڲ��洢. 
};