/**
 * @file Lattice.hpp
 * @author Finn Lasse Buessen
 * @brief Representation of a physical lattice. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <array>
#include <vector>
#include <tuple>
#include "lib/Geometry.hpp"
#include "lib/Assert.hpp"

struct SpinModel;
struct Lattice;
namespace LatticeModelFactory {
	struct LatticeUnitCell;
	struct SpinModelUnitCell;

	std::pair<Lattice *, SpinModel *> newLatticeModel(const LatticeModelFactory::LatticeUnitCell &uc, const LatticeModelFactory::SpinModelUnitCell &spinModelDefinition, const int latticeRange, const std::string &ldfPath);
};

/**
 * @brief 自旋算子的组成部分. 
 */
enum struct SpinComponent : int
{
	X = 0, ///< x-component of a spin. 
	Y = 1, ///< y-component of a spin. 
	Z = 2, ///< z-component of a spin. 
	None = 3 ///< No spin component specified. 
};

/**
 * @brief 用于描述与 j v(i1,j)*v(j,i2) 形式相关的晶格部分的结构. 
 * @details 用于描述与 j v(i1,j)*v(j,i2) 形式相关的晶格部分的结构, 
 * 其中 v(...) 是一个双粒子顶点函数，具有两个晶格位点参数. 
 * 重叠覆盖两个顶点截断范围内的所有晶格位点. 
 * 晶格重叠结构包含一个代表性 id 列表，对于重叠区域内的每个 j，第一个顶点 v(i1,j) 在晶格对称性约简下映射到该列表. 
 * 类似地，它包含第二个顶点 v(j,i2) 映射到的代表性 id 列表. 
 * 此外，存储对称变换所需的自旋排列. 
 * (rid1[i],rid2[i],变换后的X1[i],变换后的Y1[i],变换后的Z1[i],变换后的X2[i],变换后的Y2[i],变换后的Z2[i])的每个元组描述重叠格点 j 上的总和中的单个项的变换. 
 */
struct LatticeOverlap
{
	friend std::pair<Lattice *, SpinModel *> LatticeModelFactory::newLatticeModel(const LatticeModelFactory::LatticeUnitCell &uc, const LatticeModelFactory::SpinModelUnitCell &spinModelDefinition, const int latticeRange, const std::string &ldfPath);
	friend struct Lattice;

public:
	LatticeOverlap() : rid1(nullptr), rid2(nullptr), transformedX1(nullptr), transformedY1(nullptr), transformedZ1(nullptr), transformedX2(nullptr), transformedY2(nullptr), transformedZ2(nullptr), size(0) {}

	/**
	 * @brief 为给定数量的站点构造一个新的 Lattice Overlap 对象. 
	 * 
	 * @param size 重叠区域的站点数量. 
	 */
	LatticeOverlap(const int size) : size(size)
	{
		rid1 = new int[size];
		rid2 = new int[size];
		transformedX1 = new SpinComponent[size];
		transformedY1 = new SpinComponent[size];
		transformedZ1 = new SpinComponent[size];
		transformedX2 = new SpinComponent[size];
		transformedY2 = new SpinComponent[size];
		transformedZ2 = new SpinComponent[size];
	}

	/**
	 * @brief 从现有的 Lattice Overlap 对象构造一个新的 Lattice Overlap 对象.
	 *
	 * @param rhs 晶格重叠对象.
	 */
	LatticeOverlap(const LatticeOverlap &rhs) : LatticeOverlap(rhs.size)
	{
		memcpy(rid1, rhs.rid1, size * sizeof(int));
		memcpy(rid2, rhs.rid2, size * sizeof(int));
		memcpy(transformedX1, rhs.transformedX1, size * sizeof(SpinComponent));
		memcpy(transformedY1, rhs.transformedY1, size * sizeof(SpinComponent));
		memcpy(transformedZ1, rhs.transformedZ1, size * sizeof(SpinComponent));
		memcpy(transformedX2, rhs.transformedX2, size * sizeof(SpinComponent));
		memcpy(transformedY2, rhs.transformedY2, size * sizeof(SpinComponent));
		memcpy(transformedZ2, rhs.transformedZ2, size * sizeof(SpinComponent));
	}

