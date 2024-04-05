/**
 * @file TRIEffectiveAction.hpp
 * @author Finn Lasse Buessen
 * @brief Implementation of a flowing effective action for time reversal invariant models.  
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "lib/Exception.hpp"
#include "EffectiveAction.hpp"
#include "TRIFrgCore.hpp"
#include "TRIVertexSingleParticle.hpp"
#include "TRIVertexTwoParticle.hpp"

/**
 * @brief ʱ�䷴ת����ģ�͵�������Ч������ʵ��.  
 */
struct TRIEffectiveAction : public EffectiveAction
{
public:
	/**
	 * @brief ����һ���µ� TRIEffective Action ����. 
	 */
	TRIEffectiveAction()
	{
		vertexSingleParticle = new TRIVertexSingleParticle;
		vertexTwoParticle = new TRIVertexTwoParticle;
	}

	/**
	 * @brief Ϊ����������ģ�͹���һ���µ���Ч�������ڸ����Ľ�ֵֹ����ʼ��ֵ. 
	 * 
	 * @param cutoff ��ʼ���Ľ�ֵֹ. 
	 * @param spinModel ����ģ�ͳ�ʼ��. 
	 * @param core ���ô�������� FRGCore. 
	 */
	TRIEffectiveAction(const float cutoff, const SpinModel &spinModel, const TRIFrgCore *core)
	{
		vertexSingleParticle = new TRIVertexSingleParticle;
		vertexTwoParticle = new TRIVertexTwoParticle;

		//set initial value
		this->cutoff = cutoff;

		for (int linearIterator = 0; linearIterator < vertexTwoParticle->size; ++linearIterator)
		{
			float s, t, u;
			LatticeIterator i1;
			SpinComponent s1, s2;
			vertexTwoParticle->expandIterator(linearIterator, i1, s, t, u, s1, s2);

			//skip initial conditions for density and spin/density interactions
			if (static_cast<int>(s1) == 3 || static_cast<int>(s2) == 3) continue;
			//set initial conditions for spin/spin interactions
			for (auto i : spinModel.interactions)
			{
				if (i.first == i1)
				{
					vertexTwoParticle->getValueRef(linearIterator) += 0.25f * i.second.interactionStrength[static_cast<int>(s1)][static_cast<int>(s2)] / core->normalization;
				}
			}
		}
	}

	/**
	 * @brief Destroy the TRIEffectiveAction object. 
	 */
	~TRIEffectiveAction()
	{
		delete vertexSingleParticle;
		delete vertexTwoParticle;
	}

