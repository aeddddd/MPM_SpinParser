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
 * @brief 离散截止值的迭代器. 
 */
struct CutoffIterator
{
public:
	/**
	 * @brief 构造一个指向特定值的新 Cutoff Iterator 对象. 
	 * 
	 * @param p 指向初始值的指针. 
	 */
	CutoffIterator(float* p) : _pointer(p) {};

	/**
	 * @brief 迭代器解引用. 
	 * 
	 * @return float 截止值. 
	 */
	float operator*() const
	{
		return *_pointer;
	}

	/**
	 * @brief 迭代器比较
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 迭代器相等则返回 true，否则返回 false.  
	 */
	bool operator==(const CutoffIterator& rhs) const
	{
		return _pointer == rhs._pointer;
	}
	
	/**
	 * @brief 负迭代器比较
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 迭代器不相等返回 true，否则返回 false.   
	 */
	bool operator!=(const CutoffIterator& rhs) const
	{
		return _pointer != rhs._pointer;
	}
	
	/**
	 * @brief 前缀增量. 
	 * 
	 * @return 截止迭代器和对自身的引用. 
	 */
	CutoffIterator& operator++()
	{
		++_pointer;
		return *this;
	}

private:
	float *_pointer; ///< 迭代器当前指向的指针. 
};
#pragma endregion

/**
 * @brief 频率轴截止离散化的表示. 
 */
struct CutoffDiscretization
{
public:
	/**
	 * @brief 从截止值列表构造一个新的截止离散化对象. 
	 * 
	 * @param values 用于离散化的截止值列表. 
	 */
	CutoffDiscretization(const std::vector<float> &values)
	{
		//确保离散化包含足够多的截止值
		if (values.size() < 2) throw Exception(Exception::Type::ArgumentError, "截止离散化必须包含至少两个频率值");

		_size = int(values.size());
		_data = new float[values.size()];
		memcpy(_data, values.data(), values.size() * sizeof(float));
	}

	/**
	 * @brief 销毁截止离散化对象
 	*/
	~CutoffDiscretization()
	{
		delete[] _data;
	}

	/**
	 * @brief 检索迭代器到第一个离散化值. 
	 * 
	 * @return CutoffIterator 第一个离散值的迭代器. 
	 */
	CutoffIterator begin() const
	{
		return CutoffIterator(_data);
	}

	/**
	 * @brief 检索迭代器到最后一个离散化值. 
	 * 
	 * @return CutoffIterator 迭代到最后一个离散值. 
	 */
	CutoffIterator last() const
	{
		return CutoffIterator(_data + _size - 1);
	}

	/**
	 * @brief 检索迭代器到最后+1离散化值. 
	 * 
	 * @return CutoffIterator 迭代到最后+1离散化值. 
	 */
	CutoffIterator end() const
	{
		return CutoffIterator(_data + _size);
	}

	/**
	 * @brief 检索指向特定截止值的迭代器. 
	 * 
	 * @param cutoff 截止搜索值. 
	 * @return CutoffIterator 指向指定截止值（如果存在）的迭代器；否则指向last+1. 
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
	int _size; ///< 离散化中的截止值数量. 
	float *_data; ///< 离散值的内部存储. 
};