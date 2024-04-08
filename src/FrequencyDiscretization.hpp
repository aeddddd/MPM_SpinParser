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
	 * @brief 前缀自减运算符. 
	 * 
	 * @return FrequencyIterator& Reference to self. 
	 */
	FrequencyIterator& operator--()
	{
		--_pointer;
		return *this;
	}
	
	/**
	 * @brief 迭代器加法运算符. 
	 * 
	 * @param rhs 迭代器递增的步数. 
	 * @return FrequencyIterator 迭代器按指定步数递增. 
	 */
	FrequencyIterator operator+(const int rhs) const
	{
		return FrequencyIterator(_pointer + rhs);
	}

	/**
	 * @brief 迭代减法运算符. 
	 * 
	 * @param rhs 递减迭代器的步数. 
	 * @return FrequencyIterator 迭代器递减指定的步数. 
	 */
	FrequencyIterator operator-(const int rhs) const
	{
		return FrequencyIterator(_pointer - rhs);
	}

	/**
	 * @brief 更大的比较运算符. 
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 如果迭代器大于指定值则返回 true，否则返回 false。 
	 */
	bool operator>(const FrequencyIterator &rhs) const
	{
		return _pointer > rhs._pointer;
	}
	
	/**
	 * @brief 大于或等于比较运算符. 
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 如果迭代器大于或等于指定值则返回 true，否则返回 false. 
	 */
	bool operator>=(const FrequencyIterator &rhs) const
	{
		return _pointer >= rhs._pointer;
	}
	
	/**
	 * @brief 较小的比较运算符. 
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 如果迭代器小于指定值则返回 true，否则返回 false. 
	 */

	bool operator<(const FrequencyIterator &rhs) const
	{
		return _pointer < rhs._pointer;
	}
	
	/**
	 * @brief 小于或等于比较运算符. 
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 如果迭代器小于或等于指定值，则返回 true. 
	 */
	bool operator<=(const FrequencyIterator &rhs) const
	{
		return _pointer <= rhs._pointer;
	}

	float *_pointer; ///< Pointer which the iterator currently points to. 
};
#pragma endregion

/**
 * @brief 松原频率空间的离散化. 
 * @details 该结构表示基于指定网格点列表的松原频率空间的离散化. 
 * 它提供了迭代频率网格、查找最近网格点以及线性插值任意值的方法在网格点之间. 
 * 网格绕原点镜像对称.  
 */
struct FrequencyDiscretization
{
public:
	/**
	 * @brief 根据指定网格点列表构建频率离散化. 
	 * 指定网格点的列表必须按升序且正定. 
	 * 自动生成与对称性相关的负值. 
	 * 
	 * @param values 网格点列表. 
	 */
	FrequencyDiscretization(const std::vector<float> &values)
	{
		//确保频率值按升序排列
		ASSERT(std::is_sorted(values.begin(), values.end()));

		//确保离散化包含足够多的频率
		if (values.size() < 2) throw Exception(Exception::Type::ArgumentError, "FrequencyDiscretization must contain at least two frequency values");
		
		//分配内存并存储频率
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
	 * @brief 销毁频率离散化对象. 
	 */
	~FrequencyDiscretization()
	{
		delete[] _dataNegative;
	}

	/**
	 * @brief 检索迭代器到第一个正网格点；这是绝对值最小的正值. 
	 * 
	 * @return FrequencyIterator 到第一个正网格点的迭代器. 
	 */
	FrequencyIterator begin() const
	{
		return FrequencyIterator(_data);
	}

	/**
	 * @brief 检索迭代器到第一个负网格点；这是绝对值最大的负值. 
	 * 
	 * @return FrequencyIterator 到第一个负网格点的迭代器.
	 */
	FrequencyIterator beginNegative() const
	{
		return FrequencyIterator(_dataNegative);
	}

	/**
	 * @brief 检索迭代器到最后一个网格点；这是绝对值最大的正值. 
	 * 
	 * @return FrequencyIterator 到最后一个网格点的迭代器. 
	 */
	FrequencyIterator last() const
	{
		return FrequencyIterator(_data + size - 1);
	}

	/**
	 * @brief 检索迭代器到最后+1个网格点. 
	 * 
	 * @return FrequencyIterator 迭代到最后+1个网格点. 
	 */
	FrequencyIterator end() const
	{
		return FrequencyIterator(_data + size);
	}

	/**
	 * @brief 检索小于指定频率值的最近网格点的迭代器. 
	 * 如果不存在更小的网格点，则返回一个迭代器到最近的网格点.  
	 * 
	 * @param w 指定频率上限. 
	 * @return FrequencyIterator 到最近网格点的迭代器. 
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
	 * @brief 检索大于指定频率值的最近网格点的迭代器. 
	 * 如果不存在更大的网格点，则返回一个迭代器到最近的网格点.  
	 * 
	 * @param w 指定频率上限. 
	 * @return FrequencyIterator 到最近网格点的迭代器. 
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
	 * @brief 返回与给定频率值相关的网格点相对于第一个正网格点的迭代器增量数. 
	 * 指定频率的值必须为正数. 
	 * 如果不存在具有该值的网格点，则返回最后一个网格点的索引. 
	 * 
	 * @param w 频率值。必须是正的. 
	 * @return int 网格点索引. 
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
	 * @brief 在网格点之间执行任意正频率的插值. 
	 * 
	 * @param[in] w 要插值的频率值。必须是正的. 
	 * @param[out] lowerOffset 较小网格点相对于第一个正网格点的迭代器增量数. 
	 * @param[out] upperOffset 相对于第一个正网格点的较大网格点的迭代器增量数. 
	 * @param[out] bias 线性插值权重.如果“w”与“lower Offset”处的网格点匹配:0. 如果“w”与“upper Offset”处的网格点匹配:1. 
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

	int size; ///< 正网格点数. 
	float *_data; ///< 指向第一个正网格点的指针。之后连续保存 FrequencyDiscretization::_dataNegative. 
	float *_dataNegative; ///< 指向第一个负网格点的指针. 
};