	int *rid1;
	int *rid2;
	SpinComponent *transformedX1;
	SpinComponent *transformedY1;
	SpinComponent *transformedZ1;
	SpinComponent *transformedX2;
	SpinComponent *transformedY2;
	SpinComponent *transformedZ2;
	int size;

protected:
	/**
	 * @brief Destroy the LatticeOverlap object. 
	 */
	~LatticeOverlap()
	{
		delete[] rid1;
		delete[] rid2;
		delete[] transformedX1;
		delete[] transformedY1;
		delete[] transformedZ1;
		delete[] transformedX2;
		delete[] transformedY2;
		delete[] transformedZ2;
	}

	/**
	 * @brief Assignment operator. 
	 * 
	 * @param rhs Object to assign. 
	 * @return LatticeOverlap& Reference to self. 
	 */
	LatticeOverlap &operator=(const LatticeOverlap &rhs)
	{
		size = rhs.size;
		delete[] rid1;
		rid1 = new int[size];
		memcpy(rid1, rhs.rid1, size * sizeof(int));
		delete[] rid2;
		rid2 = new int[size];
		memcpy(rid2, rhs.rid2, size * sizeof(int));
		delete[] transformedX1;
		transformedX1 = new SpinComponent[size];
		memcpy(transformedX1, rhs.transformedX1, size * sizeof(SpinComponent));
		delete[] transformedY1;
		transformedY1 = new SpinComponent[size];
		memcpy(transformedY1, rhs.transformedY1, size * sizeof(SpinComponent));
		delete[] transformedZ1;
		transformedZ1 = new SpinComponent[size];
		memcpy(transformedZ1, rhs.transformedZ1, size * sizeof(SpinComponent));
		delete[] transformedX2;
		transformedX2 = new SpinComponent[size];
		memcpy(transformedX2, rhs.transformedX2, size * sizeof(SpinComponent));
		delete[] transformedY2;
		transformedY2 = new SpinComponent[size];
		memcpy(transformedY2, rhs.transformedY2, size * sizeof(SpinComponent));
		delete[] transformedZ2;
		transformedZ2 = new SpinComponent[size];
		memcpy(transformedZ2, rhs.transformedZ2, size * sizeof(SpinComponent));

		return *this;
	}
};

/**
 * @brief 指定对称变换晶格位点的结构. 
 * @details 对称变换结合了到不同晶格位置的映射和自旋分量的排列.
 * 自旋排列存储在 Lattice Site Descriptor::spin Permutation 中，其中变换后的 Component=spin Permutation[original Component]。
 */
struct LatticeSiteDescriptor
{
	int rid; ///< 变换后的格点的代表id. 
	SpinComponent spinPermutation[3]; ///< 变换中涉及的自旋排列. 
};

/**
 * @brief Lattice iterator object
 */
struct LatticeIterator
{
friend struct Lattice;

public:
	/**
	 * @brief 构造一个新的 Lattice Iterator 对象，默认指向代表 id 0. 
	 */
	LatticeIterator() : id(0) {}

	/**
	 * @brief 构造一个新的 Lattice Iterator 对象并将其初始化为任何有代表性的 id. 
	 * 
	 * @param id 要初始化的代表 ID. 
	 */
	LatticeIterator(int id) : id(id) {}

	/**
	 * @brief 比较运算符. 
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 如果两个迭代器都指向同一个代表 id，则返回 true。否则返回 false. 
	 */
	bool operator==(const LatticeIterator &rhs) const
	{
		return id == rhs.id;
	}

	/**
	 * @brief 负比较运算符. 
	 * 
	 * @param rhs 右侧操作数. 
	 * @return bool 如果两个迭代器都指向同一个代表 id，则返回 false。否则返回 true. 
	 */
	bool operator!=(const LatticeIterator &rhs) const
	{
		return id != rhs.id;
	}

