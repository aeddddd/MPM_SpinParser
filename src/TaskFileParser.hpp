/**
 * @file TaskFileParser.hpp
 * @author Finn Lasse Buessen
 * @brief Task file parser routine. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <string>
#include <set>
#include <boost/property_tree/ptree.hpp>

struct FrequencyDiscretization;
struct CutoffDiscretization;
struct Lattice;
struct ComputationStatus;
class FrgCore;

/**
 * @brief �����ļ�����������.�ö���󶨵����������ļ�,�����ȡ���������ļ�,�����Զ������д�����
 */
class TaskFileParser
{
public:
	/**
	 * @brief ����һ���µ�TaskFileParser���󲢽���ָ���������ļ�,������xml�ļ����д���
	 * @details �ڽ���������,TaskFileParser������ͳ�ʼ����ʹ�������ļ���ָ���Ĳ�����FrequencyDiscretization��CutoffDiscretization��Lattice����
	 * ����Щ�����ָ��ֱ�洢��frequency��cutoff��lattice�С�
	 * ����,���������������ļ���ָ����FrgCore������FrgCore::measurements��,������д��frgCore��
	 * �������������ļ�������ļ���״̬��
	 * 
	 * @param[in] taskFilePath Ҫ�����������ļ���·��,ʹ�� const �̶�.
	 * @param[out] frequency �����ɵ�FrequencyDiscretization(Ƶ����ɢ��)
	 * @param[out] cutoff �����ɵ�CutoffDiscretization(�ض���ɢ��)
	 * @param[out] lattice �����ɵ�Lattice(����)
	 * @param[out] frgCore �����ɵ�FRG����
	 * @param[out] computationStatus �������ļ�������ļ���״̬��
	 */
	TaskFileParser(const std::string &taskFilePath, FrequencyDiscretization *&frequency, CutoffDiscretization *&cutoff, Lattice *&lattice, FrgCore *&frgCore, ComputationStatus &computationStatus);

	/**
	 * @brief ������״̬д�������ļ�. 
	 * 
	 * @param finalize �������Ϊtrue,������״̬����Ϊ'finished'. �ж������Ƿ����
	 * @param updateCheckpoint �������Ϊtrue,�����һ������ʱ�佫����Ϊ��ǰʱ��. ���¼���.
	 */
	void writeTaskFile(const ComputationStatus &computationStatus);

protected: 
	/**
	 * @brief ȷ������������������������֪�ı�����ѡԪ�ص��ض��ڵ�.���Ҫ��δ����,���׳�Exception::Type::InitializationError
	 * 
	 * @param tree Ҫ���Ե�������. 
	 * @param node Ҫ���ԵĽڵ�. 
	 * @param requiredChildren �ڵ�ı��������б�. 
	 * @param requiredAttributes �ڵ�ı��������б�. 
	 * @param optionalChildren �ڵ�Ŀ�ѡ�����б�,Ĭ��Ϊ��. 
	 * @param optionalAttributes  �ڵ�Ŀ�ѡ�����б�,Ĭ��Ϊ��.
	 */
	void _validateProperties(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &requiredChildren, const std::set<std::string> &requiredAttributes, const std::set<std::string> &optionalChildren = {}, const std::set<std::string> &optionalAttributes = {}) const;

	/**
	 * @brief ȷ�������������������ض��ڵ�,�����Ըýڵ��Ƿ��������������б�.���������Ҫ��,������ Exception::Type::InitializationError��
	 * 
	 * @param tree Ҫ���Ե�������. 
	 * @param node Ҫ���ԵĽڵ�. 
	 * @param requiredChildren �ڵ�������ӽڵ��б�. 
	 * @param treePath ����·��,�ڿ��ܻ����׳����쳣����Ϊ�ڵ����Ƶ�ǰ׺.  
	 */
	void _validateRequiredChildren(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &requiredChildren, const std::string &treePath = "") const;
	
	/**
	 * @brief ȷ�������������������ض��ڵ㣬�����Ըýڵ��Ƿ�����������Ե��б����������Ҫ�������� Exception::Type::InitializationError.
	 * 
	 * @param tree Ҫ���Ե�������. 
	 * @param node Ҫ���ԵĽڵ�. 
	 * @param requiredAttributes �ڵ���������б�. 
	 * @param treePath ����·��,�ڿ��ܻ����׳����쳣����Ϊ�ڵ����Ƶ�ǰ׺.  
	 */
	void _validateRequiredAttributes(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &requiredAttributes, const std::string &treePath = "") const;
	
	/**
	 * @brief ȷ�������������������ض��ڵ㣬�����Ըýڵ��Ƿ��������ѡֵ�б��е�������������Ҫ�������� Exception::Type::InitializationError.
	 * 
	 * @param tree Ҫ���Ե�������. 
	 * @param node Ҫ���ԵĽڵ�. 
	 * @param optionalChildren �ڵ�Ŀ�ѡ�ӽڵ��б�. 
	 * @param treePath ����·��,�ڿ��ܻ����׳����쳣����Ϊ�ڵ����Ƶ�ǰ׺.  
	 */
	void _validateOptionalChildren(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &optionalChildren, const std::string &treePath = "") const;
	
	/**
	 * @brief ȷ�������������������ض��ڵ㣬�����Ըýڵ��Ƿ��������ѡֵ�б��е����ԡ����������Ҫ�������� Exception::Type::InitializationError.
	 * 
	 * @param tree Ҫ���Ե�������. 
	 * @param node Ҫ���ԵĽڵ�. 
	 * @param optionalAttributes �ڵ�Ŀ�ѡ�����б�. 
	 * @param treePath ����·��,�ڿ��ܻ����׳����쳣����Ϊ�ڵ����Ƶ�ǰ׺.  
	 */
	void _validateOptionalAttributes(const boost::property_tree::ptree &tree, const std::string &node, const std::set<std::string> &optionalAttributes, const std::string &treePath = "") const;

	boost::property_tree::ptree _taskFile; ///< ���������ļ����ڲ���������ʾ��ʽ. 
};