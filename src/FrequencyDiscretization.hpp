/**
 * @file FrequencyDiscretization.hpp
 * @author Finn Lasse Buessen
 * @brief Discretization of Matsubara frequency space. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "lib/Exception.hpp"
#include "lib/Log.hpp"
#include "lib/Assert.hpp"

#pragma region FrequencyIterator
/**
 * @brief Frequency iterator. 
 */
struct FrequencyIterator
{
	/**
	 * @brief Construct a new FrequencyIterator object, and initialize to a specific frequency value. 
	 * 
	 * @param p Pointer to the initial value. 
	 */
	FrequencyIterator(float *p)
	{
		_pointer = p;
	}

	/**
	 * @brief Dereference operator. 
	 * 
	 * @return float Value of the frequency which the iterator points to. 
	 */
	float operator*() const
	{
		return *_pointer;
	}

	/**
	 * @brief Iterator comparison. 
	 * 
	 * @param rhs Right hand side operand. 
	 * @return bool Returns true if the iterators point to the same frequency value, otherwise returns false. 
	 */
	bool operator==(const FrequencyIterator &rhs) const
	{
		return _pointer == rhs._pointer;
	}

	/**
	 * @brief Negative iterator comparison. 
	 * 
	 * @param rhs Right hand side operand. 
	 * @return bool Returns false if the iterators point to the same frequency value, otherwise returns true. 
	 */
	bool operator!=(const FrequencyIterator &rhs) const
	{
		return _pointer != rhs._pointer;
	}

	/**
	 * @brief Prefix increment operator. 
	 * 
	 * @return FrequencyIterator& Reference to self. 
	 */
	FrequencyIterator& operator++()
	{
		++_pointer;
		return *this;
	}

	/**
	 * @brief ǰ׺�Լ������. 
	 * 
	 * @return FrequencyIterator& Reference to self. 
	 */
	FrequencyIterator& operator--()
	{
		--_pointer;
		return *this;
	}
	
	/**
	 * @brief �������ӷ������. 
	 * 
	 * @param rhs �����������Ĳ���. 
	 * @return FrequencyIterator ��������ָ����������. 
	 */
	FrequencyIterator operator+(const int rhs) const
	{
		return FrequencyIterator(_pointer + rhs);
	}

	/**
	 * @brief �������������. 
	 * 
	 * @param rhs �ݼ��������Ĳ���. 
	 * @return FrequencyIterator �������ݼ�ָ���Ĳ���. 
	 */
	FrequencyIterator operator-(const int rhs) const
	{
		return FrequencyIterator(_pointer - rhs);
	}

	/**
	 * @brief ����ıȽ������. 
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool �������������ָ��ֵ�򷵻� true�����򷵻� false�� 
	 */
	bool operator>(const FrequencyIterator &rhs) const
	{
		return _pointer > rhs._pointer;
	}
	
	/**
	 * @brief ���ڻ���ڱȽ������. 
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool ������������ڻ����ָ��ֵ�򷵻� true�����򷵻� false. 
	 */
	bool operator>=(const FrequencyIterator &rhs) const
	{
		return _pointer >= rhs._pointer;
	}
	
	/**
	 * @brief ��С�ıȽ������. 
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool ���������С��ָ��ֵ�򷵻� true�����򷵻� false. 
	 */

	bool operator<(const FrequencyIterator &rhs) const
	{
		return _pointer < rhs._pointer;
	}
	
	/**
	 * @brief С�ڻ���ڱȽ������. 
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool ���������С�ڻ����ָ��ֵ���򷵻� true. 
	 */
	bool operator<=(const FrequencyIterator &rhs) const
	{
		return _pointer <= rhs._pointer;
	}

	float *_pointer; ///< Pointer which the iterator currently points to. 
};
#pragma endregion

/**
 * @brief ��ԭƵ�ʿռ����ɢ��. 
 * @details �ýṹ��ʾ����ָ��������б����ԭƵ�ʿռ����ɢ��. 
 * ���ṩ�˵���Ƶ�����񡢲������������Լ����Բ�ֵ����ֵ�ķ����������֮��. 
 * ������ԭ�㾵��Գ�.  
 */
struct FrequencyDiscretization
{
public:
	/**
	 * @brief ����ָ��������б���Ƶ����ɢ��. 
	 * ָ���������б���밴����������. 
	 * �Զ�������Գ�����صĸ�ֵ. 
	 * 
	 * @param values ������б�. 
	 */
	FrequencyDiscretization(const std::vector<float> &values)
	{
		//ȷ��Ƶ��ֵ����������
		ASSERT(std::is_sorted(values.begin(), values.end()));

		//ȷ����ɢ�������㹻���Ƶ��
		if (values.size() < 2) throw Exception(Exception::Type::ArgumentError, "FrequencyDiscretization must contain at least two frequency values");
		
		//�����ڴ沢�洢Ƶ��
		size = int(values.size());
		_dataNegative = new float[2 * size];
		_data = _dataNegative + size;
		
		for (int i = 0; i < size; ++i)
		{
			ASSERT(values[i] > 0);
			_data[-i- 1] = -values[i];
			_data[i] = values[i];
		}

		Log::log << Log::LogLevel::Debug<< "Initialized frequency grid with mesh values" << Log::endl;
		for (auto i = beginNegative(); i != end(); ++i)	Log::log << "\t" << *i << Log::endl;
	}