	/**
	 * @brief 减法运算符. 
	 * 
	 * @param rhs 右侧操作数. 
	 * @return int 迭代器之间的区别. 
	 */
	int operator-(const LatticeIterator &rhs) const
	{
		return id - rhs.id;
	}

	/**
	 * @brief 前缀增量运算符. 
	 * 
	 * @return LatticeIterator& Reference to self. 
	 */
	virtual LatticeIterator& operator++()
	{
		++id;
		return *this;
	}

	/**
	 * @brief 前缀自减运算符. 
	 * 
	 * @return LatticeIterator& Reference to self. 
	 */
	virtual LatticeIterator& operator--()
	{
		--id;
		return *this;
	}

protected:
	int id; ///< 迭代器指向的代表id. 
};

/**
 * @brief 子晶格迭代器对象. 
 */
struct SublatticeIterator : public LatticeIterator
{
public:
	/**
	 * @brief 在代表性 id 列表上构造一个新的 Sublattice Iterator 对象. 
	 * @details 子格迭代器不拥有列表的所有权. 
	 * 必须保证列表在迭代器对象的整个生命周期内保持有效. 
	 * 该列表必须由lattice->end()条目终止. 
	 * 
	 * @param allowedIds 组成子晶格的代表性 id 列表。必须由lattice->end()终止。 
	 */
	SublatticeIterator(int *allowedIds) : LatticeIterator(*allowedIds), offset(0), allowedIds(allowedIds) {}

	/**
	 * @brief 前缀增量运算符. 
	 * 
	 * @return SublatticeIterator& Reference to self. 
	 */
	SublatticeIterator &operator++()
	{
		id = allowedIds[++offset];
		return *this;
	}

	/**
	 * @brief 前缀自减运算符. 
	 * 
	 * @return SublatticeIterator& Reference to self. 
	 */
	SublatticeIterator &operator--()
	{
		id = allowedIds[--offset];
		return *this;
	}

protected:
	int offset; ///< 相对于 Sublattice Iterator::allowed Ids 第一个条目的偏移量. 
	int *allowedIds; ///< 组成子晶格的代表性 id 列表。内存不属于迭代器. 
};

/**
 * @brief 物理晶格的表示，具有两点相关器的对称信息. 
 * @details 晶格表示利用了对称性。给定的晶格包含有限数量的位点，这取决于所选的晶格范围参数. 
 * 晶格将存储任何基本位点周围指定范围（以晶格键为单位测量的距离）内的所有位点. 
 *
 * 使用晶格对称性，在任何一对晶格位点（任何两点相关器）中，一个位点始终可以映射到原点处的固定参考位点. 
 * 然后，在此映射下的第二个晶格站点将被映射到其他某个站点. 
 * 因此，对称性约简后的两点相关器仅依赖于单个晶格位点. 
 * 代表任何两点相关器所需的这些站点的最小设置由所谓的代表性 id 枚举，范围从零到某个有限值，具体取决于晶格几何形状. 
 * 
 * 晶格表示提供了快速迭代晶格、迭代某些子晶格区域或将任何两点相关器映射到相应代表位点的方法. 
 */
struct Lattice
{
	friend std::pair<Lattice *, SpinModel *> LatticeModelFactory::newLatticeModel(const LatticeModelFactory::LatticeUnitCell &uc, const LatticeModelFactory::SpinModelUnitCell &spinModelDefinition, const int latticeRange, const std::string &ldfPath);

protected:
	/**
	 * @brief 创建未初始化的晶格对象. 
	 * @details 不应直接调用构造函数。使用 Lattice Model Factory::new Lattice Model() 创建新的晶格. 
	 */
	Lattice() : size(0), _dataSize(0), _symmetryTable(nullptr), _bufferSites(nullptr), _bufferInvertedSites(nullptr), _bufferOverlapMatrices(nullptr), _bufferBasis(nullptr), _bufferLatticeRange(nullptr) {};

public:
	/**
	 * @brief 销毁 Lattice 对象. 
	 */
	~Lattice()
	{
		delete[] _bufferBasis;
		for (size_t b = 0; b < _basis.size(); ++b) delete[] _bufferLatticeRange[b];
		delete[] _bufferLatticeRange;
		delete[] _symmetryTable;
		delete[] _bufferSites;
		delete[] _bufferInvertedSites;
		delete[] _bufferOverlapMatrices;
	}

