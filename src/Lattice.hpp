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
 * @brief �������ӵ���ɲ���. 
 */
enum struct SpinComponent : int
{
	X = 0, ///< x-component of a spin. 
	Y = 1, ///< y-component of a spin. 
	Z = 2, ///< z-component of a spin. 
	None = 3 ///< No spin component specified. 
};

/**
 * @brief ���������� j v(i1,j)*v(j,i2) ��ʽ��صľ��񲿷ֵĽṹ. 
 * @details ���������� j v(i1,j)*v(j,i2) ��ʽ��صľ��񲿷ֵĽṹ, 
 * ���� v(...) ��һ��˫���Ӷ��㺯����������������λ�����. 
 * �ص�������������ضϷ�Χ�ڵ����о���λ��. 
 * �����ص��ṹ����һ�������� id �б������ص������ڵ�ÿ�� j����һ������ v(i1,j) �ھ���Գ���Լ����ӳ�䵽���б�. 
 * ���Ƶأ��������ڶ������� v(j,i2) ӳ�䵽�Ĵ����� id �б�. 
 * ���⣬�洢�ԳƱ任�������������. 
 * (rid1[i],rid2[i],�任���X1[i],�任���Y1[i],�任���Z1[i],�任���X2[i],�任���Y2[i],�任���Z2[i])��ÿ��Ԫ�������ص���� j �ϵ��ܺ��еĵ�����ı任. 
 */
struct LatticeOverlap
{
	friend std::pair<Lattice *, SpinModel *> LatticeModelFactory::newLatticeModel(const LatticeModelFactory::LatticeUnitCell &uc, const LatticeModelFactory::SpinModelUnitCell &spinModelDefinition, const int latticeRange, const std::string &ldfPath);
	friend struct Lattice;

public:
	LatticeOverlap() : rid1(nullptr), rid2(nullptr), transformedX1(nullptr), transformedY1(nullptr), transformedZ1(nullptr), transformedX2(nullptr), transformedY2(nullptr), transformedZ2(nullptr), size(0) {}

	/**
	 * @brief Ϊ����������վ�㹹��һ���µ� Lattice Overlap ����. 
	 * 
	 * @param size �ص������վ������. 
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
	 * @brief �����е� Lattice Overlap ������һ���µ� Lattice Overlap ����.
	 *
	 * @param rhs �����ص�����.
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
 * @brief ָ���ԳƱ任����λ��Ľṹ. 
 * @details �ԳƱ任����˵���ͬ����λ�õ�ӳ�����������������.
 * �������д洢�� Lattice Site Descriptor::spin Permutation �У����б任��� Component=spin Permutation[original Component]��
 */
struct LatticeSiteDescriptor
{
	int rid; ///< �任��ĸ��Ĵ���id. 
	SpinComponent spinPermutation[3]; ///< �任���漰����������. 
};

/**
 * @brief Lattice iterator object
 */
struct LatticeIterator
{
friend struct Lattice;

public:
	/**
	 * @brief ����һ���µ� Lattice Iterator ����Ĭ��ָ����� id 0. 
	 */
	LatticeIterator() : id(0) {}

	/**
	 * @brief ����һ���µ� Lattice Iterator ���󲢽����ʼ��Ϊ�κ��д����Ե� id. 
	 * 
	 * @param id Ҫ��ʼ���Ĵ��� ID. 
	 */
	LatticeIterator(int id) : id(id) {}

	/**
	 * @brief �Ƚ������. 
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool ���������������ָ��ͬһ������ id���򷵻� true�����򷵻� false. 
	 */
	bool operator==(const LatticeIterator &rhs) const
	{
		return id == rhs.id;
	}

	/**
	 * @brief ���Ƚ������. 
	 * 
	 * @param rhs �Ҳ������. 
	 * @return bool ���������������ָ��ͬһ������ id���򷵻� false�����򷵻� true. 
	 */
	bool operator!=(const LatticeIterator &rhs) const
	{
		return id != rhs.id;
	}

