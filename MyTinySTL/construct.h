#pragma once
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new>

#include "TypeTraits.h"
#include "iterator.h"

namespace myTinySTL 
{

	template<class T>
	inline void construct(T* p)
	{
		new(p) T(); // placement new；调用T:T();
	}

	template<class T1, class T2>
	inline void construct(T1* p, const T2& value) 
	{
		new(p) T1(value); // placement new；调用T1:T1(value);
	}

	// 以下是destory()第一版本，接受一个指针
	template<class T>
	inline void destroy(T* pointer) 
	{
		pointer->~T();
	}

	// 如果元素的数值型别（value type）有trivial destroctur...
	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _true_type) {}

	// 如果元素的数值型别（value type）有non-trivial destroctur...
	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _false_type) 
	{
		for (; first < last; ++first) 
		{
			myTinySTL::destroy(&*first);
		}
	}

	// 判断元素的数值型别(value type)是否有trivial destructor
	template<class ForwardIterator, class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename _type_traits<T>::has_trivial_destructor trivial_destroctur;
		myTinySTL::_destroy_aux(first, last, trivial_destroctur());
	}

	// 以下是destory()第二版本，接受两个迭代器，此函数设法找出元素的数值型别，
	// 进而利用__type_traits<>求取最适当措施
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		myTinySTL::_destroy(first, last, myTinySTL::value_type(first));
	}

}

#endif // !_CONSTRUCT_H_

