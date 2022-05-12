#pragma once
#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

#include "../vector.h"

namespace myTinySTL 
{
	//***********************构造，复制，析构相关***********************
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n) 
	{
		allocate_and_fill_n(n, value_type());
	}

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const T& value)
	{
		allocate_and_fill_n(n, value);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) 
	{
		//处理指针和数字间的区别的函数
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	vector<T, Alloc>::~vector() 
	{
		destroy_and_deallocate_all();
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last)
	{
		start = data_allocator::allocate(last - first);
		finish = myTinySTL::uninitialized_copy(first, last, start);
		end_of_storage = finish;
	}
	
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector<T, Alloc>& v) 
	{
		allocate_and_copy(v.start, v.finish);
	}

	template<class T, class Alloc>
	vector<T, Alloc>::vector(vector&& v) // 移动语义
	{
		start = v.start;
		finish = v.finish;
		end_of_storage = v.end_of_storage;
		v.start = v.finish = v.end_of_storage = 0;
	}

	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (const vector<T, Alloc>& v) 
	{
		if (this != &v) 
			allocate_and_copy(v.start, v.finish);
		return *this;
	}

	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (vector&& v) 
	{
		if (this != &v) 
		{
			destroy_and_deallocate_all();
			start = v.start;
			finish = v.finish;
			end_of_storage = v.end_of_storage;
			v.start = v.finish = v.end_of_storage = 0;
		}
		return *this;
	}

	//*************和容器的容量相关******************************
	template<class T, class Alloc>
	void vector<T, Alloc>::resize(size_type new_size, const T& x) 
	{
		if (new_size < size()) 
		{
			data_allocator::destroy(start + new_size, finish);
			finish = start + new_size;
		}
		else if (new_size > size() && new_size <= capacity()) 
		{
			auto length_of_insert = new_size - size();
			finish = myTinySTL::uninitialized_fill_n(finish, length_of_insert, x);
		}
		else if (new_size > capacity()) 
		{
			auto length_of_insert = new_size - size();
			T* newStart = data_allocator::allocate(getNewCapacity(length_of_insert));
			T* newFinish = myTinySTL::uninitialized_copy(begin(), end(), newStart);
			newFinish = myTinySTL::uninitialized_fill_n(newFinish, length_of_insert, x);

			destroy_and_deallocate_all();
			start = newStart;
			finish = newFinish;
			end_of_storage = start + new_size;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::resize(size_type new_size)
	{
		resize(new_size, T());
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n)
	{
		if (capacity() < n)
		{
			const auto old_size = size();
			auto tmp = data_allocator::allocate(n);
			myTinySTL::uninitialized_copy(start, finish, tmp);
			destroy_and_deallocate_all();
			start = tmp;
			finish = tmp + old_size;
			end_of_storage = start + n;
		}
	}

	//***************修改容器的相关操作**************************
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
	{
		iterator i = copy(last, finish, first);
		data_allocator::destory(i, finish);
		finish = finish - (last - first);
		return first;
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) 
	{
		if (position + 1 != end())
			copy(position + 1, finish, position);
		--finish;
		myTinySTL::destroy(finish);
		return position;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::reallocate_and_copy(iterator position, InputIterator first, InputIterator last) 
	{
		difference_type newCapacity = getNewCapacity(last - first);

		T* newStart = data_allocator::allocate(newCapacity);
		T* newEndOfStorage = newStart + newCapacity;
		T* newFinish = myTinySTL::uninitialized_copy(begin(), position, newStart);
		newFinish = myTinySTL::uninitialized_copy(first, last, newFinish);
		newFinish = myTinySTL::uninitialized_copy(position, end(), newFinish);

		destroy_and_deallocate_all();
		start = newStart;
		finish = newFinish;
		end_of_storage = newEndOfStorage;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::reallocate_and_fill_n(iterator position, const size_type& n, const T& val) 
	{
		difference_type newCapacity = getNewCapacity(n);

		T* newStart = data_allocator::allocate(newCapacity);
		T* newEndOfStorage = newStart + newCapacity;
		T* newFinish = myTinySTL::uninitialized_copy(begin(), position, newStart);
		newFinish = myTinySTL::uninitialized_fill_n(newFinish, n, val);
		newFinish = myTinySTL::uninitialized_copy(position, end(), newFinish);

		destroy_and_deallocate_all();
		start = newStart;
		finish = newFinish;
		end_of_storage = newEndOfStorage;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position,
		InputIterator first,
		InputIterator last,
		std::false_type) 
	{
		difference_type locationLeft = end_of_storage - finish; // the size of left storage
		difference_type locationNeed = distance(first, last);//last - first;

		if (locationLeft >= locationNeed) 
		{
			if (finish - position > locationNeed) 
			{
				myTinySTL::uninitialized_copy(finish - locationNeed, finish, finish);
				std::copy_backward(position, finish - locationNeed, finish);
				copy(first, last, position);
			}
			else 
			{
				iterator temp = myTinySTL::uninitialized_copy(first + (finish - position), last, finish);
				myTinySTL::uninitialized_copy(position, finish, temp);
				copy(first, first + (finish - position), position);
			}
			finish += locationNeed;
		}
		else 
		{
			reallocate_and_copy(position, first, last);
		}
	}

	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, const T& value, std::true_type) 
	{
		assert(n != 0);
		difference_type locationLeft = end_of_storage - finish; // the size of left storage
		difference_type locationNeed = n;

		if (locationLeft >= locationNeed) 
		{
			auto tempPtr = end() - 1;
			for (; tempPtr - position >= 0; --tempPtr) 
			{//move the [position, finish_) back
				//*(tempPtr + locationNeed) = *tempPtr;//bug
				myTinySTL::construct(tempPtr + locationNeed, *tempPtr);
			}
			myTinySTL::uninitialized_fill_n(position, n, value);
			finish += locationNeed;
		}
		else 
		{
			reallocate_and_fill_n(position, n, value);
		}
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) 
	{
		insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, const size_type& n, const T& val) 
	{
		insert_aux(position, n, val, typename std::is_integral<size_type>::type());
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const T& val) 
	{
		const auto index = position - begin();
		insert(position, 1, val);
		return begin() + index;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& value) 
	{
		if (finish != end_of_storage)
		{
			data_allocator::construct(finish, value);
			++finish;
		}
		else
			insert(end(), value);
	}
	//***********逻辑比较操作相关*******************
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator == (const vector& v)const 
	{
		if (size() != v.size()) 
		{
			return false;
		}
		else 
		{
			auto ptr1 = start;
			auto ptr2 = v.start;
			for (; ptr1 != finish && ptr2 != v.finish; ++ptr1, ++ptr2)
			{
				if (*ptr1 != *ptr2)
					return false;
			}
			return true;
		}
	}

	template<class T, class Alloc>
	bool vector<T, Alloc>::operator != (const vector& v)const 
	{
		return !(*this == v);
	}

	template<class T, class Alloc>
	bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) 
	{
		//return v1 == v2;
		return v1.operator==(v2);
	}

	template<class T, class Alloc>
	bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) 
	{
		return !(v1 == v2);
	}


	template<class T, class Alloc>
	void vector<T, Alloc>::clear() {
		data_allocator::destroy(start, finish);
		finish = start;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector& v) 
	{
		if (this != &v) 
		{
			myTinySTL::swap(start, v.start);
			myTinySTL::swap(finish, v.finish);
			myTinySTL::swap(end_of_storage, v.end_of_storage);
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back() 
	{
		--finish;
		data_allocator::destroy(finish);
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::destroy_and_deallocate_all() 
	{
		if (capacity() != 0) 
		{
			data_allocator::destroy(start, finish);
			data_allocator::deallocate(start, capacity());
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::allocate_and_fill_n(const size_type n, const T& value) 
	{
		start = data_allocator::allocate(n);
		myTinySTL::uninitialized_fill_n(start, n, value);
		finish = start + n;
		end_of_storage = finish;
	}


	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) 
	{
		allocate_and_copy(first, last);
	}

	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const T& value, std::true_type) 
	{
		allocate_and_fill_n(n, value);
	}

	//****************** [getNewCapacity] ********************************
	// 更新容量：如果旧容量等于0，分配所要求的len大小容量
	//           如果旧容量不等于0，所需容量len与旧容量进行比较
	template<class T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const 
	{
		size_type oldCapacity = end_of_storage - start;
		auto res = myTinySTL::max(oldCapacity, len);
		size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);
		return newCapacity;
	}
}

#endif