	/**
	 * @brief 检索迭代器到第一个格点. 
	 * 
	 * @return LatticeIterator 到第一个格点的迭代器. 
	 */
	LatticeIterator begin() const
	{
		return LatticeIterator(0);
	}

	/**
	 * @brief 检索到格点 i1=(0,0,0,0) 的迭代器.
	 * 
	 * @return LatticeIterator 到格点的迭代器 i1=(0,0,0,0).
	 */
	LatticeIterator zero() const
	{
		return LatticeIterator(0);
	}

	/**
	 * @brief 检索到最后+1格点的迭代器. 
	 * 
	 * @return LatticeIterator 到最后+1格点的迭代器. 
	 */
	LatticeIterator end() const
	{
		return LatticeIterator(_dataSize);
	}

	/**
	 * @brief 检索指定代表的迭代器. 
	 * 
	 * @param rid 所需格点的代表 id. 
	 * @return LatticeIterator 到指定代表站点的迭代器. 
	 */
	LatticeIterator fromParametrization(const int rid) const
	{
		ASSERT(rid < size);
		return LatticeIterator(rid);
	}

	/**
	 * @brief 获取指定格点的坐标，单位为a1、a2、a3、b. 
	 * 
	 * @param site 输入格点. 
	 * @return std::tuple<int, int, int, int> 元组 (n1,n2,n3,nb) 描述坐标 n1*a1+n2*a2+n3*a3+b[nb] 处的晶格位置. 
	 */
	std::tuple<int, int, int, int> getSiteParameters(const LatticeIterator &site) const
	{
		ASSERT(site - begin() < _dataSize);
		return _geometryTable[site.id];
	}

	/**
	 * @brief 获取实空间中格点的位置. 
	 * 
	 * @param site 输入格点. 
	 * @return geometry::Vec3<double> 格点在实空间中的位置. 
	 */
	geometry::Vec3<double> getSitePosition(const LatticeIterator &site) const
	{
		ASSERT(site - begin() < _dataSize);
		return double(std::get<0>(_geometryTable[site.id])) * _bravaisLattice[0] + double(std::get<1>(_geometryTable[site.id])) * _bravaisLattice[1] + double(std::get<2>(_geometryTable[site.id])) * _bravaisLattice[2] + _basis[std::get<3>(_geometryTable[site.id])];
	}

	/**
	 * @brief 变换一对格点 (i1,i2) 并将其变换为 (0,i2'). 
	 * 该变换可能涉及自旋分量的排列，其作用将被忽略. 
	 * 
	 * @param i1 输入格点. 
	 * @param i2 输入格点. 
	 * @return int i2'的代表id. 
	 */
	int symmetryTransform(const LatticeIterator& i1, const LatticeIterator& i2) const
	{
		ASSERT(_symmetryTable[i1.id * _dataSize + i2.id].rid != -1);
		return _symmetryTable[i1.id * _dataSize + i2.id].rid;
	}

