/**
 * @file FrgCoreFactory.hpp
 * @author Finn Lasse Buessen
 * @brief Factory function to create a new FrgCore. 
 * @details �����Զ��� FrgCore ��ʵ�֣�Ӧ����չ FrgCore Factory::new FrgCore ���̣��Ա�����������ʶ���Զ��� FrgCore �ı�ʶ������Ӧ�ش�����ʵ��. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <string>
#include <vector>
#include <map>
#include "FrgCore.hpp"

namespace FrgCoreFactory
{
	/**
	 * @brief ����Э��ĳ���淶. 
	 */
	struct MeasurementSpecification
	{
		std::string identifier; ///< �������͵��ַ�����ʽ��ʶ�����������ļ�����ָ��. 
		std::string output; ///< �������������ļ�. 
		float minCutoff; ///< Ҫ���õ���С��ֵֹ. 
		float maxCutoff; ///< Ҫ���õ�����ֵֹ. 
		bool defer; ///< �Ƴٱ�־���������Ϊ true������ں���׶ε��ò���. 
		std::vector<std::pair<std::string, std::string>> options; ///< �����ļ���ָ�����ַ�����ʽЭ�����η�. 
	};

	/**
	 * @brief Ϊ�����ĶԳƱ�ʶ��������ģ�ͺͲ���Э�鴴���µ�FrgCore. 
	 * 
	 * @param identifier �ַ�����ʽ�ĶԳƱ�ʶ�����������ļ���ָ��. 
	 * @param model ����ȷ��������ʼ����������ģ��. 
	 * @param measurements  �ں���ִ���ڼ���õĲ���Э��. 
	 * @param options �����ļ���ָ�����ַ�����ʽ�������η�. 
	 * @return FrgCore* ָ����FrgCore�����ָ��. 
	 */
	FrgCore *newFrgCore(const std::string &identifier, const SpinModel &model, const std::vector<MeasurementSpecification> &measurements, const std::map<std::string, std::string> &options);
}