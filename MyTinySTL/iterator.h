#pragma once
#include <cstddef>
#ifndef _ITERATOR_H_
#define _ITERATOR_H_

namespace myTinySTL 
{
	// 五个作为标记用的型别
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};

	template <class T, class Distance> 
	struct input_iterator
	{
		typedef input_iterator_tag	iterator_category;
		typedef T					value_type;
		typedef Distance			difference_type;
		typedef T*                  pointer;
		typedef T&                  reference;
	};
	struct output_iterator
	{
		typedef output_iterator_tag iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;
	};
	template <class T, class Distance> 
	struct forward_iterator
	{
		typedef forward_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*                      pointer;
		typedef T&                      reference;
	};
	template <class T, class Distance> 
	struct bidirectional_iterator
	{
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	template <class T, class Distance> 
	struct random_access_iterator
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};

	// 为避免写代码时挂一漏万，自行开发的迭代器最好继承自下面这个iterator
	template<class Category, class T, class Distance = ptrdiff_t,
		     class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	// “榨汁机”traits
	template<class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference 			reference;
	};

	// 针对原生指针而设计的traits偏特化版
	template<class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag 	iterator_category;
		typedef T 							value_type;
		typedef ptrdiff_t 					difference_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag 	iterator_category;
		typedef T 							value_type;
		typedef ptrdiff_t 					difference_type;
		typedef const T*                    pointer;
		typedef const T&                    reference;
	};

	// 这个函数可以很方便地决定某个迭代器的类型(category)
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator& It) 
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	// 这个函数可以很方便地决定某个迭代器的value type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) 
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	// 这个函数可以很方便地决定某个迭代器的distance type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	difference_type(const Iterator& It) 
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}
}

#endif // !_ITERATOR_H_

