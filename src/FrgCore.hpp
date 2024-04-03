/**
 * @file FrgCore.hpp
 * @author Finn Lasse Buessen
 * @brief Numerics core for pf-FRG calculations. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <vector>
#include "EffectiveAction.hpp"
#include "Measurement.hpp"
#include "SpinModel.hpp"
#include "SpinParser.hpp"

class SpinParser;

/**
 * @brief pf-FRG ��ֵ���ĵ�����ʵ��. 
 * @details FrgCore ���� pf-FRG �����������ֵ��Ԫ. 
 * ��������΢�ַ��������������Э����������̵ľ���ʵ��֮��Ľӿ�. 
 * FrgCore�������˶�д����Ľӿ�. 
 *
 * ÿ���ض��Ĳ�ͬ�Գ�Լ����ʽ�������������Դ�Դ���. 
 * �Զ�����������ͨ�����໯ FrgCore ���ṩ�����Ա������ʵ����ʵ�ֵ�. 
 * 
 * FrgCore ����ʵ���� FrgCoreFactory::new FrgCore() ���̴���.
 */
class FrgCore
{
	friend class SpinParser;
public:
	/**
	 * @brief �������й����Ĳ���Э�顣 
	 */
	void takeMeasurements() const
	{
		if (SpinParser::spinParser()->getComputationStatus().statusIdentifier == ComputationStatus::Identifier::Postprocessing)
		{
			//ִ���ӳٲ���
			for (auto m : _measurements)
			{
				if (_flowingFunctional->cutoff <= m->maxCutoff() && _flowingFunctional->cutoff >= m->minCutoff())
				{
					if (SpinParser::spinParser()->getCommandLineOptions()->deferMeasurements() || m->isDeferred()) m->takeMeasurement(*_flowingFunctional, SpinParser::spinParser()->isMasterRank());
				}
			}
		}
		else
		{
			//ִ�з��ӳٲ���
			for (auto m : _measurements)
			{
				if (_flowingFunctional->cutoff <= m->maxCutoff() && _flowingFunctional->cutoff >= m->minCutoff())
				{
					if (!SpinParser::spinParser()->getCommandLineOptions()->deferMeasurements() && !m->isDeferred()) m->takeMeasurement(*_flowingFunctional, SpinParser::spinParser()->isMasterRank());
				}
			}

			//���ָ�����ӳٲ�������д�붥�����
			bool postprocessingRequired = false;
			if (SpinParser::spinParser()->getCommandLineOptions()->deferMeasurements()) postprocessingRequired = true;
			for (auto m : _measurements) if (m->isDeferred()) postprocessingRequired = true;


			if (postprocessingRequired && SpinParser::spinParser()->isMasterRank()) _flowingFunctional->writeCheckpoint(SpinParser::spinParser()->getFileset().dataFile, true);
		}
	}

	/**
	 * @brief ������������е��� RG ���������ʵ��. 
	 * @details �÷����ľ���ʵ��Ԥ�ƽ����� FrgCore::flowing Function �е�ǰ���õ��������̣����������䵽 FrgCore::flow ��. 
	 * Ԥ�Ʋ�������κν�һ�����޸�. 
	 * 
	 * @see FrgCore::finalizeStep()
	 */
	virtual void computeStep() = 0;

	/**
	 * @brief ����ʵ��������������е��� RG ���������ȷ��. 
	 * @details �÷����ľ���ʵ��Ԥ�ƻ����FrgCore::flowingFunctional��ֵ, 
	 * ��������FrgCore::flow ��ֵ��ָ������Ƶ�ʽ�ֵֹ. 
	 * 
	 * @param newCutoff �µĽ�ֵֹ. 
	 */
	virtual void finalizeStep(float newCutoff) = 0;

	/**
	 * @brief ������������.
	 *
	 * @return EffectiveAction* ��������.
	 */
	EffectiveAction *flowingFunctional() const
	{
		return _flowingFunctional;
	}

	/**
	 * @brief ����������.
	 *
	 * @return EffectiveAction* ������.
	 */
	EffectiveAction *flow() const
	{
		return _flow;
	}

	/**
	 * @brief ���������б�.
	 *
	 * @return std::vector<Measurement *> �����б�.
	 */
	std::vector<Measurement *> measurements() const
	{
		return _measurements;
	}

protected:
	/**
	 * @brief ����һ���µ�FrgCore����ӵ��ָ������������Ȩ.
	 * @see Measurement
	 *
	 * @param measurements ������������ڼ���õĲ���Э���б�.
	 */
	FrgCore(const std::vector<Measurement *> &measurements) : _flowingFunctional(nullptr), _flow(nullptr), _measurements(measurements) {};

	/**
	 * @brief ����FrgCore����ɾ���κι����Ĳ���Э��.
	 */
	virtual ~FrgCore()
	{
		while (_measurements.size() > 0)
		{
			delete _measurements.back();
			_measurements.pop_back();
		}
	}

	EffectiveAction *_flowingFunctional; ///< ��ʾ��Ч�����ĵ�ǰ״̬. 
	EffectiveAction *_flow; ///< ����Ч�����ĵ�ǰ״̬��ص� RG ���ı�ʾ. 
	std::vector<Measurement *> _measurements; ///< ���������������������е��õĲ���Э���б�. 
};