	/**
	 * @brief ���������. 
	 * 
	 * @param rhs �Ҳ������. 
	 * @return int ������֮�������. 
	 */
	int operator-(const LatticeIterator &rhs) const
	{
		return id - rhs.id;
	}

	/**
	 * @brief ǰ׺���������. 
	 * 
	 * @return LatticeIterator& Reference to self. 
	 */
	virtual LatticeIterator& operator++()
	{
		++id;
		return *this;
	}

	/**
	 * @brief ǰ׺�Լ������. 
	 * 
	 * @return LatticeIterator& Reference to self. 
	 */
	virtual LatticeIterator& operator--()
	{
		--id;
		return *this;
	}

protected:
	int id; ///< ������ָ��Ĵ���id. 
};

/**
 * @brief �Ӿ������������. 
 */
struct SublatticeIterator : public LatticeIterator
{
public:
	/**
	 * @brief �ڴ����� id �б��Ϲ���һ���µ� Sublattice Iterator ����. 
	 * @details �Ӹ��������ӵ���б������Ȩ. 
	 * ���뱣֤�б��ڵ�����������������������ڱ�����Ч. 
	 * ���б������lattice->end()��Ŀ��ֹ. 
	 * 
	 * @param allowedIds ����Ӿ���Ĵ����� id �б�������lattice->end()��ֹ�� 
	 */
	SublatticeIterator(int *allowedIds) : LatticeIterator(*allowedIds), offset(0), allowedIds(allowedIds) {}

	/**
	 * @brief ǰ׺���������. 
	 * 
	 * @return SublatticeIterator& Reference to self. 
	 */
	SublatticeIterator &operator++()
	{
		id = allowedIds[++offset];
		return *this;
	}

	/**
	 * @brief ǰ׺�Լ������. 
	 * 
	 * @return SublatticeIterator& Reference to self. 
	 */
	SublatticeIterator &operator--()
	{
		id = allowedIds[--offset];
		return *this;
	}

protected:
	int offset; ///< ����� Sublattice Iterator::allowed Ids ��һ����Ŀ��ƫ����. 
	int *allowedIds; ///< ����Ӿ���Ĵ����� id �б��ڴ治���ڵ�����. 
};

/**
 * @brief ������ı�ʾ����������������ĶԳ���Ϣ. 
 * @details �����ʾ�����˶Գ��ԡ������ľ����������������λ�㣬��ȡ������ѡ�ľ���Χ����. 
 * ���񽫴洢�κλ���λ����Χָ����Χ���Ծ����Ϊ��λ�����ľ��룩�ڵ�����λ��. 
 *
 * ʹ�þ���Գ��ԣ����κ�һ�Ծ���λ�㣨�κ�������������У�һ��λ��ʼ�տ���ӳ�䵽ԭ�㴦�Ĺ̶��ο�λ��. 
 * Ȼ���ڴ�ӳ���µĵڶ�������վ�㽫��ӳ�䵽����ĳ��վ��. 
 * ��ˣ��Գ���Լ��������������������ڵ�������λ��. 
 * �����κ�����������������Щվ�����С��������ν�Ĵ����� id ö�٣���Χ���㵽ĳ������ֵ������ȡ���ھ��񼸺���״. 
 * 
 * �����ʾ�ṩ�˿��ٵ������񡢵���ĳЩ�Ӿ���������κ����������ӳ�䵽��Ӧ����λ��ķ���. 
 */
struct Lattice
{
	friend std::pair<Lattice *, SpinModel *> LatticeModelFactory::newLatticeModel(const LatticeModelFactory::LatticeUnitCell &uc, const LatticeModelFactory::SpinModelUnitCell &spinModelDefinition, const int latticeRange, const std::string &ldfPath);

protected:
	/**
	 * @brief ����δ��ʼ���ľ������. 
	 * @details ��Ӧֱ�ӵ��ù��캯����ʹ�� Lattice Model Factory::new Lattice Model() �����µľ���. 
	 */
	Lattice() : size(0), _dataSize(0), _symmetryTable(nullptr), _bufferSites(nullptr), _bufferInvertedSites(nullptr), _bufferOverlapMatrices(nullptr), _bufferBasis(nullptr), _bufferLatticeRange(nullptr) {};

public:
	/**
	 * @brief ���� Lattice ����. 
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
	 * @brief ��������������һ�����. 
	 * 
	 * @return LatticeIterator ����һ�����ĵ�����. 
	 */
	LatticeIterator begin() const
	{
		return LatticeIterator(0);
	}

