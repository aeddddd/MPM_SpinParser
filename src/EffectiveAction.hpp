/**
 * @file EffectiveAction.hpp
 * @author Finn Lasse Buessen
 * @brief Virtual implementation of a flowing effective action. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <hdf5.h>
#include "lib/Log.hpp"
#include "lib/Exception.hpp"

/**
 * @brief ����ʵʩ������Ч���ж�. 
 * @details ����ʵ��Ӧ�ø�����Ҫʵ�����ݽṹ�����������㶥����ĵ㶥����Ϣ. 
 * �ö����ṩ�� FrgCore ʵ�ֺ� Spin Parser ���ʵ�����ӿ�. 
 */
struct EffectiveAction
{
public:
	/**
	 * @brief ����һ���µ�Effective Action����.
	 */
	EffectiveAction() : cutoff(0.0f) {};

	/**
	 * @brief ������������.
	 */
	virtual ~EffectiveAction() {};

	/**
	 * @brief �������ڲ�����д��ָ���ļ�·���ļ���,������д��ļ���ı�ʶ��.�����ʶ����һ���Ǹ�����,ö������ļ��е����м������ݼ�,���㿪ʼ.
	 * 
	 * @param dataFilePath �����ļ�·��. 
	 * @param append �������Ϊ false,�򸲸����м���.����,��������ھ�����ͬ��ֵֹ����ǰ����,�򸽼Ӽ���.���������ͬ��ֵֹ�ļ����Ѵ���,��ִ���κβ���.
	 *
	 * @return int д��ļ���ı�ʶ��.���д����̱�����,�򷵻�-1. 
	 */
	virtual int writeCheckpoint(const std::string &dataFilePath, const bool append = false) const = 0;

	/**
	 * @brief ��ָ���ļ�·��������ָ�������ʶ���ļ����ȡ�ڲ�����. 
	 * ��������ʶ������Ϊ-1,���ȡ����ļ���.
	 * 
	 * @param datafilePath �����ļ�·��. 
	 * @param checkpointId Ҫ��ȡ�ļ���ı�ʶ��. 
	 * @return bool ��������ȡ�ɹ��򷵻� true�����򷵻� false. 
	 */
	virtual bool readCheckpoint(const std::string &datafilePath, const int checkpointId = -1) = 0;

	/**
	 * @brief ָʾ�����Ƿ��Ѿ���ɢ�� NaN. 
	 *
	 * @return bool ��������ѷ�ɢ,�򷵻� true,���򷵻� false. 
	 */
	virtual bool isDiverged() const = 0;

	float cutoff; ///< RG ��ֵֹ. 
};