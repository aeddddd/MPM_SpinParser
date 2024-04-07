/**
 * @file SU2VertexTwoParticle.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) ģ�͵������Ӷ���ʵ��.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <istream>
#include "lib/ValueBundle.hpp"
#include "lib/Assert.hpp"
#include "FrgCommon.hpp"

/**
 * @brief Ƶ�ʲ�ֵ��Ϣ������. 
 * @details ���������������Ӷ���Ƶ��ά���е��ڴ�ƫ������Ԫ���������б���Щƫ������Ӧ����Ϊ��ֵһ���ֵ�����֧��ֵ. 
 * Ƶ�ʿռ��е��ڴ�ƫ�Ʊ����뾧��λ��ά���е������ڴ�ƫ�ƻ���. 
 * ÿ��֧��ֵ��������һ��Ȩ������. 
 * ÿ��֧��ֵ����+1��-1�ķ��������Լ�����֧��ֵʱ�Ƿ�Ӧִ������վ�㽻������Ϣ���в���. 
 * 
 * @tparam size ���ڲ�ֵ��֧��λ������. 
 */
template <int size> struct SU2VertexTwoParticleAccessBuffer
{
public:
	/**
	 * @brief ����δ��ʼ���� SU2Vertex �����ӷ��ʻ���������. 
	 */
	SU2VertexTwoParticleAccessBuffer() : siteExchange(false) {}

	int frequencyOffsets[size]; ///< ˫���Ӷ���Ƶ��ά���е����Լ���ƫ�ƣ�Ԫ��������.  
	float frequencyWeights[size]; ///< ֧��ֵ��Ȩ������. 
	int signFlag[size]; ///< ֧��ֵ�ķ�������. 
	bool siteExchange; ///< λ�㽻��ָ��. 
};

/**
 * @brief SU(2) ģ�͵������Ӷ���ʵ��.
 */
struct SU2VertexTwoParticle
{
public:
	/**
	 * @brief ��ͬ����ͨ����ö��. 
	 */
	enum struct Symmetry : int
	{
		Spin = 0, ///< �����Ͷ���. 
		Density = 1 ///< �ܶ��Ͷ���. 
	};

	/**
	 * @brief ׼ȷλ��Ƶ��������ϵ�Ƶ��ָʾ��.  
	 */
	enum struct FrequencyChannel
	{
		S, ///< s-channel. 
		T, ///< t-channel. 
		U, ///< u-channel. 
		All, ///< All channels. 
		None ///< No channel. 
	};

	/**
	 * @brief ����һ���µ� SU2Vertex �����Ӷ��󲢽�������Ŀ��ʼ��Ϊ��. 
	 */
	SU2VertexTwoParticle()
	{
		//�������ڴ�ά���д洢���
		_memoryStepLattice = FrgCommon::lattice().size;
		_memoryStepLatticeT = _memoryStepLattice * FrgCommon::frequency().size;

		sizeFrequency = FrgCommon::frequency().size * FrgCommon::frequency().size * (FrgCommon::frequency().size + 1) / 2;
		size = FrgCommon::lattice().size * sizeFrequency;

		//����ͳ�ʼ���ڴ�
		_dataSS = new float[size];
		_dataDD = new float[size];
		memset(_dataSS, 0, sizeof(float) * size);
		memset(_dataDD, 0, sizeof(float) * size);
	}

	/**
	 * @brief ���� SU2Vertex �������Ӷ���. 
	 */
	~SU2VertexTwoParticle()
	{
		delete[] _dataSS;
		delete[] _dataDD;
	}

	/**
	 * @brief �� [0,size) ��Χ��չ��һ�����Ե��������������ж�����Ŀ. 
	 * 
	 * @param[in] iterator ���Ե�����. 
	 * @param[out] i1 ��������. 
	 * @param[out] s ��һƵ�ʲ���. 
	 * @param[out] t �ڶ�Ƶ�ʲ���. 
	 * @param[out] u ����Ƶ�ʲ���. 
	 */
	void expandIterator(int iterator, LatticeIterator &i1, float &s, float &t, float &u) const
	{
		ASSERT(iterator >= 0 && iterator < size);
		ASSERT(&s != &t);
		ASSERT(&t != &u);
		ASSERT(&s != &u);

		int su = iterator / _memoryStepLatticeT;
		iterator = iterator % _memoryStepLatticeT;
		t = FrgCommon::frequency()._data[iterator / _memoryStepLattice];
		i1 = FrgCommon::lattice().fromParametrization(iterator % _memoryStepLattice);

		for (int so = 0; so <= su; ++so)
		{
			for (int uo = 0; uo <= so; ++uo)
			{
				if (su == so * (so + 1) / 2 + uo)
				{
					s = FrgCommon::frequency()._data[so];
					u = FrgCommon::frequency()._data[uo];
					return;
				}
			}
		}
	}

