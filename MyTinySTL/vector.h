#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <algorithm>
#include <type_traits>

#include "allocator.h"
#include "algorithm.h"
#include "iterator.h"
#include "reverseIterator.h"
#include "uninitializedfunction.h"
#include "construct.h"

namespace myTinySTL 
{
	//********* vector *************
	template<class T, class Alloc = allocator<T>>
	class vector 
	{
	public:
		// vector的嵌套型别定义
		typedef T									value_type;
		typedef value_type*                         pointer;
		typedef value_type*                         iterator;
		typedef value_type&                         reference;
		typedef size_t								size_type;
		typedef ptrdiff_t	                        difference_type;

		typedef const T*                            const_iterator;
		typedef reverse_iterator_t<T*>				reverse_iterator;
		typedef reverse_iterator_t<const T*>		const_reverse_iterator;
		typedef const T&                            const_reference;
	
	protected:
		typedef allocator<T> data_allocator;
		iterator start; // 表示目前使用空间的头
		iterator finish; // 表示目前使用空间的尾
		iterator end_of_storage; // 表示目前可用空间的尾
	public:
		//构造，拷贝构造，赋值运算符，析构相关函数
		vector():start(0), finish(0), end_of_storage(0) {}
		explicit vector(const size_type n);
		vector(const size_type n, const T& value);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);

		vector(const vector& v);
		vector(vector&& v); // 移动语义

		vector& operator = (const vector& v);
		vector& operator = (vector&& v);

		~vector();

		//比较操作相关
		bool operator == (const vector& v)const;
		bool operator != (const vector& v)const;

		//迭代器相关
		iterator begin() { return start; }
		const_iterator begin()const { return start; }
		const_iterator cbegin()const { return start; }
		iterator end() { return finish; }
		const_iterator end()const { return finish; }
		const_iterator cend()const { return finish; }

		reverse_iterator rbegin() { return reverse_iterator(finish); }
		const_reverse_iterator crbegin()const { return const_reverse_iterator(finish); }
		reverse_iterator rend() { return reverse_iterator(start); }
		const_reverse_iterator crend()const { return const_reverse_iterator(start); }

		//与容量相关
		size_type size()const { return size_type(end() - begin()); }
		size_type capacity()const { return size_type(end_of_storage - begin()); }
		bool empty()const { return begin() == end(); }
		void resize(size_type new_size, const T& x);
		void resize(size_type new_size);
		void reserve(size_type n);

		//访问元素相关
		reference operator[](const size_type n) { return *(begin() + n); }
		const_reference operator[](const size_type n)const { return *(cbegin() + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		pointer data() { return start; }

		//修改容器相关的操作
		//清空容器，销毁容器中的所有对象并使容器的size为0，但不回收容器已有的空间
		void clear();
		void swap(vector& v);
		void push_back(const T& x);
		void pop_back();
		iterator insert(iterator position, const T& val);
		void insert(iterator position, const size_type& n, const T& val);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		//容器的空间配置器相关
		Alloc get_allocator() { return data_allocator; }
	private:
		void destroy_and_deallocate_all();
		void allocate_and_fill_n(const size_type n, const T& value);
		template<class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);

		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void vector_aux(Integer n, const T& value, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void insert_aux(iterator position, Integer n, const T& value, std::true_type);
		template<class InputIterator>
		void reallocate_and_copy(iterator position, InputIterator first, InputIterator last);
		void reallocate_and_fill_n(iterator position, const size_type& n, const T& val);
		size_type getNewCapacity(size_type len)const;
	public:
		template<class T, class Alloc>
		friend bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
		template<class T, class Alloc>
		friend bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
	};// end of class vector
}

#include "Detail\vector.impl.h"

#endif // !_VECTOR_H_
