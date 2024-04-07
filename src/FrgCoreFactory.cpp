/**
 * @file FrgCoreFactory.cpp
 * @author Finn Lasse Buessen
 * @brief Factory function to create a new FrgCore. 
 * @details �����Զ��� Frg Core ��ʵ��,Ӧ����չ FrgCoreFactory::new FrgCore ����,�Ա�����������ʶ���Զ��� FrgCore �ı�ʶ������Ӧ�ش�����ʵ��. 
 * 
 * @copyright Copyright (c) 2020
 */

#include "lib/Exception.hpp"
#include "lib/InputParser.hpp"
#include "FrgCoreFactory.hpp"

//SU2
#include "SU2/SU2FrgCore.hpp"
#include "SU2/SU2MeasurementCorrelation.hpp"
//XYZ
#include "XYZ/XYZFrgCore.hpp"
#include "XYZ/XYZMeasurementCorrelation.hpp"
//TRI
#include "TRI/TRIFrgCore.hpp"
#include "TRI/TRIMeasurementCorrelation.hpp"


FrgCore *FrgCoreFactory::newFrgCore(const std::string &identifier, const SpinModel &model, const std::vector<MeasurementSpecification> &measurements, const std::map<std::string, std::string> &options)
{
	//������������
	std::vector<Measurement *> measurementObjects;

	for (auto specification : measurements)
	{
		//��ز���
		if (specification.identifier == "correlation")
		{
			Measurement *m = nullptr;
			if (identifier == "SU2") m = new SU2MeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
			else if (identifier == "XYZ") m = new XYZMeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
			else if (identifier == "TRI") m = new TRIMeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
//		    else if (identifier == "TSV") m = new TSVMeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
			else throw Exception(Exception::Type::InitializationError, "Measurement [correlation]: δ֪ģ�ͶԳ��� '" + identifier + "'.");

			Log::log << Log::LogLevel::Info << "Added measurement [correlation]." << Log::endl;
			measurementObjects.push_back(m);
		}
		else throw Exception(Exception::Type::InitializationError, "Measurement: δ֪�������� '" + identifier + "'.");

	}

	if (identifier == "SU2") return new SU2FrgCore(model, measurementObjects, options);
	else if (identifier == "XYZ") return new XYZFrgCore(model, measurementObjects, options);
	else if (identifier == "TRI") return new TRIFrgCore(model, measurementObjects, options);
//	else if (identifier == "TSV") return new TSVFrgCore(model, measurementObjects, options);
	else throw Exception(Exception::Type::ArgumentError, "Spin model identifier '" + identifier + "' does not exist.");
}