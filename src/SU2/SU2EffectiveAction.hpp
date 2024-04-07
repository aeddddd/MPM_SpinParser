/**
 * @file SU2EffectiveAction.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) ģ�͵�������Ч������ʵ��.  
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include "lib/Exception.hpp"
#include "EffectiveAction.hpp"
#include "SU2FrgCore.hpp"
#include "SU2VertexSingleParticle.hpp"
#include "SU2VertexTwoParticle.hpp"

/**
 * @brief Ϊ SU(2) ģ��ʵʩ������Ч���ж�. 
 */
struct SU2EffectiveAction : public EffectiveAction
{
public:
	/**
	 * @brief ����һ���µ� SU2Effective Action ����. 
	 */
	SU2EffectiveAction()
	{
		vertexSingleParticle = new SU2VertexSingleParticle;
		vertexTwoParticle = new SU2VertexTwoParticle;
	}

	/**
	 * @brief Ϊ����������ģ�͹���һ���µ���Ч�������ڸ����Ľ�ֵֹ����ʼ��ֵ. 
	 * 
	 * @param cutoff ��ʼ���Ľ�ֵֹ. 
	 * @param spinModel ����ģ�ͳ�ʼ��. 
	 * @param core ���ô�������� FRGCore. 
	 */
	SU2EffectiveAction(const float cutoff, const SpinModel &spinModel, const SU2FrgCore *core)
	{
		vertexSingleParticle = new SU2VertexSingleParticle;
		vertexTwoParticle = new SU2VertexTwoParticle;

		//���ó�ʼֵ
		this->cutoff = cutoff;

		for (int linearIterator = 0; linearIterator < vertexTwoParticle->size; ++linearIterator)
		{
			float s, t, u;
			LatticeIterator i1;
			vertexTwoParticle->expandIterator(linearIterator, i1, s, t, u);

			for (auto i : spinModel.interactions)
			{
				if (i.first == i1)
				{
					vertexTwoParticle->getValueRef(linearIterator, SU2VertexTwoParticle::Symmetry::Spin) += i.second.interactionStrength[0][0] / core->normalization;
				}
			}
		}
	}

	/**
	 * @brief ���� SU2Effective Action ����. 
	 */
	~SU2EffectiveAction()
	{
		delete vertexSingleParticle;
		delete vertexTwoParticle;
	}

	/**
	 * @brief д������ļ�. 
	 * 
	 * @param dataFilePath �����ļ�·��. 
	 * @param append ���ӱ�־. 
	 *
	 * @return int д��ļ���ļ����ʶ��.
	 */
	int writeCheckpoint(const std::string &dataFilePath, const bool append = false) const override
	{
		H5Eset_auto(H5E_DEFAULT, NULL, NULL);
		hid_t file;

		//�򿪻򴴽��ļ�
		if (append && H5Fis_hdf5(dataFilePath.c_str()) > 0) file = H5Fopen(dataFilePath.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
		else file = H5Fcreate(dataFilePath.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
		if (file < 0) throw Exception(Exception::Type::IOError, "Could not open data file for writing");

		//ȷ���µļ��� ID
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

		//����������
		hid_t group = H5Gcreate(file, checkpointName.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		hsize_t attrSpaceSize[1] = { 1 };
		const int attrSpaceDim = 1;
		hid_t attrSpace = H5Screate_simple(attrSpaceDim, attrSpaceSize, NULL);
		hid_t attr = H5Acreate(group, "cutoff", H5T_NATIVE_FLOAT, attrSpace, H5P_DEFAULT, H5P_DEFAULT);
		H5Awrite(attr, H5T_NATIVE_FLOAT, &cutoff);
		H5Aclose(attr);
		H5Sclose(attrSpace);

		//д�붥������
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
		writeCheckpointDataset("v4dd", vertexTwoParticle->size, vertexTwoParticle->_dataDD);
		writeCheckpointDataset("v4ss", vertexTwoParticle->size, vertexTwoParticle->_dataSS);

		//��������
		H5Gclose(group);
		H5Fclose(file);
		return checkpointId;
	}

	/**
	 * @brief ���ļ��ж�ȡ����. 
	 * 
	 * @param dataFilePath �����ļ�·��. 
	 * @param checkpointId Ҫ��ȡ�ļ���ı�ʶ��. 
	 * @return bool ��������ȡ�ɹ��򷵻� true�����򷵻� false. 
	 */
	bool readCheckpoint(const std::string &dataFilePath, const int checkpointId) override
	{
		H5Eset_auto(H5E_DEFAULT, NULL, NULL);

		//���ļ�
		hid_t file = H5Fopen(dataFilePath.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
		if (file < 0) throw Exception(Exception::Type::IOError, "Could not open data file for reading");

		//�ҵ���������ݼ�����
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

		//��ȡ���ݼ�
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
		if (!readDataset("v4dd", vertexTwoParticle->_dataDD)) return false;
		if (!readDataset("v4ss", vertexTwoParticle->_dataSS)) return false;

		//��������
		H5Gclose(group);
		H5Fclose(file);
		return true;
	}

	/**
	 * @brief ָʾ�����Ƿ��Ѿ���ɢ�� Na N. 
	 *
	 * @return bool ��������ѷ�ɢ���򷵻� true�����򷵻� false. 
	 */
	bool isDiverged() const override
	{
		for (int i = 0; i < vertexSingleParticle->size; ++i)
		{
			if (std::isnan(vertexSingleParticle->getValueRef(i))) return true;
		}

		for (int i = 0; i < vertexTwoParticle->size; ++i)
		{
			if (std::isnan(vertexTwoParticle->getValueRef(i, SU2VertexTwoParticle::Symmetry::Spin))) return true;
		}

		for (int i = 0; i < vertexTwoParticle->size; ++i)
		{
			if (std::isnan(vertexTwoParticle->getValueRef(i, SU2VertexTwoParticle::Symmetry::Density))) return true;
		}

		return false;
	}

	SU2VertexSingleParticle *vertexSingleParticle; ///< �����Ӷ�������. 
	SU2VertexTwoParticle *vertexTwoParticle; ///< �����Ӷ�������. 
};