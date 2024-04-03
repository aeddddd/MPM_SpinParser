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
 * @brief pf-FRG ���������Ķ��������.
 */
struct FrgCommon
{
	friend class SpinParser;
public:
	/**
	 * @brief ���������ʾ. 
	 * 
	 * @return const Lattice& ���ӱ�ʾ. 
	 */
	static const Lattice &lattice()
	{
		return *_lattice;
	}

	/**
	 * @brief ������ԭƵ����ɢ��. 
	 * 
	 * @return const FrequencyDiscretization& ��ԭƵ����ɢ��. 
	 */
	static const FrequencyDiscretization &frequency()
	{
		return *_frequency;
	}

	/**
	 * @brief ����Ƶ�ʽ�ֹ��ɢ��.
	 * 
	 * @return const CutoffDiscretization& Ƶ�ʽ�ֹ��ɢ��. 
	 */
	static const CutoffDiscretization &cutoff()
	{
		return *_cutoff;
	}

private:
	static Lattice *_lattice; ///< ���ӱ�ʾ. 
	static FrequencyDiscretization *_frequency; ///< ��ԭƵ����ɢ��. 
	static CutoffDiscretization *_cutoff; ///< Ƶ�ʽ�ֹ��ɢ��. 
};