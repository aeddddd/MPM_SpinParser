/**
 * @file SU2VertexTwoParticle.hpp
 * @author Finn Lasse Buessen
 * @brief SU(2) 模型的两粒子顶点实现.
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <istream>
#include "lib/ValueBundle.hpp"
#include "lib/Assert.hpp"
#include "FrgCommon.hpp"

/**
 * @brief 频率插值信息缓冲区. 
 * @details 缓冲区包含两粒子顶点频率维度中的内存偏移量（元素数量）列表，这些偏移量对应于作为插值一部分的所有支持值. 
 * 频率空间中的内存偏移必须与晶格位点维度中的线性内存偏移互补. 
 * 每个支持值都分配有一个权重因子. 
 * 每个支持值还由+1或-1的符号因子以及访问支持值时是否应执行网格站点交换的信息进行补充. 
 * 
 * @tparam size 用于插值的支持位点数量. 
 */
template <int size> struct SU2VertexTwoParticleAccessBuffer
{
public:
	/**
	 * @brief 构造未初始化的 SU2Vertex 二粒子访问缓冲区对象. 
	 */
	SU2VertexTwoParticleAccessBuffer() : siteExchange(false) {}

	int frequencyOffsets[size]; ///< 双粒子顶点频率维度中的线性记忆偏移（元素数量）.  
	float frequencyWeights[size]; ///< 支持值的权重因子. 
	int signFlag[size]; ///< 支持值的符号因子. 
	bool siteExchange; ///< 位点交换指标. 
};

/**
 * @brief SU(2) 模型的两粒子顶点实现.
 */
struct SU2VertexTwoParticle
{
public:
	/**
	 * @brief 不同顶点通道的枚举. 
	 */
	enum struct Symmetry : int
	{
		Spin = 0, ///< 自旋型顶点. 
		Density = 1 ///< 密度型顶点. 
	};

	/**
	 * @brief 准确位于频率网格点上的频道指示器.  
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
	 * @brief 构造一个新的 SU2Vertex 二粒子对象并将所有条目初始化为零. 
	 */
	SU2VertexTwoParticle()
	{
		//在所有内存维度中存储宽度
		_memoryStepLattice = FrgCommon::lattice().size;
		_memoryStepLatticeT = _memoryStepLattice * FrgCommon::frequency().size;

		sizeFrequency = FrgCommon::frequency().size * FrgCommon::frequency().size * (FrgCommon::frequency().size + 1) / 2;
		size = FrgCommon::lattice().size * sizeFrequency;

		//分配和初始化内存
		_dataSS = new float[size];
		_dataDD = new float[size];
		memset(_dataSS, 0, sizeof(float) * size);
		memset(_dataDD, 0, sizeof(float) * size);
	}

	/**
	 * @brief 销毁 SU2Vertex 两个粒子对象. 
	 */
	~SU2VertexTwoParticle()
	{
		delete[] _dataSS;
		delete[] _dataDD;
	}

	/**
	 * @brief 在 [0,size) 范围内展开一个线性迭代器，迭代所有顶点条目. 
	 * 
	 * @param[in] iterator 线性迭代器. 
	 * @param[out] i1 格点迭代器. 
	 * @param[out] s 第一频率参数. 
	 * @param[out] t 第二频率参数. 
	 * @param[out] u 第三频率参数. 
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
	 * @brief 在 [0,sizeFrequency) 范围内展开线性迭代器，迭代所有参数化频率值. 
	 * 
	 * @param[in] iterator 线性迭代器. 
	 * @param[out] s 第一频率参数. 
	 * @param[out] t 第二频率参数. 
	 * @param[out] u 第三频率参数. 
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
	 * @brief 通过线性迭代器直接访问 [0,size) 范围内的顶点值. 
	 * 
	 * @param iterator 线性迭代器. 
	 * @param symmetry 顶点通道. 
	 * @return float& 顶点值. 
	 */
	float &getValueRef(const int iterator, const SU2VertexTwoParticle::Symmetry symmetry) const
	{
		if (symmetry == SU2VertexTwoParticle::Symmetry::Spin) return _dataSS[iterator];
		else return _dataDD[iterator];
	}

