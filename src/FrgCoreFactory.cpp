/**
 * @file FrgCoreFactory.cpp
 * @author Finn Lasse Buessen
 * @brief Factory function to create a new FrgCore. 
 * @details 对于自定义 Frg Core 的实现,应该扩展 FrgCoreFactory::new FrgCore 例程,以便它可以重新识别自定义 FrgCore 的标识符并相应地创建其实例. 
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
	//创建测量对象
	std::vector<Measurement *> measurementObjects;

	for (auto specification : measurements)
	{
		//相关测量
		if (specification.identifier == "correlation")
		{
			Measurement *m = nullptr;
			if (identifier == "SU2") m = new SU2MeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
			else if (identifier == "XYZ") m = new XYZMeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
			else if (identifier == "TRI") m = new TRIMeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
//		    else if (identifier == "TSV") m = new TSVMeasurementCorrelation(specification.output, specification.minCutoff, specification.maxCutoff, specification.defer);
			else throw Exception(Exception::Type::InitializationError, "Measurement [correlation]: 未知模型对称性 '" + identifier + "'.");

			Log::log << Log::LogLevel::Info << "Added measurement [correlation]." << Log::endl;
			measurementObjects.push_back(m);
		}
		else throw Exception(Exception::Type::InitializationError, "Measurement: 未知测量类型 '" + identifier + "'.");

	}

	if (identifier == "SU2") return new SU2FrgCore(model, measurementObjects, options);
	else if (identifier == "XYZ") return new XYZFrgCore(model, measurementObjects, options);
	else if (identifier == "TRI") return new TRIFrgCore(model, measurementObjects, options);
//	else if (identifier == "TSV") return new TSVFrgCore(model, measurementObjects, options);
	else throw Exception(Exception::Type::ArgumentError, "Spin model identifier '" + identifier + "' does not exist.");
}