	/**
 * @brief Write checkpoint file.
 *
 * @param dataFilePath Checkpoint file path.
 * @param append Append flag.
 *
 * @return int Checkpoint identifier of the checkpoint written.
 */
	int writeCheckpoint(const std::string &dataFilePath, const bool append = false) const override
	{
		H5Eset_auto(H5E_DEFAULT, NULL, NULL);
		hid_t file;

		//open or create file
		if (append && H5Fis_hdf5(dataFilePath.c_str()) > 0) file = H5Fopen(dataFilePath.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
		else file = H5Fcreate(dataFilePath.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
		if (file < 0) throw Exception(Exception::Type::IOError, "Could not open data file for writing");

		//determine new checkpoint id
		hsize_t numObjects;
		H5Gget_num_objs(file, &numObjects);
		int checkpointId = 0;
		for (int i = 0; i < int(numObjects); ++i)
		{
			if (H5Gget_objtype_by_idx(file, i) == H5G_GROUP)
			{
				++checkpointId;

				const int groupNameMaxLength = 32;
				char groupName[groupNameMaxLength];
				H5Gget_objname_by_idx(file, i, groupName, groupNameMaxLength);

				hid_t group = H5Gopen(file, groupName, H5P_DEFAULT);
				hid_t attr = H5Aopen(group, "cutoff", H5P_DEFAULT);
				float c;
				H5Aread(attr, H5T_NATIVE_FLOAT, &c);
				H5Aclose(attr);
				H5Gclose(group);

				if (c == cutoff)
				{
					Log::log << Log::LogLevel::Warning << "Found existing checkpoint at cutoff " + std::to_string(cutoff) + ". Skipping checkpoint." << Log::endl;
					H5Fclose(file);
					return -1;
				}
			}
		}
		std::string checkpointName = "checkpoint_" + std::to_string(checkpointId);

		//create checkpoint group
		hid_t group = H5Gcreate(file, checkpointName.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		hsize_t attrSpaceSize[1] = { 1 };
		const int attrSpaceDim = 1;
		hid_t attrSpace = H5Screate_simple(attrSpaceDim, attrSpaceSize, NULL);
		hid_t attr = H5Acreate(group, "cutoff", H5T_NATIVE_FLOAT, attrSpace, H5P_DEFAULT, H5P_DEFAULT);
		H5Awrite(attr, H5T_NATIVE_FLOAT, &cutoff);
		H5Aclose(attr);
		H5Sclose(attrSpace);

		//write vertex data
		auto writeCheckpointDataset = [&group](const std::string &identifier, const int size, const float *data)
		{
			const int dataSpaceDim = 1;
			const hsize_t dataSpaceSize[1] = { (hsize_t)size };
			hid_t dataSpace = H5Screate_simple(dataSpaceDim, dataSpaceSize, NULL);
			hid_t dataset = H5Dcreate(group, identifier.c_str(), H5T_NATIVE_FLOAT, dataSpace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
			H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
			H5Dclose(dataset);
			H5Sclose(dataSpace);
		};
		writeCheckpointDataset("cutoff", 1, &cutoff);
		writeCheckpointDataset("v2", vertexSingleParticle->size, vertexSingleParticle->_data);
		writeCheckpointDataset("v4", vertexTwoParticle->size, vertexTwoParticle->_data);

		//clean up and return
		H5Gclose(group);
		H5Fclose(file);
		return checkpointId;
	}

	/**
 * @brief Read checkpoint from file.
 *
 * @param dataFilePath Checkpoint file path.
 * @param checkpointId Identifier of the checkpoint to read.
 * @return bool Returns true if the checkpoint was read successfully, false otherwise.
 */
	bool readCheckpoint(const std::string &dataFilePath, const int checkpointId) override
	{
		H5Eset_auto(H5E_DEFAULT, NULL, NULL);

		//open file
		hid_t file = H5Fopen(dataFilePath.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
		if (file < 0) throw Exception(Exception::Type::IOError, "Could not open data file for reading");

		//find desired dataset name
		std::string checkpointName;
		if (checkpointId >= 0) checkpointName = "checkpoint_" + std::to_string(checkpointId);
		else
		{
			hsize_t numObjects;
			H5Gget_num_objs(file, &numObjects);
			for (int i = int(numObjects) - 1; i >= 0; --i)
			{
				if (H5Gget_objtype_by_idx(file, i) == H5G_GROUP)
				{
					checkpointName = "checkpoint_" + std::to_string(i);
					break;
				}
			}

		}
		hid_t group = H5Gopen(file, checkpointName.c_str(), H5P_DEFAULT);
		if (group < 0) return false;

		//read dataset
		auto readDataset = [&group](const std::string &name, float *data)->bool
		{
			hid_t dataset = H5Dopen(group, name.c_str(), H5P_DEFAULT);
			if (dataset < 0) return false;
			H5Dread(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
			H5Dclose(dataset);
			return true;
		};
		if (!readDataset("cutoff", &cutoff)) return false;
		if (!readDataset("v2", vertexSingleParticle->_data)) return false;
		if (!readDataset("v4", vertexTwoParticle->_data)) return false;

		//clean up and return
		H5Gclose(group);
		H5Fclose(file);
		return true;
	}

	/**
	 * @brief Indicate whether the vertex has diverged to NaN. 
	 *
	 * @return bool Return true if the vertex has diverged, otherwise return false. 
	 */
	bool isDiverged() const override
	{
		for (int i = 0; i < vertexSingleParticle->size; ++i)
		{
			if (std::isnan(vertexSingleParticle->getValueRef(i))) return true;
		}

		for (int i = 0; i < vertexTwoParticle->size; ++i)
		{
			if (std::isnan(vertexTwoParticle->getValueRef(i))) return true;
		}

		return false;
	}

	TRIVertexSingleParticle *vertexSingleParticle; ///< Single-particle vertex data. 
	TRIVertexTwoParticle *vertexTwoParticle; ///< Two-particle vertex data. 
};