	/**
	 * @brief �� [0,sizeFrequency) ��Χ��չ�����Ե��������������в�����Ƶ��ֵ. 
	 * 
	 * @param[in] iterator ���Ե�����. 
	 * @param[out] s ��һƵ�ʲ���. 
	 * @param[out] t �ڶ�Ƶ�ʲ���. 
	 * @param[out] u ����Ƶ�ʲ���. 
	 */
	void expandIterator(int iterator, float &s, float &t, float &u) const
	{
		ASSERT(iterator >= 0 && iterator < sizeFrequency);
		ASSERT(&s != &t);
		ASSERT(&t != &u);
		ASSERT(&s != &u);

		int su = iterator / FrgCommon::frequency().size;
		t = FrgCommon::frequency()._data[iterator % FrgCommon::frequency().size];

		for (int so = 0; so <= su; ++so)
		{
			for (int uo = 0; uo <= so; ++uo)
			{
				if (su == so * (so + 1) / 2 + uo)
				{
					s = FrgCommon::frequency()._data[so];
					u = FrgCommon::frequency()._data[uo];
					return;
				}
			}
		}
	}

	/**
	 * @brief ͨ�����Ե�����ֱ�ӷ��� [0,size) ��Χ�ڵĶ���ֵ. 
	 * 
	 * @param iterator ���Ե�����. 
	 * @param symmetry ����ͨ��. 
	 * @return float& ����ֵ. 
	 */
	float &getValueRef(const int iterator, const SU2VertexTwoParticle::Symmetry symmetry) const
	{
		if (symmetry == SU2VertexTwoParticle::Symmetry::Spin) return _dataSS[iterator];
		else return _dataDD[iterator];
	}

