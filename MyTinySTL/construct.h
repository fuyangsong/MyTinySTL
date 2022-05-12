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
		new(p) T(); // placement new������T:T();
	}

	template<class T1, class T2>
	inline void construct(T1* p, const T2& value) 
	{
		new(p) T1(value); // placement new������T1:T1(value);
	}

	// ������destory()��һ�汾������һ��ָ��
	template<class T>
	inline void destroy(T* pointer) 
	{
		pointer->~T();
	}

	// ���Ԫ�ص���ֵ�ͱ�value type����trivial destroctur...
	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _true_type) {}

	// ���Ԫ�ص���ֵ�ͱ�value type����non-trivial destroctur...
	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _false_type) 
	{
		for (; first < last; ++first) 
		{
			myTinySTL::destroy(&*first);
		}
	}

	// �ж�Ԫ�ص���ֵ�ͱ�(value type)�Ƿ���trivial destructor
	template<class ForwardIterator, class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename _type_traits<T>::has_trivial_destructor trivial_destroctur;
		myTinySTL::_destroy_aux(first, last, trivial_destroctur());
	}

	// ������destory()�ڶ��汾�������������������˺����跨�ҳ�Ԫ�ص���ֵ�ͱ�
	// ��������__type_traits<>��ȡ���ʵ���ʩ
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		myTinySTL::_destroy(first, last, myTinySTL::value_type(first));
	}

}

#endif // !_CONSTRUCT_H_

