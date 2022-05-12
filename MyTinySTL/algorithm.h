#pragma once
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <cstring>
#include <utility>
#include <iostream>

#include "allocator.h"
#include "functional.h"
#include "iterator.h"
#include "TypeTraits.h"
#include "utility.h"

namespace myTinySTL
{
	//********* [fill] ********************
	//********* [Algorithm Complexity: O(N)] ****************
	// ��[first,last)�ڵ�����Ԫ�ظ�����ֵ
	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		for (; first != last; first++)
			*first = value;
	}

	inline void fill(char* first, char* last, const char& value)
	{
		memset(first, static_cast<unsigned char>(value), last - first);
	}

	inline void fill(wchar_t* first, wchar_t* last, const wchar_t& value)
	{
		memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(wchar_t));
	}

	//*************** [fill_n] ***********************
	//*************** Algorithm Complexity: O(N) ***************
	// ��[first,last)�ڵ�ǰn��Ԫ�ظ�����ֵ�����صĵ�����ָ����������һ��Ԫ�ص���һ��λ��
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}

	template<class Size>
	char* fill_n(char* first, Size n, const char& value)
	{
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;
	}

	template<class Size>
	wchar_t* fill_n(wchar_t* first, Size n, const wchar_t& value)
	{
		memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
		return first + n;
	}

   //*************** [lower_bound] ************************************
   // ��[first, last)�в��ҵ�һ����С�� value ��Ԫ�ص�λ��
   // ����һ����������ָ���ڷ�Χ�ڵ����������п��Բ���ָ��ֵ�����ƻ�����˳��ĵ�һ��λ��
   // __lower_bound �� forward_iterator_tag �汾
	template <class ForwardIterator, class T, class Distance>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Distance*, forward_iterator_tag)
	{
		auto len = distance(first, last);
		Distance half;
		ForwardIterator middle;
		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			advance(middle, half); // �� middle ָ���м�λ��
			if (*middle < value)
			{ // ����м�λ��Ԫ��ֵС�� value
				first = middle;
				++first;              // �� first ָ�� middle ����һ��λ��
				len = len - half - 1; // ���� len�����Һ���
			}
			else
			{
				len = half; //���� len������ǰ���
			}
		}
		return first;
	}

	// __lower_bound �� random_access_iterator_tag �汾
	template <class RandomAccessIterator, class T, class Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
		const T& value, Distance*, random_access_iterator_tag)
	{
		auto len = last - first;
		Distance half;
		RandomAccessIterator middle;
		while (len > 0)
		{
			half = len >> 1;
			middle = first + half; // �� middle ָ���м�λ��
			if (*middle < value)
			{                         // ����м�λ��Ԫ��ֵС�� value
				first = middle + 1;   // �� first ָ�� middle ����һ��λ��
				len = len - half - 1; // ���� len�����Һ���
			}
			else
			{
				len = half; // ���� len������ǰ���
			}
		}
		return first;
	}

	template <class ForwardIterator, class T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value)
	{
		return myTinySTL::__lower_bound(first, last, value,
			difference_type(first), iterator_category(first));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// __lower_bound �� forward_iterator_tag �汾
	template <class ForwardIterator, class T, class Distance, class Compared>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Distance*,
		forward_iterator_tag,
		Compared comp)
	{
		auto len = distance(first, last);
		Distance half;
		ForwardIterator middle;
		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			advance(middle, half); // �� middle ָ���м�λ��
			if (comp(*middle, value))
			{
				first = middle;
				++first;              // �� first ָ�� middle ����һ��λ��
				len = len - half - 1; // ���� len�����Һ���
			}
			else
			{
				len = half; // ���� len������ǰ���
			}
		}
		return first;
	}

	// __lower_bound �� random_access_iterator_tag �汾
	template <class RandomAccessIterator, class T, class Distance, class Compared>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
		const T& value, Distance*,
		random_access_iterator_tag,
		Compared comp)
	{
		auto len = last - first;
		Distance half;
		RandomAccessIterator middle;
		while (len > 0)
		{
			half = len >> 1;
			middle = first + half; // �� middle ָ���м�λ��
			if (comp(*middle, value))
			{
				first = middle + 1;   // �� first ָ�� middle ����һ��λ��
				len = len - half - 1; // ���� len�����Һ���
			}
			else
			{
				len = half; // ���� len������ǰ���
			}
		}
		return first;
	}

	template <class ForwardIterator, class T, class Compared>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compared comp)
	{
		return myTinySTL::__lower_bound(first, last, value,
			difference_type(first), iterator_category(first), comp);
	}


	//************** [min] ***********************
	//************** Algorithm Complexity: O(1) ************
	// ȡ���������еĽ�Сֵ���������汾���汾һʹ�ö����ͱ�T���ṩ��less-than
	// ���������жϴ�С���汾��ʹ�÷º���comp���жϴ�С
	template<class T>
	inline const T& min(const T& a, const T& b)
	{
		return b < a ? b : a;
	}

	template<class T, class Compare>
	inline const T& min(const T& a, const T& b, Compare comp)
	{
		return comp(b, a) ? b : a; // ��comp��������С�Ƚϡ���׼
	}

	//*************** [max] ************************
	//*************** Algorithm Complexity: O[1] ************
	// ȡ���������еĽϴ�ֵ���������汾���汾һʹ�ö����ͱ����ṩ��greater-than
	// ���������Ƚϴ�С���汾��ʹ�÷º���comp���жϴ�С
	template<class T>
	inline const T& max(const T& a, const T& b)
	{
		return a < b ? b : a;
	}

	template<class T, class Compare>
	inline const T& max(const T& a, const T& b, Compare comp)
	{
		return comp(a, b) ? b : a;
	}

	//*************** [push_heap] *****************************
	//*************** Algorithm Complexity: O(logN) ***************
	// percolate up(����)����
	template<class RandomAccessIterator, class Compare>
	static void up(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp)//1.[first, last], 2.headr points the header of the heap
	{
		if (first != last)
		{
			auto holeIndex = last - head;
			auto parentIndex = (holeIndex - 1) / 2;
			for (auto cur = last; parentIndex >= 0 && cur != head; parentIndex = (holeIndex - 1) / 2)
			{
				auto parent = head + parentIndex;
				if (comp(*parent, *cur))
				{
					myTinySTL::swap(*parent, *cur);
				}
				cur = parent;
				holeIndex = cur - head;
			}

		}
	}

	template <class RandomAccessIterator, class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		myTinySTL::up(first, last - 1, first, comp);
	}

	template <class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last) 
	{
		myTinySTL::push_heap(first, last,
			myTinySTL::less<typename myTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	//*************** [pop_heap] *********************
	//*************** Algorithm Complexity: O(logN)*************
	// percolate down(����)����
	template<class RandomAccessIterator, class Compare>
	static void down(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp) //1.[first, last], 2.headr points the header of the heap
	{
		if (first != last) 
		{
			auto index = first - head;
			auto leftChildIndex = index * 2 + 1;
			for (auto cur = first; leftChildIndex < (last - head + 1) && cur < last; leftChildIndex = index * 2 + 1) 
			{
				auto leftchild = head + leftChildIndex;//get the left child
				if ((leftchild + 1) <= last && *(leftchild + 1) > * leftchild)//cur has a right child
					leftchild = leftchild + 1; // ȡ���ӵĽϴ����븸�ڵ㻥��
				if (comp(*cur, *leftchild))
					myTinySTL::swap(*cur, *leftchild);
				cur = leftchild;
				index = cur - head;
			}
		}
	}

	// ע�⣬pop_heap֮�����Ԫ��ֻ�Ǳ������ڵײ���������β�ˣ���δ��ȡ�ߡ�
	template <class RandomAccessIterator, class Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		myTinySTL::swap(*first, *(last - 1));
		if (last - first >= 2)
			myTinySTL::down(first, last - 2, first, comp);
	}

	template <class RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last) 
	{
		myTinySTL::pop_heap(first, last,
			myTinySTL::less<typename myTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	//********* [sort_heap] ***************
	//********* [Algorithm Complexity: O(N)] ****************
	template <class RandomAccessIterator, class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		for (auto cur = last; cur != first; --cur)
		{
			myTinySTL::pop_heap(first, cur, comp);
		}
	}

	template <class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) 
	{
		return myTinySTL::sort_heap(first, last,
			myTinySTL::less<typename myTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	//************* [make_heap] *************************
	//************* Algorithm Complexity: O(N)*****************
	template <class RandomAccessIterator, class Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) 
	{
		const auto range = last - first; // Ԫ�ظ���
		for (auto cur = first + range / 2 - 1; cur >= first; --cur) 
		{
			myTinySTL::down(cur, last - 1, first, comp);
			if (cur == first) 
				return;
		}
	}

	template <class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last) 
	{
		myTinySTL::make_heap(first, last,
			myTinySTL::less<typename myTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	//********** [for_each] *************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ���º���fʩ����[first,last)�����ڵ�ÿһ��Ԫ�����ϡ�f���ɸı�Ԫ�����ݣ���Ϊfirst��last����InputIterator,
	// ����֤���ܸ�ֵ��Ϊ��
	template <class InputIterator, class Function>
	Function for_each(InputIterator first, InputIterator last, Function f) 
	{
		for (; first != last; ++first)
			f(*first);
		return f;
	}

	//********** [find] *************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ����equality��������ѭ�����[first,last)�ڵ�����Ԫ�أ��ҳ���һ��ƥ�䡰��ͬ(equality)�������ߡ�
	// ����ҵ����ͷ���һ��InputIteratorָ���Ԫ�أ����򷵻ص�����last��
	template <class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& val) {
		for (; first != last; ++first) {
			if (*first == val)
				break;
		}
		return first;
	}

	//********** [find_if] *************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ����ָ����pred����������ѭ�����[first,last)�ڵ�����Ԫ�أ��ҳ���һ����pred������Ϊtrue�ߡ�
	// ����ҵ��ͷ���һ��InputIteratorָ���Ԫ�أ����򷵻ص�����last��
	template<class InputIterator, class Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
	{
		for (; first != last; ++first)
			if (pred(*first))
				break;
		return first;
	}

	//********** [find_end] ******************************
	//********* [Algorithm Complexity: O(N*N)] ****************
	// ������һ[first1, last1)�����ǵ������У��������ж�[first2,last2)�����һ�γ��ֵ㡣
	// �������һ֮�ڲ����ڡ���ȫƥ�����ж����������У��㷵�ص�����last1��
	// ���㷨�������汾���汾һʹ��Ԫ���Ա����ṩ��equality���������汾�������û�ָ��ĳ����Ԫ�������
	// ��Ϊ�ж�Ԫ������������ݡ�
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2) 
	{
		if (first2 == last2)
			return last1;
		else
		{
			ForwardIterator1 result = last1;
			while (first1 != last1)
			{
				ForwardIterator1 iter1 = first1;
				ForwardIterator2 iter2 = first2;
				while (*iter1 == *iter2)
				{
					++iter1;
					++iter2;
					if (iter1 == last1)
						return result;
					if (iter2 == last2)
					{
						result = iter1;
						break;
					}
				}
				++first1;
			}
			return result;
		}
	}

	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred) 
	{
		if (first2 == last2)
			return last1;
		else
		{
			ForwardIterator1 result = last1;
			while (first1 != last1)
			{
				ForwardIterator1 iter1 = first1;
				ForwardIterator2 iter2 = first2;
				while (pred(*iter1, *iter2))
				{
					++iter1;
					++iter2;
					if (iter1 == last1)
						return result;
					if (iter2 == last2)
					{
						result = iter1;
						break;
					}
				}
				++first1;
			}
			return result;
		}
	}

	//********** [find_first_of] ******************************
	//********* [Algorithm Complexity: O(N*N)] ****************
	// ���㷨��[first2,last2)�����ڵ�ĳЩԪ����Ϊ����Ŀ�꣬Ѱ��������[first1,last1)�����ڵ�һ�γ��ֵص㡣
	// ���㷨�������汾���汾һʹ��Ԫ���ͱ����ṩ��equality���������汾�������û�ָ��ĳ����Ԫ�����pred
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2) 
	{
		for (; first1 != last1; ++first1) 
		{
			for (auto it = first2; it != last2; ++it) 
			{
				if (*first1 == *it)
					return first1;
			}
		}
		return last1;
	}

	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred) 
	{
		for (; first1 != last1; ++first1) {
			for (auto it = first2; it != last2; ++it) {
				if (pred(*first1, *it))
					return first1;
			}
		}
		return last1;
	}

	//********** [adjacent_find] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	// �ҳ���һ����������������Ԫ��
	template <class ForwardIterator, class BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred) 
	{
		for (; first != last; ++first) 
		{
			if (first + 1 != last && pred(*(first), *(first + 1)))
				break;
		}
		return first;
	}

	template<class ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
	{
		myTinySTL::adjacent_find(first, last,
			myTinySTL::equal_to<typename myTinySTL::iterator_traits<ForwardIterator>::value_type()>);
	}

	//********** [count] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ����equality���������[first,last)�����ڵ�ÿһ��Ԫ��������ָ��ֵvalue�Ƚϣ�
	// ��������value��ȵ�Ԫ�صĸ�����
	template <class InputIterator, class T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& val) 
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) 
		{
			if (*first == val)
				++n;
		}
		return n;
	}

	//********** [count_if] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ��ָ������predʵʩ��[first,last)�����ڵ�ÿһ��Ԫ�����ϣ����������pred֮������Ϊtrue��
	// ������Ԫ�صĸ�������
	template <class InputIterator, class UnaryPredicate>
	typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, UnaryPredicate pred) 
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) 
		{
			if (pred(*first))
				++n;
		}
		return n;
	}

	//********** [mismatch] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ����ƽ�бȽ��������У�ָ������֮��ĵ�һ����ƥ��㣬����һ�Ե�������
	// �ֱ�ָ�����������еĲ�ƥ��㡣��������е����ж�ӦԪ�ض�ƥ�䣬���صı��������и��Ե�last��������
	// Ҫ�󣺵ڶ�������Ԫ�ظ������ڵ��ڵ�һ�����е�Ԫ�ظ���
	template <class InputIterator1, class InputIterator2>
	myTinySTL::pair<InputIterator1, InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) 
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (*first1 != *first2)
				break;
		}
		return myTinySTL::make_pair(first1, first2);
	}
	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	myTinySTL::pair<InputIterator1, InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, BinaryPredicate pred) 
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (!pred(*first1, *first2))
				break;
		}
		return myTinySTL::make_pair(first1, first2);
	}

	//********** [equal] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	// �������������[first,last)��������ȣ�equal()����true��
	// ����ڶ������е�Ԫ�رȽ϶࣬�������Ԫ�ز��迼��
	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, BinaryPredicate pred) 
	{
		for (; first1 != last1; ++first1, ++first2) 
		{
			if (!pred(*first1, *first2))
				return false;
		}
		return true;
	}

	template <class InputIterator1, class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2) 
	{
		return myTinySTL::equal(first1, last1, first2, myTinySTL::equal_to<typename myTinySTL::iterator_traits<InputIterator1>::value_type>());
	}
	
	//********** [search] ******************************
	//********* [Algorithm Complexity: O(N*N)] ****************
	// ������һ[first1,last1)�����ǵ������У��������ж�[first2,last2)���״γ��ֵ㡣
	// �������һ�ڲ����������ж���ȫƥ��������У��㷵�ص�����last1��
	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred) 
	{
		while (first1 != last1) 
		{
			auto it1 = first1;
			auto it2 = first2;
			while (it1 != last1 && it2 != last2) 
			{
				if (pred(*it1, *it2)) 
				{
					++it1;
					++it2;
				}
				else 
				{
					break;
				}
			}
			if (it2 == last2)
				return first1;
			if (it1 == last1)
				return last1;
			++first1;
		}
		return last1;
	}

	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2) 
	{
		return myTinySTL::search(first1, last1, first2, last2,
			myTinySTL::equal_to<typename myTinySTL::iterator_traits<ForwardIterator1>::value_type>());
	}
	
	//********** [sort] ******************************
	//********* [Algorithm Complexity: O(NlogN)] ****************
	namespace 
	{
		template<class RandomAccessIterator, class BinaryPredicate>
		typename iterator_traits<RandomAccessIterator>::value_type
			mid3(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate pred) 
		{//[first, last]
			auto mid = first + (last + 1 - first) / 2;
			if (pred(*mid, *first)) 
			{
				swap(*mid, *first);
			}
			if (pred(*last, *mid)) 
			{
				swap(*last, *mid);
			}
			if (pred(*last, *first)) 
			{
				swap(*last, *first);
			}
			auto ret = *mid;
			swap(*mid, *(last - 1));//��mid item��λ��Ϊ�ڱ�
			return ret;
		}

		template<class RandomAccessIterator, class BinaryPredicate>
		void bubble_sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate pred) 
		{
			auto len = last - first;
			for (auto i = len; i != 1; --i) 
			{
				bool swaped = false;
				for (auto p = first; p != (first + i - 1); ++p) 
				{
					if (pred(*(p + 1), *p)) 
					{
						myTinySTL::swap(*(p + 1), *p);
						swaped = true;
					}
				}
				if (!swaped)
					break;
			}
		}
	}

	template<class RandomAccessIterator, class BinaryPredicate>
	void sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate pred)
	{
		if (first >= last || first + 1 == last) // 0��Ԫ�ػ���1��Ԫ��
			return;
		if (last - first <= 20)//���䳤��С�ڵ���20�Ĳ���ð���������
			return bubble_sort(first, last, pred);

		auto mid = mid3(first, last - 1, pred);
		auto p1 = first, p2 = last - 2;
		while (p1 < p2)
		{
			while (pred(*p1, mid) && (p1 < p2)) ++p1;
			while (!pred(*p2, mid) && (p1 < p2)) --p2;
			if (p1 < p2)
			{
				swap(*p1, *p2);
			}
		}
		swap(*p1, *(last - 2));//����Ϊ�ڱ���mid item����ԭ����λ��
		sort(first, p1, pred);
		sort(p1 + 1, last, pred);
	}

	template<class RandomAccessIterator>
	void sort(RandomAccessIterator first, RandomAccessIterator last) 
	{
		return sort(first, last, 
			myTinySTL::less<typename myTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}

	//********** [generate] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ���º���func����������д��[first,last)�����ڵ�����Ԫ�����ϡ�
	template<class ForwardIterator, class Function>
	void generate(ForwardIterator first, ForwardIterator last, Function func) 
	{
		for (; first != last; ++first) 
		{
			*first = func();
		}
	}

	//********** [generate_n] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	// ���º���gen����������д�ڴӵ�����first��ʼ��n��Ԫ�����ϡ�
	template <class OutputIterator, class Size, class Generator>
	OutputIterator generate_n(OutputIterator first, Size n, Generator gen) 
	{
		for (; n > 0; --n, ++first)
			*first = gen();
		return first;
	}

	//********** [distance] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	template<class InputIterator>
	typename myTinySTL::iterator_traits<InputIterator>::difference_type
		_distance(InputIterator first, InputIterator last, input_iterator_tag) 
	{
		typename myTinySTL::iterator_traits<InputIterator>::difference_type dist = 0;
		while (first != last) 
		{
			++first;
			++dist;
		}
		return dist;
	}

	template<class RandomAccessIterator>
	typename myTinySTL::iterator_traits<RandomAccessIterator>::difference_type
		_distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) 
	{
		auto dist = last - first;
		return dist;
	}

	template<class Iterator>
	typename myTinySTL::iterator_traits<Iterator>::difference_type
		distance(Iterator first, Iterator last) 
	{
		return _distance(first, last, iterator_category(first));
	}

	//************** [advance] ***********************
	// �õ�����ǰ��n������
	template <class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n)
	{
		__advance(i, n, iterator_category(i));
	}

	// advance �� input_iterator_tag �İ汾
	template <class InputIterator, class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag)
	{
		while (n--)
			++i;
	}

	// advance �� bidirectional_iterator_tag �İ汾
	template <class BidirectionalIterator, class Distance>
	inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
			while (n--)
				++i;
		else
			while (n++)
				--i;
	}

	// advance �� random_access_iterator_tag �İ汾
	template <class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	//********** [copy] ******************************
	//********* [Algorithm Complexity: O(N)] ****************
	template<class InputIterator, class OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag)
	{
		// std::cout << "__copy(input_iterator_tag)" << std::endl;
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}

	template<class RandomAccessIterator, class OutputIterator, class Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance*)
	{
		// std::cout << "__copy_d()" << std::endl;
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
			*result = *first;
		return result;
	}

	template<class RandomAccessIterator, class OutputIterator>
	inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, random_access_iterator_tag)
	{
		// std::cout << "__copy(random_access_iterator_tag)" << std::endl;
		return __copy_d(first, last, result, myTinySTL::difference_type(first));
	}

	template<class InputIterator, class OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator() (InputIterator first, InputIterator last,
			OutputIterator result)
		{
			// std::cout << "_copy_dispatch()" << std::endl;
			return __copy(first, last, result, myTinySTL::iterator_category(first));
		}
	};

	template<class T>
	inline T* __copy_t(const T* first, const T* last, T* result, _true_type)
	{
		// std::cout << "__copy_t(_true_type)" << std::endl;
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	template<class T>
	inline T* __copy_t(const T* first, const T* last, T* result, _false_type)
	{
		// std::cout << "__copy_t(_false_type)" << std::endl;
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

	template<class T>
	struct __copy_dispatch<T*, T*>
	{
		T* operator() (T* first, T* last, T* result)
		{
			// std::cout << "__copy_dispatch(T*, T*)" << std::endl;
			typedef typename myTinySTL::_type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	template<class T>
	struct __copy_dispatch<const T*, T*>
	{
		T* operator() (const T* first, const T* last, T* result)
		{
			// std::cout << "__copy_dispatch(const T*, T*)" << std::endl;
			typedef typename myTinySTL::_type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	template <class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) 
	{
		// std::cout << "copy()" << std::endl;
		return __copy_dispatch<InputIterator, OutputIterator>()
			(first, last, result);
	}

	template<>
	inline char* copy(const char* first, const char* last, char* result) 
	{
		// std::cout << "copy(const char*)" << std::endl;
		auto dist = last - first;
		memmove(result, first, dist);
		return result + dist;
	}

	template<>
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) 
	{
		// std::cout << "copy(const wchar_t*)" << std::endl;
		auto dist = last - first;
		memmove(result, first, sizeof(wchar_t) * dist);
		return result + dist;
	}
}
	

#endif // !_ALGORITHM_H_

