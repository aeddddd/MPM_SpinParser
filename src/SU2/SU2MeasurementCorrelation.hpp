/**
 * @file SU2MeasurementCorrelation.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) ģ�͵�����Բ���.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "Measurement.hpp"

/**
 * @brief SU(2) ģ�͵�����Բ���.
 */
class SU2MeasurementCorrelation : public Measurement
{
public:
	/**
	 * @brief ����һ���µ� SU2Measurement Correlation ���������������. 
	 * @see Measurement
	 * 
	 * @param outfile �ļ��� ������ļ�д���λ��. 
	 * @param minCutoff ���ò���Э�����С��ֵֹ. 
	 * @param maxCutoff ����ֵֹ�����ڸ�ֵ����ò���Э��. 
	 * @param defer �������Ϊ true���������Ƴٵ�����׶�. 
	 */
	SU2MeasurementCorrelation(const std::string &outfile, const float minCutoff, const float maxCutoff, const bool defer);
	
	/**
	 * @brief ���� SU2Measurement ��ض���. 
	 */
	~SU2MeasurementCorrelation();

	/**
	 * @brief ���в���. 
	 * @see Measurement::takeMeasurement()
	 *  
	 * @param state ִ�в�������Ч��������. 
	 * @param isMasterTask �������Ϊ true����������Ӧ����д������ļ�. 
	 */
	void takeMeasurement(const EffectiveAction &state, const bool isMasterTask) const override;

private: 
	/**
	 * @brief ����Ƶ���б������Ե������������. 
	 * 
	 * @param iterator Ƶ�ʵ�����. 
	 */
	void _calculateCorrelation(const int iterator) const;

	/**
	 * @brief д������ļ��а�����Ԫ��Ϣ. 
	 * @details �������ļ��в����ڣ��򴴽�һ�����ļ�. 
	 * ���HDF5��`observable Group/meta`�Ѿ����ڣ�������д�����.  
	 * 
	 * @param observableGroup ��� HDF5 �������. 
	 */
	void _writeOutfileHeader(const std::string &observableGroup) const;

	/**
	 * @brief д������ļ���������ݼ�.�����ǰ��ֵֹ�����ڼ��Ѵ��ڣ�������д�����. 
	 * 
	 * @param observableGroup ��� HDF5 �������. 
	 * @param correlation Ҫд����������. 
	 */
	void _writeOutfileCorrelation(const std::string &observableGroup, const float *correlation) const;

	float _currentCutoff; ///< ��������ԵĽ�ֵֹ. 
	float *_correlationsDD; ///< �����ܶ���ز����Ļ�����. 
	float *_correlationsZZ; ///< ������ز����Ļ�����. 
	int _memoryStepLattice; ///< ��ػ������е��ڴ���. 
};