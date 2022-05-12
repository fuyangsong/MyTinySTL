#pragma once
#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE

#include "vector.h"
#include "functional.h"

namespace myTinySTL
{
	template <class T, class Sequence = myTinySTL::vector<T>,
		class Compare = myTinySTL::less<typename Sequence::value_type>>
	class priority_queue
	{
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
	protected:
		Sequence c; // 底层容器
		Compare comp; // 元素大小比较标准
	public:
		priority_queue(): c() {}
		explicit priority_queue(const Compare& x) : c(), comp(x) {}
		explicit priority_queue(size_type n) : c(n)
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}
		priority_queue(size_type n, const value_type& value) : c(n, value)
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& x)
			: c(first, last), comp(x)
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			: c(first, last)
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}
		priority_queue(const Sequence& s) : c(s)
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}
		priority_queue(Sequence&& s) : c(std::move(s))
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}

		priority_queue(const priority_queue& rhs) : c(rhs.c), comp(rhs.comp)
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}
		priority_queue(priority_queue&& rhs) : c(std::move(rhs.c)), comp(rhs.comp)
		{
			myTinySTL::make_heap(c.begin(), c.end(), comp);
		}

		priority_queue& operator=(const priority_queue& rhs)
		{
			c = rhs.c;
			comp = rhs.comp;
			myTinySTL::make_heap(c.begin(), c.end(), comp);
			return *this;
		}
		priority_queue& operator=(priority_queue&& rhs)
		{
			c = std::move(rhs.c);
			comp = rhs.comp;
			myTinySTL::make_heap(c.begin(), c.end(), comp);
			return *this;
		}
	public:
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		const_reference top() { return c.front(); }
		void push(const value_type& x)
		{
			try
			{
				c.push_back(x);
				myTinySTL::push_heap(c.begin(), c.end(), comp);
			}
			catch (...)
			{
				c.clear();
			}
			
		}
		void pop()
		{
			try
			{
				myTinySTL::pop_heap(c.begin(), c.end(), comp);
				c.pop_back();
			}
			catch (...)
			{
				c.clear();
			}
		}

		void clear()
		{
			while (!empty())
				pop();
		}

		void swap(priority_queue& rhs)
		{
			myTinySTL::swap(c, rhs.c);
			myTinySTL::swap(comp, rhs.comp);
		}
	};

	// 重载swap
	template <class T, class Sequence, class Compare>
	void swap(priority_queue<T, Sequence, Compare>& lhs, priority_queue<T, Sequence, Compare>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif // !_PRIORITY_QUEUE