	/**
	 * @brief ����Ƶ����ɢ������. 
	 */
	~FrequencyDiscretization()
	{
		delete[] _dataNegative;
	}

	/**
	 * @brief ��������������һ��������㣻���Ǿ���ֵ��С����ֵ. 
	 * 
	 * @return FrequencyIterator ����һ���������ĵ�����. 
	 */
	FrequencyIterator begin() const
	{
		return FrequencyIterator(_data);
	}

	/**
	 * @brief ��������������һ��������㣻���Ǿ���ֵ���ĸ�ֵ. 
	 * 
	 * @return FrequencyIterator ����һ���������ĵ�����.
	 */
	FrequencyIterator beginNegative() const
	{
		return FrequencyIterator(_dataNegative);
	}

	/**
	 * @brief ���������������һ������㣻���Ǿ���ֵ������ֵ. 
	 * 
	 * @return FrequencyIterator �����һ�������ĵ�����. 
	 */
	FrequencyIterator last() const
	{
		return FrequencyIterator(_data + size - 1);
	}

	/**
	 * @brief ���������������+1�������. 
	 * 
	 * @return FrequencyIterator ���������+1�������. 
	 */
	FrequencyIterator end() const
	{
		return FrequencyIterator(_data + size);
	}

	/**
	 * @brief ����С��ָ��Ƶ��ֵ����������ĵ�����. 
	 * ��������ڸ�С������㣬�򷵻�һ��������������������.  
	 * 
	 * @param w ָ��Ƶ������. 
	 * @return FrequencyIterator ����������ĵ�����. 
	 */
	FrequencyIterator lesser(const float w) const
	{
		if (w < 0)
		{
			FrequencyIterator it = greater(-w);
			it._pointer = _data - (it._pointer - _data + 1);
			return it;
		}
		else
		{
			if (w <= _data[0]) return FrequencyIterator(_data);
			for (int i = 1; i < size; ++i)
			{
				if (_data[i] > w) return FrequencyIterator(_data + i - 1);
			}
			return FrequencyIterator(_data + size - 1);
		}
	}

	/**
	 * @brief ��������ָ��Ƶ��ֵ����������ĵ�����. 
	 * ��������ڸ��������㣬�򷵻�һ��������������������.  
	 * 
	 * @param w ָ��Ƶ������. 
	 * @return FrequencyIterator ����������ĵ�����. 
	 */
	FrequencyIterator greater(const float w) const
	{
		if (w < 0)
		{
			FrequencyIterator it = lesser(-w);
			it._pointer = _data - (it._pointer - _data + 1);
			return it;
		}
		else
		{
			if (w <= _data[0]) return FrequencyIterator(_data);
			for (int i = 1; i < size; ++i)
			{
				if (_data[i] > w) return FrequencyIterator(_data + i);
			}
			return FrequencyIterator(_data + size - 1);
		}
	}

	/**
	 * @brief ���������Ƶ��ֵ��ص����������ڵ�һ���������ĵ�����������. 
	 * ָ��Ƶ�ʵ�ֵ����Ϊ����. 
	 * ��������ھ��и�ֵ������㣬�򷵻����һ������������. 
	 * 
	 * @param w Ƶ��ֵ������������. 
	 * @return int ���������. 
	 */
	int offset(const float w) const
	{
		ASSERT(w >= 0);

		if (w <= _data[0]) return 0;
		for (int i = 1; i < size; ++i)
		{
			if (_data[i] >= w) return i;
		}
		return size - 1;
	}	

	/**
	 * @brief �������֮��ִ��������Ƶ�ʵĲ�ֵ. 
	 * 
	 * @param[in] w Ҫ��ֵ��Ƶ��ֵ������������. 
	 * @param[out] lowerOffset ��С���������ڵ�һ���������ĵ�����������. 
	 * @param[out] upperOffset ����ڵ�һ���������Ľϴ������ĵ�����������. 
	 * @param[out] bias ���Բ�ֵȨ��.�����w���롰lower Offset�����������ƥ��:0. �����w���롰upper Offset�����������ƥ��:1. 
	 */
	void interpolateOffset(const float w, int &lowerOffset, int &upperOffset, float &bias) const
	{
		ASSERT(w >= 0);
		ASSERT(&lowerOffset != &upperOffset)

		if (w <= _data[0])
		{
			lowerOffset = 0; 
			upperOffset = 0;
			bias = 0.0f;
			return;
		}
		for (int i = 1; i < size; ++i)
		{
			if (_data[i] > w)
			{
				upperOffset = i;
				lowerOffset = i - 1;
				bias = (w - _data[lowerOffset]) / (_data[upperOffset] - _data[lowerOffset]);
				return;
			}
		}
		lowerOffset = size - 1;
		upperOffset = size - 1;
		bias = 0.0f;
		return;

		ASSERT(lowerOffset >= 0 && lowerOffset < size);
		ASSERT(upperOffset >= 0 && upperOffset < size);
		ASSERT(bias >= 0.0f && bias <= 1.0f);
	}

	int size; ///< ���������. 
	float *_data; ///< ָ���һ����������ָ�롣֮���������� FrequencyDiscretization::_dataNegative. 
	float *_dataNegative; ///< ָ���һ����������ָ��. 
};
