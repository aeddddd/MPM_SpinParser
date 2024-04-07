/**
 * @file SpinModel.hpp
 * @author Finn Lasse Buessen
 * @brief Representation of a spin model with two-spin interactions. 
 * 
 * @copyright Copyright (c) 2020
 */

#pragma once
#include <tuple>
#include "Lattice.hpp"

/**
 * @brief ����ģ�ͱ�ʾ. 
 * @details ����ģ�ͱ�ʾ���ܶ����е����������໥���õļ���. 
 * pf-FRG �����ĳ�ʼ����ȡ������Щ���. 
 * ���,��תģ�Ͷ��󱻴��ݸ���Ч��������ĳ�ʼ��. 
 */
struct SpinModel
{
public:
	/**
	 * @brief �������໥���õı�ʾ.
	 */
	struct SpinInteraction
	{
		/**
		 * @brief ����һ���µ� Spin Interaction ���󲢽����н�����ʼ��Ϊ��. 
		 */
		SpinInteraction()
		{
			memset(&interactionStrength[0][0], 0, 9 * sizeof(float));
		};

		float interactionStrength[3][3]; ///< �໥����ǿ��,����ΪinteractionStrength[s1][s2],���� s1 �ǵ�һ�������� x��y �� z��0��1 �� 2������,s2 �ǵڶ��������ķ���. 
	};

	std::vector<std::pair<LatticeIterator, SpinInteraction> > interactions; ///< ģ���е�˫�����໥�����б�.�����໥���ö������ھ���λ��� (s1,s2) ֮��,���� s1 �ǲο�λ��,s2 �ɾ��������ָ��. 
	std::vector<std::string> interactionParameters; ///< �����ļ���ʹ�õ��ַ�����ʽ�����������б�.�б��˳���Ӧ�� SpinModel::interactions �е�˳��. 
};