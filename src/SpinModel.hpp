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
 * @brief 自旋模型表示. 
 * @details 自旋模型表示哈密顿量中的两个自旋相互作用的集合. 
 * pf-FRG 流动的初始条件取决于这些耦合. 
 * 因此,旋转模型对象被传递给有效操作对象的初始化. 
 */
struct SpinModel
{
public:
	/**
	 * @brief 二自旋相互作用的表示.
	 */
	struct SpinInteraction
	{
		/**
		 * @brief 构造一个新的 Spin Interaction 对象并将所有交互初始化为零. 
		 */
		SpinInteraction()
		{
			memset(&interactionStrength[0][0], 0, 9 * sizeof(float));
		};

		float interactionStrength[3][3]; ///< 相互作用强度,编码为interactionStrength[s1][s2],其中 s1 是第一个自旋的 x、y 或 z（0、1 或 2）分量,s2 是第二个自旋的分量. 
	};

	std::vector<std::pair<LatticeIterator, SpinInteraction> > interactions; ///< 模型中的双自旋相互作用列表.所有相互作用都发生在晶格位点对 (s1,s2) 之间,其中 s1 是参考位点,s2 由晶格迭代器指定. 
	std::vector<std::string> interactionParameters; ///< 任务文件中使用的字符串形式交互参数的列表.列表的顺序对应于 SpinModel::interactions 中的顺序. 
};