	/**
	 * @brief ��������� i1=(0,0,0,0) �ĵ�����.
	 * 
	 * @return LatticeIterator �����ĵ����� i1=(0,0,0,0).
	 */
	LatticeIterator zero() const
	{
		return LatticeIterator(0);
	}

	/**
	 * @brief ���������+1���ĵ�����. 
	 * 
	 * @return LatticeIterator �����+1���ĵ�����. 
	 */
	LatticeIterator end() const
	{
		return LatticeIterator(_dataSize);
	}

	/**
	 * @brief ����ָ������ĵ�����. 
	 * 
	 * @param rid ������Ĵ��� id. 
	 * @return LatticeIterator ��ָ������վ��ĵ�����. 
	 */
	LatticeIterator fromParametrization(const int rid) const
	{
		ASSERT(rid < size);
		return LatticeIterator(rid);
	}

	/**
	 * @brief ��ȡָ���������꣬��λΪa1��a2��a3��b. 
	 * 
	 * @param site ������. 
	 * @return std::tuple<int, int, int, int> Ԫ�� (n1,n2,n3,nb) �������� n1*a1+n2*a2+n3*a3+b[nb] ���ľ���λ��. 
	 */
	std::tuple<int, int, int, int> getSiteParameters(const LatticeIterator &site) const
	{
		ASSERT(site - begin() < _dataSize);
		return _geometryTable[site.id];
	}

	/**
	 * @brief ��ȡʵ�ռ��и���λ��. 
	 * 
	 * @param site ������. 
	 * @return geometry::Vec3<double> �����ʵ�ռ��е�λ��. 
	 */
	geometry::Vec3<double> getSitePosition(const LatticeIterator &site) const
	{
		ASSERT(site - begin() < _dataSize);
		return double(std::get<0>(_geometryTable[site.id])) * _bravaisLattice[0] + double(std::get<1>(_geometryTable[site.id])) * _bravaisLattice[1] + double(std::get<2>(_geometryTable[site.id])) * _bravaisLattice[2] + _basis[std::get<3>(_geometryTable[site.id])];
	}

	/**
	 * @brief �任һ�Ը�� (i1,i2) ������任Ϊ (0,i2'). 
	 * �ñ任�����漰�������������У������ý�������. 
	 * 
	 * @param i1 ������. 
	 * @param i2 ������. 
	 * @return int i2'�Ĵ���id. 
	 */
	int symmetryTransform(const LatticeIterator& i1, const LatticeIterator& i2) const
	{
		ASSERT(_symmetryTable[i1.id * _dataSize + i2.id].rid != -1);
		return _symmetryTable[i1.id * _dataSize + i2.id].rid;
	}

	/**
	 * @brief �任һ�Ը�� (i1,i2) ������任Ϊ (0,i2'). 
	 * �任�����漰�������������У�����������£��任Ӧ����������������. 
	 * 
	 * @param[in] i1 ������. 
	 * @param[in] i2 ������. 
	 * @param[out] spinComponent ���������ӳ���½��б任. 
	 * @return int i2'�Ĵ���id. 
	 */
	int symmetryTransform(const LatticeIterator &i1, const LatticeIterator &i2, SpinComponent &spinComponent) const
	{
		ASSERT(_symmetryTable[i1.id * _dataSize + i2.id].rid != -1);
		if (spinComponent != SpinComponent::None) spinComponent = _symmetryTable[i1.id * _dataSize + i2.id].spinPermutation[static_cast<int>(spinComponent)];
		return _symmetryTable[i1.id * _dataSize + i2.id].rid;
	}

