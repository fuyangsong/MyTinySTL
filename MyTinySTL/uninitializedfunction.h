#pragma once
#ifndef _UNINITIALIZED_FUNCTION_H_
#define _UNINITIALIZED_FUNCTION_H_

#include "algorithm.h"
#include "construct.h"
#include "iterator.h"
#include "TypeTraits.h"

namespace myTinySTL 
{

	//******************** [uninitialized_copy] ******************************************
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type) 
	{
		return myTinySTL::copy(first, last, result);
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type) 
	{
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur) 
			construct(&*cur, *first);
		return cur;
	}

	template<class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result, T*)
	{
		typedef typename myTinySTL::_type_traits<T>::is_POD_type is_POD;
		return _uninitialized_copy_aux(first, last, result, is_POD());
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, 
		ForwardIterator result) 
	{
		return _uninitialized_copy(first, last, result, myTinySTL::value_type(result));
	}
	
	template<>
	inline char* uninitialized_copy(const char* first, const char* last, char* result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	template<>
	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

	//******************* [ uninitialized_fill] **************************************
	template<class ForwardIterator, class T>
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, _true_type) 
	{
		fill(first, last, x);
	}

	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& x, _false_type) 
	{
		ForwardIterator cur = first;
		for (; cur != last; ++cur) 
		{
			construct(&*cur, x);
		}
	}

	template<class ForwardIterator, class T, class T1>
	inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x, T1*)
	{
		typedef typename myTinySTL::_type_traits<T1>::is_POD_type is_POD;
		_uninitialized_fill_aux(first, last, x, is_POD());
	}

	template<class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) 
	{
		_uninitialized_fill(first, last, x, myTinySTL::value_type(first));
	}
	

	//************************* [uninitialized_fill_n] ***************************************
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T& x, _true_type) 
	{
		return myTinySTL::fill_n(first, n, x);
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T& x, _false_type) 
	{
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur) 
			construct(&*cur, x);
		return cur;
	}

	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator _uninitialized_fill_n(ForwardIterator first,
		Size n, const T& x, T1*)
	{
		typedef typename myTinySTL::_type_traits<ForwardIterator>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
		Size n, const T& x) 
	{
		return _uninitialized_fill_n(first, n, x, myTinySTL::value_type(first));
	}
	
}

#endif // !_UNINITIALIZED_FUNCTION_H_