	/**
	 * @brief 变换一对格点 (i1,i2) 并将其变换为 (0,i2'). 
	 * 变换可能涉及自旋分量的排列，在这种情况下，变换应用于自旋分量参数. 
	 * 
	 * @param[in] i1 输入格点. 
	 * @param[in] i2 输入格点. 
	 * @param[out] spinComponent 自旋组件在映射下进行变换. 
	 * @return int i2'的代表id. 
	 */
	int symmetryTransform(const LatticeIterator &i1, const LatticeIterator &i2, SpinComponent &spinComponent) const
	{
		ASSERT(_symmetryTable[i1.id * _dataSize + i2.id].rid != -1);
		if (spinComponent != SpinComponent::None) spinComponent = _symmetryTable[i1.id * _dataSize + i2.id].spinPermutation[static_cast<int>(spinComponent)];
		return _symmetryTable[i1.id * _dataSize + i2.id].rid;
	}

	/**
	 * @brief 变换一对格点 (i1,i2) 并将其变换为 (0,i2'). 
	 * 该变换可能涉及自旋分量的排列，在这种情况下，变换将应用于自旋分量 1 和自旋分量 2 参数. 
	 * 
	 * @param[in] i1 Input lattice site. 
	 * @param[in] i2 Input lattice site. 
	 * @param[out] spinComponent1 Spin component to transform under the mapping. 
	 * @param[out] spinComponent2 Spin component to transform under the mapping. 
	 * @return int Representative id of i2'. 
	 */
	int symmetryTransform(const LatticeIterator &i1, const LatticeIterator &i2, SpinComponent &spinComponent1, SpinComponent &spinComponent2) const
	{
		ASSERT(_symmetryTable[i1.id * _dataSize + i2.id].rid != -1);
		ASSERT(&spinComponent1 != &spinComponent2);

		if (spinComponent1 != SpinComponent::None) spinComponent1 = _symmetryTable[i1.id * _dataSize + i2.id].spinPermutation[static_cast<int>(spinComponent1)];
		if (spinComponent2 != SpinComponent::None) spinComponent2 = _symmetryTable[i1.id * _dataSize + i2.id].spinPermutation[static_cast<int>(spinComponent2)];
		return _symmetryTable[i1.id * _dataSize + i2.id].rid;
	}

	/**
	 * @brief Transform a pair of lattice sites (i1,i2) and transform it to (0,i2'). 
	 * The transformation may involve a permutation of spin components, in which case the transformation is applied to the spinComponent1, spinComponent2, and spinComponent3 arguments. 
	 * 
	 * @param[in] i1 Input lattice site. 
	 * @param[in] i2 Input lattice site. 
	 * @param[out] spinComponent1 Spin component to transform under the mapping. 
	 * @param[out] spinComponent2 Spin component to transform under the mapping. 
	 * @param[out] spinComponent3 Spin component to transform under the mapping. 
	 * @return int Representative id of i2'. 
	 */
	int symmetryTransform(const LatticeIterator &i1, const LatticeIterator &i2, SpinComponent &spinComponent1, SpinComponent &spinComponent2, SpinComponent &spinComponent3) const
	{
		ASSERT(_symmetryTable[i1.id * _dataSize + i2.id].rid != -1);
		ASSERT(&spinComponent1 != &spinComponent2);
		ASSERT(&spinComponent2 != &spinComponent3);
		ASSERT(&spinComponent1 != &spinComponent3);

		if (spinComponent1 != SpinComponent::None) spinComponent1 = _symmetryTable[i1.id * _dataSize + i2.id].spinPermutation[static_cast<int>(spinComponent1)];
		if (spinComponent2 != SpinComponent::None) spinComponent2 = _symmetryTable[i1.id * _dataSize + i2.id].spinPermutation[static_cast<int>(spinComponent2)];
		if (spinComponent3 != SpinComponent::None) spinComponent3 = _symmetryTable[i1.id * _dataSize + i2.id].spinPermutation[static_cast<int>(spinComponent3)];
		return _symmetryTable[i1.id * _dataSize + i2.id].rid;
	}

	/**
	 * @brief Retrieve the lattice overlap of the reference site i1=(0,0,0,0) with some other representative lattice site. 
	 * The overlap is defined as all lattice sites which are within range of both lattice site. 
	 * 
	 * @param rid Representative lattice site to determine overlap with i1. 
	 * @return const LatticeOverlap& Lattice overlap descriptor. 
	 */
	const LatticeOverlap &getOverlap(const int rid) const
	{
		ASSERT(rid < size);
		return _bufferOverlapMatrices[rid];
	}

