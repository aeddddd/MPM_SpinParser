/**
 * @file FrgCommon.hpp
 * @author Finn Lasse Buessen
 * @brief Hub for central objects in pf-FRG calculations. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "FrequencyDiscretization.hpp"
#include "CutoffDiscretization.hpp"
#include "Lattice.hpp"

/**
 * @brief pf-FRG 计算中中心对象的中心.
 */
struct FrgCommon
{
	friend class SpinParser;
public:
	/**
	 * @brief 检索晶格表示. 
	 * 
	 * @return const Lattice& 格子表示. 
	 */
	static const Lattice &lattice()
	{
		return *_lattice;
	}

	/**
	 * @brief 检索松原频率离散化. 
	 * 
	 * @return const FrequencyDiscretization& 松原频率离散化. 
	 */
	static const FrequencyDiscretization &frequency()
	{
		return *_frequency;
	}

	/**
	 * @brief 检索频率截止离散化.
	 * 
	 * @return const CutoffDiscretization& 频率截止离散化. 
	 */
	static const CutoffDiscretization &cutoff()
	{
		return *_cutoff;
	}

private:
	static Lattice *_lattice; ///< 格子表示. 
	static FrequencyDiscretization *_frequency; ///< 松原频率离散化. 
	static CutoffDiscretization *_cutoff; ///< 频率截止离散化. 
};