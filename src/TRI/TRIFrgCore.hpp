/**
 * @file TRIFrgCore.hpp
 * @author Finn Lasse Buessen
 * @brief FrgCore implementation for time reversal invariant models.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "FrgCore.hpp"

/**
 * @brief ʱ�䷴ת����ģ�͵� FrgCore ʵ��.
 */
class TRIFrgCore : public FrgCore
{
public:
	/**
	 * @brief ����һ���µ� TRIFrgCore,ʹ��ָ��������ģ�ͽ��г�ʼ������Ӳ���. 
	 * 
	 * @param spinModel ����ģ�ͳ�ʼ����Ч����. 
	 * @param measurements Ҫ��ӵĲ���ֵ. 
	 * @param options �����ļ����ṩ�ĺ���ѡ����ַ�����ʽ�б�. 
	 */
	TRIFrgCore(const SpinModel &spinModel, const std::vector<Measurement *> &measurements, const std::map<std::string, std::string> &options);
	
	/**
	 * @brief ���� TRIFrgCore ����. 
	 */
	~TRIFrgCore();

	/**
	 * @brief ������������. 
	 */
	void computeStep() override;

	/**
	 * @brief �������������̵ļ���. 
	 * 
	 * @param newCutoff �ƶ��������½�ֹ��. 
	 */
	void finalizeStep(const float newCutoff) override;

	float normalization; ///< ������һ������. 

private:
	int dataStacks[6]; ///<�� LoadManager::DataStack ������. 

	/**
	 * @brief �����ض����Ե������ĵ����Ӷ�����,�õ�����ͨ�� TRIVertexSingleParticle::expand Iterator() ������չ.
	 * 
	 * @param iterator ���Ե�����. 
	 */
	void _calculateVertexSingleParticle(const int iterator);

	/**
	 * @brief �����ض����Ե�������˫���Ӷ�����,�õ�����ͨ�� TRIVertexTwoParticle::expand Iterator() չ��.
	 * 
	 * @param iterator ���Ե�����. 
	 */
	void _calculateVertexTwoParticle(const int iterator);
};