#pragma once
#ifndef _BS_TREE_H_
#define _BS_TREE_H_

namespace myTinySTL
{
	// struct of __bs_node
	template<class T>
	struct __bs_node
	{
		typedef T value_type;
		value_type data;
		__bs_node* lchild;
		__bs_node* rchild;
		explicit __bs_node() : data(0), lchild(nullptr), rchild(nullptr) {}
	};

	// class of __bs_iterator
	template<class T>
	class __bs_iterator
	{

	};
}

#endif // !_BS_TREE_H_