	/**
	 * @brief �任һ�Ը�� (i1,i2) ������任Ϊ (0,i2'). 
	 * �ñ任�����漰�������������У�����������£��任��Ӧ������������ 1 ���������� 2 ����. 
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
	 * @brief ˫����������б� (i2,i1)������ i1=(0,0,0,0) �ǲο�λ�㣬���б�������вο�λ�� i2. 
	 * 
	 * @return const LatticeSiteDescriptor* �Գ��Խ��͵�˫����������б� (i1,i2). 
	 */
	const LatticeSiteDescriptor *getInvertedSites() const
	{
		return _bufferInvertedSites;
	}

	/**
	 * @brief ˫����������б� (i1,i2)������ i1=(0,0,0,0) �ǲο�λ�㣬���б�������вο�λ�� i2. 
	 * 
	 * @return const LatticeSiteDescriptor* �Գ��Խ��͵�˫����������б� (i1,i2). 
	 */
	const LatticeSiteDescriptor *getSites() const
	{
		return _bufferSites;
	}

	/**
	 * @brief ����λ�ڸ�� b ��Χ�ڵ����е�ĵ����������� b ������������֮һ. 
	 * @see Lattice::getBasis()
	 * 
	 * @param b ������վ��ĵ�����.  
	 * @return SublatticeIterator ��Χ������վ��ĵ�����. 
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
	 * @brief ����λ�ڸ�� (0,0,0,b) ��Χ�ڵ����е�ĵ�����. 
	 * 
	 * @param b ����վ������. 
	 * @return SublatticeIterator ��Χ������վ��ĵ�����. 
	 */
	SublatticeIterator getRange(const int b) const
	{
		ASSERT(b < _basis.size());
		return SublatticeIterator(_bufferLatticeRange[b]);
	}

	/**
	 * @brief �������л���վ���ϵĵ�����. 
	 * 
	 * @return SublatticeIterator ���л���վ��ĵ�����. 
	 */
	SublatticeIterator getBasis() const
	{
		return SublatticeIterator(_bufferBasis);
	}

	std::vector<geometry::Vec3<double> > _bravaisLattice; ///< ��������ά���������б�.  
	std::vector<geometry::Vec3<double> > _basis; ///< ���񾧰������л���λ�õ��б�. 

	int size; ///< ����վ������. 

protected: 
	std::vector<std::tuple<int, int, int, int>> _geometryTable; ///< ʵ�ռ侧��λ�õ��ڲ��洢���洢ΪԪ�飨a0��a1��a2��b��.

	int _dataSize; ///< ���Ǵ洢��Ϣ�����и��ӵ��������൱�� Lattice::geometry Table �Ĵ�С��. 
	
	LatticeSiteDescriptor *_symmetryTable; ///< ������ (id1, id2) �ĶԳ���Լ���б����Ի�Ϊ id1* ���ݴ�С+id2���洢�� id1 ӳ�䵽�������ת��. 
	LatticeSiteDescriptor *_bufferSites; ///< ת��λ���б�0��rid�������д���rid. 
	LatticeSiteDescriptor *_bufferInvertedSites; ///< ת��λ���б�rid��0�������д���rid. 
	LatticeOverlap *_bufferOverlapMatrices; ///< �����ص��б����е� i ����Ŀ������Ԫ�� (0,j)(j,i) ���ص�. 

	int *_bufferBasis; ///< ���л���վ��Ĵ����� ID �б�. 
	int **_bufferLatticeRange; ///< վ�㷶Χ������վ�� ID ���б�� (0,0,0,b). 
};