	/**
	 * @brief 双自旋相关器列表 (i2,i1)，其中 i1=(0,0,0,0) 是参考位点，该列表包括所有参考位点 i2. 
	 * 
	 * @return const LatticeSiteDescriptor* 对称性降低的双自旋相关器列表 (i1,i2). 
	 */
	const LatticeSiteDescriptor *getInvertedSites() const
	{
		return _bufferInvertedSites;
	}

	/**
	 * @brief 双自旋相关器列表 (i1,i2)，其中 i1=(0,0,0,0) 是参考位点，该列表包括所有参考位点 i2. 
	 * 
	 * @return const LatticeSiteDescriptor* 对称性降低的双自旋相关器列表 (i1,i2). 
	 */
	const LatticeSiteDescriptor *getSites() const
	{
		return _bufferSites;
	}

	/**
	 * @brief 检索位于格点 b 范围内的所有点的迭代器，其中 b 必须描述基点之一. 
	 * @see Lattice::getBasis()
	 * 
	 * @param b 到基础站点的迭代器.  
	 * @return SublatticeIterator 范围内所有站点的迭代器. 
	 */
	SublatticeIterator getRange(const LatticeIterator &b) const
	{
		ASSERT(std::get<0>(getSiteParameters(b)) == 0);
		ASSERT(std::get<1>(getSiteParameters(b)) == 0);
		ASSERT(std::get<2>(getSiteParameters(b)) == 0);
		ASSERT(std::get<3>(getSiteParameters(b)) < _basis.size());
		
		return SublatticeIterator(_bufferLatticeRange[std::get<3>(getSiteParameters(b))]);
	}

	/**
	 * @brief 检索位于格点 (0,0,0,b) 范围内的所有点的迭代器. 
	 * 
	 * @param b 基础站点索引. 
	 * @return SublatticeIterator 范围内所有站点的迭代器. 
	 */
	SublatticeIterator getRange(const int b) const
	{
		ASSERT(b < _basis.size());
		return SublatticeIterator(_bufferLatticeRange[b]);
	}

	/**
	 * @brief 检索所有基础站点上的迭代器. 
	 * 
	 * @return SublatticeIterator 所有基础站点的迭代器. 
	 */
	SublatticeIterator getBasis() const
	{
		return SublatticeIterator(_bufferBasis);
	}

	std::vector<geometry::Vec3<double> > _bravaisLattice; ///< 三个布拉维格向量的列表.  
	std::vector<geometry::Vec3<double> > _basis; ///< 晶格晶胞内所有基点位置的列表. 

	int size; ///< 代表站点数量. 

protected: 
	std::vector<std::tuple<int, int, int, int>> _geometryTable; ///< 实空间晶格位置的内部存储，存储为元组（a0，a1，a2，b）.

	int _dataSize; ///< 我们存储信息的所有格子的总数（相当于 Lattice::geometry Table 的大小）. 
	
	LatticeSiteDescriptor *_symmetryTable; ///< 自旋对 (id1, id2) 的对称性约简列表线性化为 id1* 数据大小+id2。存储将 id1 映射到零所需的转换. 
	LatticeSiteDescriptor *_bufferSites; ///< 转化位点列表（0，rid），所有代表rid. 
	LatticeSiteDescriptor *_bufferInvertedSites; ///< 转化位点列表（rid，0），所有代表都rid. 
	LatticeOverlap *_bufferOverlapMatrices; ///< 晶格重叠列表，其中第 i 个条目是两个元组 (0,j)(j,i) 的重叠. 

	int *_bufferBasis; ///< 所有基础站点的代表性 ID 列表. 
	int **_bufferLatticeRange; ///< 站点范围内所有站点 ID 的列表表 (0,0,0,b). 
};