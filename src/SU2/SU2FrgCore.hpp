/**
 * @file SU2FrgCore.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) ģ�͵� FRG ����ʵ��.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "FrgCore.hpp"

/**
 * @brief SU(2) ģ�͵� FRG ����ʵ��.
 */
class SU2FrgCore : public FrgCore
{
public:
	/**
	 * @brief ����һ���µ� SU2FrgCore,ʹ��ָ��������ģ�ͽ��г�ʼ������Ӳ���. 
	 * 
	 * @param spinModel ����ģ�ͳ�ʼ����Ч����. 
	 * @param measurements Ҫ��ӵĲ���ֵ. 
	 * @param options �����ļ����ṩ�ĺ���ѡ����ַ�����ʽ�б�. 
	 */
	SU2FrgCore(const SpinModel &spinModel, const std::vector<Measurement *> &measurements, const std::map<std::string, std::string> &options);
	
	/**
	 * @brief ����SU2FrgCore����. 
	 */
	~SU2FrgCore();

	/**
	 * @brief ������������. 
	 */
	void computeStep() override;
	
	/**
	 * @brief ����������̵ļ���. 
	 * 
	 * @param newCutoff �����ƶ��������½�ֹ��. 
	 */
	void finalizeStep(const float newCutoff) override;

	float spinLength; ///< S��ֵ,������������. 
	float normalization; ///< ������һ������. 

private:
	int dataStacks[8]; ///< ��LoadManager::DataStack������. 

	/**
	 * @brief �����ض����Ե������ĵ����Ӷ�����,�õ�����ͨ�� SU2VertexSingleParticle::expand Iterator() ������չ.
	 * 
	 * @param iterator ���Ե�����. 
	 */
	void _calculateVertexSingleParticle(const int iterator);

	/**
	 * @brief �����ض����Ե�������˫���Ӷ�����,�õ�����ͨ�� SU2VertexTwoParticle::expandIterator() ������չ.
	 * 
	 * @param iterator ���Ե�����. 
	 */
	void _calculateVertexTwoParticle(const int iterator);
};