	/**
	 * @brief 访问任意晶格位置、频率和对称性处的顶点值. 
	 * 
	 * @param i1 第一个格点参数. 
	 * @param i2 第二个格点参数. 
	 * @param s 第一频率参数. 
	 * @param t 第二频率参数. 
	 * @param u 第三频率参数. 
	 * @param symmetry 顶点通道. 
	 * @param channel 频率通道. 
	 * @return float 顶点值. 
	 */
	float getValue(LatticeIterator i1, LatticeIterator i2, float s, float t, float u, const SU2VertexTwoParticle::Symmetry symmetry, const SU2VertexTwoParticle::FrequencyChannel channel) const
	{
		//映射到正频率扇区
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
	 * @brief 通过给定的访问缓冲区访问任意晶格位置处的顶点值和对称性. 
	 * 
	 * @tparam n 访问缓冲区中支持站点的数量. 
	 * @param i1 第一个格点参数. 
	 * @param i2 第二个格点参数. 
	 * @param symmetry 顶点通道. 
	 * @param accessBuffer 访问缓冲区. 
	 * @return float 顶点值. 
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
	 * @brief 通过给定的访问缓冲区以任意对称性本地访问顶点值
	 * 
	 * @tparam n 访问缓冲区中支持站点的数量. 
	 * @param symmetry 顶点通道. 
	 * @param accessBuffer 访问缓冲区. 
	 * @return float 顶点值. 
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
	 * @brief 通过给定的访问缓冲区同时对所有晶格位置和对称性进行捆绑顶点访问. 
	 * 
	 * @tparam n 访问缓冲区中支持站点的数量. 
	 * @param[in] accessBuffer 访问缓冲区. 
	 * @param[out] superbundle 顶点值束. 
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
	 * @brief 为一组频率生成访问缓冲区，其中一个频率（由通道指定）恰好位于频率网格上. 
	 * 频率通道必须是 FrequencyChannel::S, FrequencyChannel::T, FrequencyChannel::U. 
	 * 
	 * @param s 第一频率参数. 
	 * @param t 第二频率参数. 
	 * @param u 第三频率参数. 
	 * @param channel 频率通道. 
	 * @return SU2VertexTwoParticleAccessBuffer<4> 访问缓冲区. 
	 */
	SU2VertexTwoParticleAccessBuffer<4> generateAccessBuffer(float s, float t, float u, const SU2VertexTwoParticle::FrequencyChannel channel) const
	{
		ASSERT(channel == FrequencyChannel::S || channel == FrequencyChannel::T || channel == FrequencyChannel::U);

		SU2VertexTwoParticleAccessBuffer<4> accessBuffer;

		//映射到正频率扇区
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

		//插值频率
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
	 * @brief 为任意一组频率生成访问缓冲区.  
	 * 
	 * @param s 第一频率参数. 
	 * @param t 第二频率参数. 
	 * @param u 第三频率参数. 
	 * @return SU2VertexTwoParticleAccessBuffer<8> 访问缓冲区. 
	 */
	SU2VertexTwoParticleAccessBuffer<8> generateAccessBuffer(float s, float t, float u) const
	{
		SU2VertexTwoParticleAccessBuffer<8> accessBuffer;

		//映射到正频率扇区
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
	 * @brief 通过给定频率和站点偏移量直接访问顶点，其中 s Offset 可能小于 u Offset. 
	 * 
	 * @param siteOffset 晶格位置偏移（元素数量）. 
	 * @param sOffset 第一频率偏移（元素数量）. 
	 * @param tOffset 第二频率偏移（元素数量）. 
	 * @param uOffset 第三频率偏移（元素数量）. 
	 * @param symmetry 顶点通道. 
	 * @return float 顶点值. 
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
	 * @brief 通过给定频率和站点偏移直接访问顶点，其中 s Offset >= u Offset. 
	 * 
	 * @param siteOffset 晶格位置偏移（元素数量）. 
	 * @param sOffset 第一频率偏移（元素数量）. 
	 * @param tOffset 第二频率偏移（元素数量）. 
	 * @param uOffset 第三频率偏移（元素数量）. 
	 * @param symmetry 顶点通道. 
	 * @return float 顶点值. 
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
	 * @brief 根据给定的频率偏移计算总内存偏移（元素数量），其中 s Offset 可能小于 u Offset. 
	 * 
	 * @param[in] sOffset 第一频率偏移（元素数量）. 
	 * @param[in] tOffset 第二频率偏移（元素数量）. 
	 * @param[in] uOffset 第三频率偏移（元素数量）. 
	 * @param[out] signFlag 要存储在访问缓冲区中的符号标志. 
	 * @return int 内存偏移量（元素数量）. 
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

	int size; ///< 每个顶点通道的顶点大小（元素数量）. 
	int sizeFrequency; ///< 频率子空间中每个顶点通道的顶点大小（元素数量）. 

	float *_dataSS; ///< 顶点的自旋通道. 
	float *_dataDD; ///< 顶点的密度通道. 
	int _memoryStepLatticeT; ///< 最后 2 维中的内存步幅宽度. 
	int _memoryStepLattice; ///< 最后一维的内存步幅宽度. 
};