	/**
	 * @brief �������⾧��λ�á�Ƶ�ʺͶԳ��Դ��Ķ���ֵ. 
	 * 
	 * @param i1 ��һ��������. 
	 * @param i2 �ڶ���������. 
	 * @param s ��һƵ�ʲ���. 
	 * @param t �ڶ�Ƶ�ʲ���. 
	 * @param u ����Ƶ�ʲ���. 
	 * @param symmetry ����ͨ��. 
	 * @param channel Ƶ��ͨ��. 
	 * @return float ����ֵ. 
	 */
	float getValue(LatticeIterator i1, LatticeIterator i2, float s, float t, float u, const SU2VertexTwoParticle::Symmetry symmetry, const SU2VertexTwoParticle::FrequencyChannel channel) const
	{
		//ӳ�䵽��Ƶ������
		if (s < 0 && u < 0)
		{
			s = -s;
			u = -u;
		}
		else
		{
			if (s < 0)
			{
				s = -s;
				LatticeIterator i3 = i1;
				i1 = i2;
				i2 = i3;
			}
			else if (u < 0)
			{
				u = -u;
				LatticeIterator i3 = i1;
				i1 = i2;
				i2 = i3;
			}
		}
		if (t < 0)
		{
			t = -t;
		}

		int siteOffset = FrgCommon::lattice().symmetryTransform(i1, i2);
		if (channel == SU2VertexTwoParticle::FrequencyChannel::S)
		{
			int exactS = FrgCommon::frequency().offset(s);

			int lowerT, upperT;
			float biasT;
			int lowerU, upperU;
			float biasU;

			FrgCommon::frequency().interpolateOffset(t, lowerT, upperT, biasT);
			FrgCommon::frequency().interpolateOffset(u, lowerU, upperU, biasU);

			return (1 - biasU) * (
				(1 - biasT) * (_directAccessMapFrequencyExchange(siteOffset, exactS, lowerT, lowerU, symmetry)) + biasT * (_directAccessMapFrequencyExchange(siteOffset, exactS, upperT, lowerU, symmetry))
				) + biasU * (
				(1 - biasT) * (_directAccessMapFrequencyExchange(siteOffset, exactS, lowerT, upperU, symmetry)) + biasT * (_directAccessMapFrequencyExchange(siteOffset, exactS, upperT, upperU, symmetry))
					);
		}
		else if (channel == SU2VertexTwoParticle::FrequencyChannel::T)
		{
			int exactT = FrgCommon::frequency().offset(t);

			int lowerS, upperS;
			float biasS;
			int lowerU, upperU;
			float biasU;

			FrgCommon::frequency().interpolateOffset(s, lowerS, upperS, biasS);
			FrgCommon::frequency().interpolateOffset(u, lowerU, upperU, biasU);

			return (1 - biasU) * (
				(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, exactT, lowerU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, exactT, lowerU, symmetry))
				) + biasU * (
				(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, exactT, upperU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, exactT, upperU, symmetry))
					);
		}
		else if (channel == SU2VertexTwoParticle::FrequencyChannel::U)
		{
			int exactU = FrgCommon::frequency().offset(u);

			int lowerS, upperS;
			float biasS;
			int lowerT, upperT;
			float biasT;

			FrgCommon::frequency().interpolateOffset(s, lowerS, upperS, biasS);
			FrgCommon::frequency().interpolateOffset(t, lowerT, upperT, biasT);

			return (1 - biasT) * (
				(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, lowerT, exactU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, lowerT, exactU, symmetry))
				) + biasT * (
				(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, upperT, exactU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, upperT, exactU, symmetry))
					);
		}
		else if (channel == SU2VertexTwoParticle::FrequencyChannel::None)
		{
			int lowerS, upperS;
			float biasS;
			int lowerT, upperT;
			float biasT;
			int lowerU, upperU;
			float biasU;

			FrgCommon::frequency().interpolateOffset(s, lowerS, upperS, biasS);
			FrgCommon::frequency().interpolateOffset(t, lowerT, upperT, biasT);
			FrgCommon::frequency().interpolateOffset(u, lowerU, upperU, biasU);

			return
				(1 - biasU) * (
				(1 - biasT) * (
					(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, lowerT, lowerU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, lowerT, lowerU, symmetry))
					) + biasT * (
					(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, upperT, lowerU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, upperT, lowerU, symmetry))
						)
					) + biasU * (
					(1 - biasT) * (
						(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, lowerT, upperU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, lowerT, upperU, symmetry))
						) + biasT * (
						(1 - biasS) * (_directAccessMapFrequencyExchange(siteOffset, lowerS, upperT, upperU, symmetry)) + biasS * (_directAccessMapFrequencyExchange(siteOffset, upperS, upperT, upperU, symmetry))
							)
						);
		}
		else if (channel == SU2VertexTwoParticle::FrequencyChannel::All)
		{
			int exactS = FrgCommon::frequency().offset(s);
			int exactT = FrgCommon::frequency().offset(t);
			int exactU = FrgCommon::frequency().offset(u);
			return _directAccessMapFrequencyExchange(siteOffset, exactS, exactT, exactU, symmetry);
		}
		else
		{
			throw Exception(Exception::Type::ArgumentError, "Invalid interpolation channel");
			return 0.0f;
		}
	}

	/**
	 * @brief ͨ�������ķ��ʻ������������⾧��λ�ô��Ķ���ֵ�ͶԳ���. 
	 * 
	 * @tparam n ���ʻ�������֧��վ�������. 
	 * @param i1 ��һ��������. 
	 * @param i2 �ڶ���������. 
	 * @param symmetry ����ͨ��. 
	 * @param accessBuffer ���ʻ�����. 
	 * @return float ����ֵ. 
	 */
	template <int n> float getValue(const LatticeIterator i1, const LatticeIterator i2, const SU2VertexTwoParticle::Symmetry symmetry, const SU2VertexTwoParticleAccessBuffer<n> &accessBuffer) const
	{
		int siteOffset = (accessBuffer.siteExchange) ? FrgCommon::lattice().symmetryTransform(i2, i1) : FrgCommon::lattice().symmetryTransform(i1, i2);

		float value = 0.0f;
		if (symmetry == SU2VertexTwoParticle::Symmetry::Spin)
		{
			for (int i = 0; i < n; ++i) value += accessBuffer.frequencyWeights[i] * _dataSS[accessBuffer.frequencyOffsets[i] + siteOffset];
		}
		else
		{
			for (int i = 0; i < n; ++i) value += accessBuffer.signFlag[i] * accessBuffer.frequencyWeights[i] * _dataDD[accessBuffer.frequencyOffsets[i] + siteOffset];
		}

		return value;
	}

	/**
	 * @brief ͨ�������ķ��ʻ�����������Գ��Ա��ط��ʶ���ֵ
	 * 
	 * @tparam n ���ʻ�������֧��վ�������. 
	 * @param symmetry ����ͨ��. 
	 * @param accessBuffer ���ʻ�����. 
	 * @return float ����ֵ. 
	 */
	template <int n> float getValueLocal(const SU2VertexTwoParticle::Symmetry symmetry, const SU2VertexTwoParticleAccessBuffer<n> &accessBuffer) const
	{
		float value = 0.0f;
		if (symmetry == SU2VertexTwoParticle::Symmetry::Spin)
		{
			for (int i = 0; i < n; ++i) value += accessBuffer.frequencyWeights[i] * _dataSS[accessBuffer.frequencyOffsets[i]];
		}
		else
		{
			for (int i = 0; i < n; ++i) value += accessBuffer.signFlag[i] * accessBuffer.frequencyWeights[i] * _dataDD[accessBuffer.frequencyOffsets[i]];
		}

		return value;
	}

	/**
	 * @brief ͨ�������ķ��ʻ�����ͬʱ�����о���λ�úͶԳ��Խ������󶥵����. 
	 * 
	 * @tparam n ���ʻ�������֧��վ�������. 
	 * @param[in] accessBuffer ���ʻ�����. 
	 * @param[out] superbundle ����ֵ��. 
	 */
	template <int n> void getValueSuperbundle(const SU2VertexTwoParticleAccessBuffer<n> &accessBuffer, ValueSuperbundle<float, 2> &superbundle) const
	{
		superbundle.reset();
		const LatticeSiteDescriptor *sites = (accessBuffer.siteExchange) ? FrgCommon::lattice().getInvertedSites() : FrgCommon::lattice().getSites();

		for (int i = 0; i < n; ++i)
		{
			float weight = accessBuffer.frequencyWeights[i];
			float signedWeight = accessBuffer.signFlag[i] * accessBuffer.frequencyWeights[i];
			int frequencyOffset = accessBuffer.frequencyOffsets[i];
			int size = FrgCommon::lattice().size;

			for (int j = 0; j < size; ++j)
			{
				superbundle.bundle(0)[j] += weight * _dataSS[frequencyOffset + sites[j].rid];
				superbundle.bundle(1)[j] += signedWeight * _dataDD[frequencyOffset + sites[j].rid];
			}
		}
	}

	/**
	 * @brief Ϊһ��Ƶ�����ɷ��ʻ�����������һ��Ƶ�ʣ���ͨ��ָ����ǡ��λ��Ƶ��������. 
	 * Ƶ��ͨ�������� FrequencyChannel::S, FrequencyChannel::T, FrequencyChannel::U. 
	 * 
	 * @param s ��һƵ�ʲ���. 
	 * @param t �ڶ�Ƶ�ʲ���. 
	 * @param u ����Ƶ�ʲ���. 
	 * @param channel Ƶ��ͨ��. 
	 * @return SU2VertexTwoParticleAccessBuffer<4> ���ʻ�����. 
	 */
	SU2VertexTwoParticleAccessBuffer<4> generateAccessBuffer(float s, float t, float u, const SU2VertexTwoParticle::FrequencyChannel channel) const
	{
		ASSERT(channel == FrequencyChannel::S || channel == FrequencyChannel::T || channel == FrequencyChannel::U);

		SU2VertexTwoParticleAccessBuffer<4> accessBuffer;

		//ӳ�䵽��Ƶ������
		if (s < 0 && u < 0)
		{
			s = -s;
			u = -u;
		}
		else
		{
			if (s < 0)
			{
				s = -s;
				accessBuffer.siteExchange = true;
			}
			else if (u < 0)
			{
				u = -u;
				accessBuffer.siteExchange = true;
			}
		}
		if (t < 0)
		{
			t = -t;
		}

		//��ֵƵ��
		if (channel == SU2VertexTwoParticle::FrequencyChannel::S)
		{
			int exactS = FrgCommon::frequency().offset(s);
			int lowerT, upperT;
			float biasT;
			int lowerU, upperU;
			float biasU;
			FrgCommon::frequency().interpolateOffset(t, lowerT, upperT, biasT);
			FrgCommon::frequency().interpolateOffset(u, lowerU, upperU, biasU);

			accessBuffer.frequencyWeights[0] = (1 - biasU) * (1 - biasT);
			accessBuffer.frequencyOffsets[0] = _generateAccessBufferOffset(exactS, lowerT, lowerU, accessBuffer.signFlag[0]);
			accessBuffer.frequencyWeights[1] = (1 - biasU) * biasT;
			accessBuffer.frequencyOffsets[1] = _generateAccessBufferOffset(exactS, upperT, lowerU, accessBuffer.signFlag[1]);
			accessBuffer.frequencyWeights[2] = biasU * (1 - biasT);
			accessBuffer.frequencyOffsets[2] = _generateAccessBufferOffset(exactS, lowerT, upperU, accessBuffer.signFlag[2]);
			accessBuffer.frequencyWeights[3] = biasU * biasT;
			accessBuffer.frequencyOffsets[3] = _generateAccessBufferOffset(exactS, upperT, upperU, accessBuffer.signFlag[3]);
		}
		else if (channel == SU2VertexTwoParticle::FrequencyChannel::T)
		{
			int exactT = FrgCommon::frequency().offset(t);
			int lowerS, upperS;
			float biasS;
			int lowerU, upperU;
			float biasU;
			FrgCommon::frequency().interpolateOffset(s, lowerS, upperS, biasS);
			FrgCommon::frequency().interpolateOffset(u, lowerU, upperU, biasU);

			accessBuffer.frequencyWeights[0] = (1 - biasU) * (1 - biasS);
			accessBuffer.frequencyOffsets[0] = _generateAccessBufferOffset(lowerS, exactT, lowerU, accessBuffer.signFlag[0]);
			accessBuffer.frequencyWeights[1] = (1 - biasU) * biasS;
			accessBuffer.frequencyOffsets[1] = _generateAccessBufferOffset(upperS, exactT, lowerU, accessBuffer.signFlag[1]);
			accessBuffer.frequencyWeights[2] = biasU * (1 - biasS);
			accessBuffer.frequencyOffsets[2] = _generateAccessBufferOffset(lowerS, exactT, upperU, accessBuffer.signFlag[2]);
			accessBuffer.frequencyWeights[3] = biasU * biasS;
			accessBuffer.frequencyOffsets[3] = _generateAccessBufferOffset(upperS, exactT, upperU, accessBuffer.signFlag[3]);

		}
		else if (channel == SU2VertexTwoParticle::FrequencyChannel::U)
		{
			int exactU = FrgCommon::frequency().offset(u);
			int lowerS, upperS;
			float biasS;
			int lowerT, upperT;
			float biasT;
			FrgCommon::frequency().interpolateOffset(s, lowerS, upperS, biasS);
			FrgCommon::frequency().interpolateOffset(t, lowerT, upperT, biasT);

			accessBuffer.frequencyWeights[0] = (1 - biasT) * (1 - biasS);
			accessBuffer.frequencyOffsets[0] = _generateAccessBufferOffset(lowerS, lowerT, exactU, accessBuffer.signFlag[0]);
			accessBuffer.frequencyWeights[1] = (1 - biasT) * biasS;
			accessBuffer.frequencyOffsets[1] = _generateAccessBufferOffset(upperS, lowerT, exactU, accessBuffer.signFlag[1]);
			accessBuffer.frequencyWeights[2] = biasT * (1 - biasS);
			accessBuffer.frequencyOffsets[2] = _generateAccessBufferOffset(lowerS, upperT, exactU, accessBuffer.signFlag[2]);
			accessBuffer.frequencyWeights[3] = biasT * biasS;
			accessBuffer.frequencyOffsets[3] = _generateAccessBufferOffset(upperS, upperT, exactU, accessBuffer.signFlag[3]);
		}

		return accessBuffer;
	}

	/**
	 * @brief Ϊ����һ��Ƶ�����ɷ��ʻ�����.  
	 * 
	 * @param s ��һƵ�ʲ���. 
	 * @param t �ڶ�Ƶ�ʲ���. 
	 * @param u ����Ƶ�ʲ���. 
	 * @return SU2VertexTwoParticleAccessBuffer<8> ���ʻ�����. 
	 */
	SU2VertexTwoParticleAccessBuffer<8> generateAccessBuffer(float s, float t, float u) const
	{
		SU2VertexTwoParticleAccessBuffer<8> accessBuffer;

		//ӳ�䵽��Ƶ������
		if (s < 0 && u < 0)
		{
			s = -s;
			u = -u;
		}
		else
		{
			if (s < 0)
			{
				s = -s;
				accessBuffer.siteExchange = true;
			}
			else if (u < 0)
			{
				u = -u;
				accessBuffer.siteExchange = true;
			}
		}
		if (t < 0)
		{
			t = -t;
		}

		int lowerS, upperS;
		float biasS;
		int lowerT, upperT;
		float biasT;
		int lowerU, upperU;
		float biasU;

		FrgCommon::frequency().interpolateOffset(s, lowerS, upperS, biasS);
		FrgCommon::frequency().interpolateOffset(t, lowerT, upperT, biasT);
		FrgCommon::frequency().interpolateOffset(u, lowerU, upperU, biasU);

		accessBuffer.frequencyWeights[0] = (1 - biasT) * (1 - biasS) * (1 - biasU);
		accessBuffer.frequencyOffsets[0] = _generateAccessBufferOffset(lowerS, lowerT, lowerU, accessBuffer.signFlag[0]);
		accessBuffer.frequencyWeights[1] = (1 - biasT) * biasS * (1 - biasU);
		accessBuffer.frequencyOffsets[1] = _generateAccessBufferOffset(upperS, lowerT, lowerU, accessBuffer.signFlag[1]);
		accessBuffer.frequencyWeights[2] = biasT * (1 - biasS) * (1 - biasU);
		accessBuffer.frequencyOffsets[2] = _generateAccessBufferOffset(lowerS, upperT, lowerU, accessBuffer.signFlag[2]);
		accessBuffer.frequencyWeights[3] = biasT * biasS * (1 - biasU);
		accessBuffer.frequencyOffsets[3] = _generateAccessBufferOffset(upperS, upperT, lowerU, accessBuffer.signFlag[3]);
		accessBuffer.frequencyWeights[4] = (1 - biasT) * (1 - biasS) * biasU;
		accessBuffer.frequencyOffsets[4] = _generateAccessBufferOffset(lowerS, lowerT, upperU, accessBuffer.signFlag[4]);
		accessBuffer.frequencyWeights[5] = (1 - biasT) * biasS * biasU;
		accessBuffer.frequencyOffsets[5] = _generateAccessBufferOffset(upperS, lowerT, upperU, accessBuffer.signFlag[5]);
		accessBuffer.frequencyWeights[6] = biasT * (1 - biasS) * biasU;
		accessBuffer.frequencyOffsets[6] = _generateAccessBufferOffset(lowerS, upperT, upperU, accessBuffer.signFlag[6]);
		accessBuffer.frequencyWeights[7] = biasT * biasS * biasU;
		accessBuffer.frequencyOffsets[7] = _generateAccessBufferOffset(upperS, upperT, upperU, accessBuffer.signFlag[7]);

		return accessBuffer;
	}

	/**
	 * @brief ͨ������Ƶ�ʺ�վ��ƫ����ֱ�ӷ��ʶ��㣬���� s Offset ����С�� u Offset. 
	 * 
	 * @param siteOffset ����λ��ƫ�ƣ�Ԫ��������. 
	 * @param sOffset ��һƵ��ƫ�ƣ�Ԫ��������. 
	 * @param tOffset �ڶ�Ƶ��ƫ�ƣ�Ԫ��������. 
	 * @param uOffset ����Ƶ��ƫ�ƣ�Ԫ��������. 
	 * @param symmetry ����ͨ��. 
	 * @return float ����ֵ. 
	 */
	float _directAccessMapFrequencyExchange(const int siteOffset, const int sOffset, const int tOffset, const int uOffset, const SU2VertexTwoParticle::Symmetry symmetry) const
	{
		ASSERT(siteOffset >= 0 && siteOffset < FrgCommon::lattice().size);
		ASSERT(sOffset >= 0 && sOffset < FrgCommon::frequency().size);
		ASSERT(tOffset >= 0 && tOffset < FrgCommon::frequency().size);
		ASSERT(uOffset >= 0 && uOffset < FrgCommon::frequency().size);


		if (sOffset >= uOffset) return _directAccess(siteOffset, sOffset, tOffset, uOffset, symmetry);
		else
		{
			if (symmetry == SU2VertexTwoParticle::Symmetry::Spin) return _directAccess(siteOffset, uOffset, tOffset, sOffset, symmetry);
			else return -_directAccess(siteOffset, uOffset, tOffset, sOffset, symmetry);
		}
	}

	/**
	 * @brief ͨ������Ƶ�ʺ�վ��ƫ��ֱ�ӷ��ʶ��㣬���� s Offset >= u Offset. 
	 * 
	 * @param siteOffset ����λ��ƫ�ƣ�Ԫ��������. 
	 * @param sOffset ��һƵ��ƫ�ƣ�Ԫ��������. 
	 * @param tOffset �ڶ�Ƶ��ƫ�ƣ�Ԫ��������. 
	 * @param uOffset ����Ƶ��ƫ�ƣ�Ԫ��������. 
	 * @param symmetry ����ͨ��. 
	 * @return float ����ֵ. 
	 */
	float _directAccess(const int siteOffset, const int sOffset, const int tOffset, const int uOffset, const SU2VertexTwoParticle::Symmetry symmetry) const
	{
		ASSERT(siteOffset >= 0 && siteOffset < FrgCommon::lattice().size);
		ASSERT(sOffset >= 0 && sOffset < FrgCommon::frequency().size);
		ASSERT(tOffset >= 0 && tOffset < FrgCommon::frequency().size);
		ASSERT(uOffset >= 0 && uOffset < FrgCommon::frequency().size);
		ASSERT(sOffset >= uOffset);

		if (symmetry == SU2VertexTwoParticle::Symmetry::Spin) return _dataSS[_memoryStepLatticeT * (sOffset * (sOffset + 1) / 2 + uOffset) + _memoryStepLattice * tOffset + siteOffset];
		else return _dataDD[_memoryStepLatticeT * (sOffset * (sOffset + 1) / 2 + uOffset) + _memoryStepLattice * tOffset + siteOffset];
	}

	/**
	 * @brief ���ݸ�����Ƶ��ƫ�Ƽ������ڴ�ƫ�ƣ�Ԫ�������������� s Offset ����С�� u Offset. 
	 * 
	 * @param[in] sOffset ��һƵ��ƫ�ƣ�Ԫ��������. 
	 * @param[in] tOffset �ڶ�Ƶ��ƫ�ƣ�Ԫ��������. 
	 * @param[in] uOffset ����Ƶ��ƫ�ƣ�Ԫ��������. 
	 * @param[out] signFlag Ҫ�洢�ڷ��ʻ������еķ��ű�־. 
	 * @return int �ڴ�ƫ������Ԫ��������. 
	 */
	int _generateAccessBufferOffset(const int sOffset, const int tOffset, const int uOffset, int &signFlag) const
	{
		ASSERT(sOffset >= 0 && sOffset < FrgCommon::frequency().size);
		ASSERT(tOffset >= 0 && tOffset < FrgCommon::frequency().size);
		ASSERT(uOffset >= 0 && uOffset < FrgCommon::frequency().size);

		if (sOffset < uOffset)
		{
			signFlag = -1;
			return _memoryStepLatticeT * (uOffset * (uOffset + 1) / 2 + sOffset) + tOffset * _memoryStepLattice;
		}
		else
		{
			signFlag = 1;
			return _memoryStepLatticeT * (sOffset * (sOffset + 1) / 2 + uOffset) + tOffset * _memoryStepLattice;
		}
	}

	int size; ///< ÿ������ͨ���Ķ����С��Ԫ��������. 
	int sizeFrequency; ///< Ƶ���ӿռ���ÿ������ͨ���Ķ����С��Ԫ��������. 

	float *_dataSS; ///< ���������ͨ��. 
	float *_dataDD; ///< ������ܶ�ͨ��. 
	int _memoryStepLatticeT; ///< ��� 2 ά�е��ڴ沽�����. 
	int _memoryStepLattice; ///< ���һά���ڴ沽�����. 
};