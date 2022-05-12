#pragma once
#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "allocator.h"
#include "iterator.h"
#include "TypeTraits.h"
#include "reverseiterator.h"
#include "utility.h"
#include "uninitializedfunction.h"
#include "algorithm.h"

namespace myTinySTL 
{
	inline size_t _deque_buf_size(size_t n, size_t sz)
	{
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}
	//class of deque iterator
	template<class T, class Ref, class Ptr, size_t BufSiz>
	struct _deque_iterator :public iterator<random_access_iterator_tag, T> 
	{
		typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
		typedef _deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
		typedef _deque_iterator self;

		static size_t buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }

		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef Ptr                         pointer;
		typedef Ref                         reference;
		typedef size_t                      size_type;
		typedef T**                         map_pointer;

		

		// 保持与容器的联结
		T* cur; // 指向所在缓冲区当前元素
		T* first; // 指向所在缓冲区的头部
		T* last; // 指向所在缓冲区的尾部
		map_pointer node; // 缓冲区所在节点

		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + buffer_size();
		}

		reference operator*() const { return *cur; }
		pointer operator->() const { &(operator*()); }
		difference_type operator-(const self& x) const
		{
			return difference_type(buffer_size()) * (node - x.node - 1)
				+ (cur - first) + (x.last - x.cur);
		}
		self& operator++()
		{
			++cur;
			if (cur == last)
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++* this;
			return tmp;
		}
		self& operator--()
		{
			if (cur == first)
			{
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--* this;
			return tmp;
		}
		self& operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				cur += n;
			else
			{
				difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset * difference_type(buffer_size()));
			}
			return *this;
		}
		self operator+(difference_type n) const
		{
			self tmp = *this;
			return tmp += n;
		}
		self& operator-=(difference_type n) { return *this += -n; }
		self operator-(difference_type n) const
		{
			self tmp = *this;
			return tmp -= n;
		}
		reference operator[](difference_type n) const { return *(*this + n); }
		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return !(*this == x); }
		bool operator<(const self& x) const
		{
			return (node == x.node) ? (cur < x.cur) : (node < x.node);
		}
		bool operator>(const self& x) const { return x < *this; }
		bool operator<=(const self& x) const { return !(x < *this); }
		bool operator>=(const self& x) const { return !(*this < x); }

		// 默认构造、初始化构造、拷贝构造、移动构造、赋值操作符
		_deque_iterator() :cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
		_deque_iterator(pointer v, map_pointer n) :cur(v), first(*n), last(*n + buffer_size()), node(n) {}
		_deque_iterator(const self& it)
			:cur(it.cur), first(it.first), last(it.last), node(it.node) {}
		_deque_iterator(self&& it)
			:cur(it.cur), first(it.first), last(it.last), node(it.node)
		{
			it.cur = nullptr;
			it.first = nullptr;
			it.last = nullptr;
			it.node = nullptr;
		}

		self& operator=(const iterator& rhs)
		{
			if (this != &rhs)
			{
				cur = rhs.cur;
				first = rhs.first;
				last = rhs.last;
				node = rhs.node;
			}
			return *this;
		}

		self& operator=(iterator&& rhs)
		{
			if (this != &rhs)
			{
				cur = rhs.cur;
				first = rhs.first;
				last = rhs.last;
				node = rhs.node;

				rhs.cur = nullptr;
				rhs.first = nullptr;
				rhs.last = nullptr;
				rhs.node = nullptr;
			}
			return *this;
		}
	};

	
	//class of deque
	template<class T, class Alloc = allocator<T>, size_t BufSiz = 0>
	class deque 
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef size_t size_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef ptrdiff_t difference_type;
	public:
		typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
		typedef _deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
		typedef myTinySTL::reverse_iterator_t<iterator> reverse_iterator;
		typedef myTinySTL::reverse_iterator_t<const_iterator> const_reverse_iterator;
	protected:
		typedef pointer* map_pointer;
		iterator start; // 表示第一个节点
		iterator finish; // 表示最后一个节点
		map_pointer map; // 指向map，map是块连续空间，其每个元素都是一个指针，指向一个缓冲区
		size_type map_size; // map内有多少指针

		enum
		{
			_initial_map_size = 8
		};
	protected:
		// 专属空间配置器，每次配置一个元素大小
		typedef allocator<value_type> data_allocator;
		// 专属空间配置器，每次配置一个指针大小
		typedef allocator<pointer> map_allocator;

		size_type buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }
	public:
		// 默认构造、初始化构造、拷贝构造、移动构造、赋值操作符、析构
		deque() { fill_initialize(0, value_type()); }
		explicit deque(size_type n) { fill_initialize(n, value_type()); }
		deque(size_type n, const value_type& value) { fill_initialize(n, value); }

		template<class InputIterator>
		deque(InputIterator first, InputIterator last);

		deque(const deque& rhs);
		deque(deque&& rhs);

		deque& operator=(const deque& rhs);
		deque& operator=(deque&& rhs);

		~deque();
	public:
		// 迭代器相关
		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end() { return finish; }
		const_iterator end() const { return finish; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		// 访问元素相关
		reference operator[] (size_type n) { return start[difference_type(n)]; }
		reference front() { return *start; }
		const_reference front() const { return *start; }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(end() - 1); }

		// 容量相关
		size_type size() const { return finish - start;; }
		size_type max_size() const { return static_cast<size_type>(-1); }
		bool empty() const { return finish == start; }
	public:
		void push_back(const value_type& value);
		void push_front(const value_type& value);
		void pop_back();
		void pop_front();
		void clear();
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		iterator insert(iterator position, const value_type& value);
		iterator insert(iterator position);
		void insert(iterator position, size_type n, const value_type& value);
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		void resize(size_type new_size, const value_type& value);
		void resize(size_type new_size);
		void swap(deque& rhs);
	private:
		void fill_initialize(size_type n, const value_type& value);
		void create_map(size_type num_elements);
		void creat_nodes(map_pointer nstart, map_pointer nfinish);
		void destroy_nodes(map_pointer nstart, map_pointer nfinish);

		template <class InputIterator>
		void range_initialize(InputIterator, InputIterator, input_iterator_tag);
		template <class ForwardIterator>
		void range_initialize(ForwardIterator, ForwardIterator, forward_iterator_tag);
		template <class Integer>
		void initialize_dispatch(Integer n, Integer value, _true_type);
		template <class InputIterator>
		void initialize_dispatch(InputIterator first, InputIterator last, _false_type);
		void fill_insert(iterator position, size_type n, const value_type& x);
		void fill_insert_aux(iterator position, size_type n, const value_type& x);
		template <class ForwardIterator>
		void fill_insert_aux(iterator, ForwardIterator, ForwardIterator, size_type);
		template <class Integer>
		void insert_dispatch(iterator, Integer, Integer, _true_type);
		template <class InputIterator>
		void insert_dispatch(iterator, InputIterator, InputIterator, _false_type);
		template <class InputIterator>
		void insert_dispatch_aux(iterator, InputIterator, InputIterator, input_iterator_tag);
		template <class ForwardIterator>
		void insert_dispatch_aux(iterator, ForwardIterator, ForwardIterator, forward_iterator_tag);
		
		iterator reserve_elements_at_back(size_type n);
		iterator reserve_elements_at_front(size_type n);

		void push_back_aux(const value_type& t);
		void push_front_aux(const value_type& t);
		void reserve_map_at_back(size_type nodes_to_add = 1);
		void reserve_map_at_front(size_type nodes_to_add = 1);
		void reallocate_map(size_type nodes_to_add, bool add_at_front);
		void pop_front_aux();
		void pop_back_aux();
		iterator insert_aux(iterator pos, const value_type& x);
	};//end of deque

	//****************** [deque]***************************************

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::creat_nodes(map_pointer nstart, map_pointer nfinish)
	{
		map_pointer cur;
		try
		{
			for (cur = nstart; cur <= nfinish; ++cur)
				*cur = data_allocator::allocate(buffer_size());
		}
		catch (...)
		{
			destroy_nodes(nstart, nfinish);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::destroy_nodes(map_pointer nstart, map_pointer nfinish)
	{
		for (map_pointer n = nstart; n != nfinish; ++n)
			data_allocator::deallocate(*n, buffer_size());
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::create_map(size_type num_elements)
	{
		// 需要的节点数=(元素个数/每个缓冲区可容纳的元素个数)+1
		// 如果刚好整除，会多配一个节点
		size_type num_nodes = num_elements / buffer_size() + 1;
		// 一个map要管理几个节点。最少8个，最多是"所需节点数加2"
		// (前后各预留一个，扩充时可用)
		map_size = myTinySTL::max(static_cast<size_type>(_initial_map_size), num_nodes + 2);
		map = map_allocator::allocate(map_size);
		// 以上配置出一个"具有map_size个节点"的map

		// 以下令nstart和nfinish指向map所拥有之全部节点的最中央区段
		// 保持在最中央，可使头尾两端的扩充能量一样大。每个节点可对应一个缓冲区
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		try
		{
			creat_nodes(nstart, nfinish);
		}
		catch (...)
		{
			map_allocator::deallocate(map, map_size);
			map = nullptr;
			map_size = 0;
		}
		
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size();
	}

	// deque的构造与内存管理
	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type& value)
	{
		create_map(n);
		map_pointer cur;
		try
		{
			for (cur = start.node; cur < finish.node; ++cur)
				myTinySTL::uninitialized_fill(*cur, *cur + buffer_size(), value);
			myTinySTL::uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...)
		{
			myTinySTL::destroy(start, iterator(*cur, cur));
		}

	}

	template<class T, class Alloc, size_t BufSiz>
	template<class InputIterator>
	deque<T, Alloc, BufSiz>::deque(InputIterator first, InputIterator last)
	{
		typedef typename _is_integer<InputIterator>::is_integer Integer;
		initialize_dispatch(first, last, Integer());
	}

	// 复制构造函数
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::deque(const deque& rhs)
	{
		create_map(rhs.size());
		myTinySTL::uninitialized_copy(rhs.begin(), rhs.end(), start);
	}

	// 移动构造函数
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::deque(deque&& rhs)
		: start(std::move(rhs.start)), finish(std::move(rhs.finish)),
		map(rhs.map), map_size(rhs.map_size)
	{
		rhs.map = nullptr;
		rhs.map_size = 0;
	}

	// 复制赋值运算符
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>& deque<T, Alloc, BufSiz>::operator=(const deque& rhs)
	{
		if (this != &rhs)
		{
			const auto len = size();
			if (len >= rhs.size())
			{
				erase(myTinySTL::copy(rhs.begin(), rhs.end(), start), finish);
			}
			else
			{
				const_iterator mid = rhs.begin() + static_cast<difference_type>(len);
				myTinySTL::copy(rhs.begin(), mid, start);
				insert(finish, mid, rhs.end());
			}
		}
		return *this;
	}

	// 移动赋值运算符
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>& deque<T, Alloc, BufSiz>::operator=(deque&& rhs)
	{
		if (this != &rhs)
		{
			clear();

			start = rhs.start;
			finish = rhs.finish;
			map = rhs.map;
			map_size = rhs.map_size;

			rhs.start = nullptr;
			rhs.finish = nullptr;
			rhs.map = nullptr;
			rhs.map_size = 0;
		}
		return *this;
	}

	// 析构函数
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::~deque()
	{
		if (map)
		{
			destroy_nodes(start.node, finish.node + 1);
			map_allocator::deallocate(map, map_size);
			map = nullptr;
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert(iterator position,
		const value_type& value)
	{
		if (position.cur == start.cur)
		{
			push_front(value);
			return start;
		}
		else if (position.cur == finish.cur)
		{
			push_back(value);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(position, value);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert(iterator position)
	{
		return insert(position, value_type());
	}

	// 在 position 位置插入 n 个元素
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::insert(iterator position, size_type n, const value_type& value)
	{
		fill_insert(position, n, value);
	}

	// 在 position 位置插入[first, last)内的元素
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::insert(iterator position, InputIterator first, InputIterator last)
	{
		typedef typename _is_integer<InputIterator>::is_integer Integer;
		insert_dispatch(position, first, last, Integer());
	}

	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::erase(iterator position)
	{
		iterator next = position;
		++next;
		difference_type index = position - start; // 清除点之前的元素个数
		if (index < (size() >> 1))
		{
			// 如果清除点之前的元素比较少
			std::copy_backward(start, position, next); // 移动清除点之前的元素
			pop_front(); // 移动完毕，最前一个元素冗余，去除之
		}
		else
		{
			// 清除点之后的元素比较少
			myTinySTL::copy(next, finish, position);
			pop_back();
		}
		return start + index;
	}

	// 清除[firast, last)区间内的元素
	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::erase(iterator first, iterator last)
	{
		if (first == start && last == finish)
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first; // 清除区间的长度
			difference_type elems_before = first - start; // 清除区间前方的元素个数
			if (elems_before < (size() - n) / 2)
			{
				std::copy_backward(start, first, last);
				iterator new_start = start + n;
				myTinySTL::destroy(start, new_start);
				destroy_nodes(start.node, new_start.node);
				start = new_start;
			}
			else
			{
				myTinySTL::copy(last, finish, first);
				iterator new_finish = finish - n;
				myTinySTL::destroy(new_finish, finish);
				destroy_nodes(new_finish.node + 1, finish.node + 1);
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	// 请注意，deque的最初状态(无任何元素)保有一个缓冲区，
	// 因此，clear()完成之后回复初始状态，也一样要保留一个缓冲区。
	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::clear()
	{
		// 以下针对头尾以外的每一个缓冲区。
		for (map_pointer node = start.node + 1; node < finish.node; ++node)
		{
			myTinySTL::destroy(*node, *node + buffer_size());
			data_allocator::deallocate(*node, buffer_size());
		}
		if (start.node != finish.node)// 至少有头尾缓冲区
		{
			myTinySTL::destroy(start.cur, start.last);
			myTinySTL::destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else // 只有一个缓冲区
		{
			myTinySTL::destroy(start.cur, finish.cur);
		}
		finish = start;
	}

	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert_aux(iterator position,
		const value_type& value)
	{
		difference_type index = position - start;
		value_type value_copy = value;
		if (index < size() / 2)
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			position = start + index;
			iterator pos1 = position;
			++pos1;
			myTinySTL::copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			position = start + index;
			std::copy_backward(position, back2, back1);
		}
		*position = value_copy;
		return position;
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_back(const value_type& value)
	{
		if (finish.cur != finish.last - 1)
		{
			// 最后缓冲区尚有两个(含)以上的元素备用空间
			construct(finish.cur, value);
			++finish.cur;
		}
		else // 最后缓冲区只剩一个元素备用空间
		{
			push_back_aux(value);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_back_aux(const value_type& value)
	{
		value_type value_copy = value;
		reserve_map_at_back(1);
		*(finish.node + 1) = data_allocator::allocate(buffer_size());
		try
		{
			myTinySTL::construct(finish.cur, value_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...)
		{
			data_allocator::deallocate(*(finish.node + 1));
		}
		
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_front(const value_type& value)
	{
		if (start.cur != start.first)
		{
			myTinySTL::construct(start.cur - 1, value);
			--start.cur;
		}
		else
		{
			push_front_aux(value);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_front_aux(const value_type& value)
	{
		value_type value_copy = value;
		reserve_map_at_front(1);
		*(start.node - 1) = data_allocator::allocate(buffer_size());
		try
		{
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, value_copy);
		}
		catch (...)
		{
			++start;
			data_allocator::deallocate(*(start.node - 1));
		}
		
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::reserve_map_at_back(size_type nodes_to_add)
	{
		if (nodes_to_add + 1 > map_size - (finish.node - map))
		{
			// 如果map尾端的节点备用空间不足
			// 符合以上条件则必须重换一个map(配置更大的，拷贝原来的，释放原来的)
			reallocate_map(nodes_to_add, false);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::reserve_map_at_front(size_type nodes_to_add)
	{
		if (nodes_to_add > start.node - map)
		{
			// 如果map前端的节点备用空间不足
			// 符合以上条件则必须重换一个map(配置更大的，拷贝原来的，释放原来的)
			reallocate_map(nodes_to_add, true);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes)
		{
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
			{
				myTinySTL::copy(start.node, finish.node + 1, new_nstart);
			}
			else
			{
				std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
			}
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_map_size) / 2
				+ (add_at_front ? nodes_to_add : 0);
			myTinySTL::copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_front()
	{
		if (start.cur != start.last - 1)
		{
			myTinySTL::destroy(start.cur);
			++start.cur;
		}
		else
		{
			pop_front_aux();
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_front_aux()
	{
		myTinySTL::destroy(start.cur);
		data_allocator::deallocate(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_back()
	{
		if (finish.cur != finish.first)
		{
			--finish.cur;
			myTinySTL::destroy(finish.cur);
		}
		else
		{
			pop_back_aux();
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_back_aux()
	{
		data_allocator::deallocate(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		myTinySTL::destroy(finish.cur);
	}

	// 重置容器大小
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::resize(size_type new_size, const value_type& value)
	{
		const auto len = size();
		if (new_size < len)
			erase(start + new_size, finish);
		else
			insert(finish, new_size - len, value);
	}

	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::resize(size_type new_size)
	{
		resize(new_size, value_type());
	}

	// 交换两个 deque 容器
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::swap(deque& rhs)
	{
		myTinySTL::swap(start, rhs.start);
		myTinySTL::swap(finish, rhs.finish);
		myTinySTL::swap(map, rhs.map);
		myTinySTL::swap(map_size, rhs.map_size);
	}

	// range_initialize 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::range_initialize(InputIterator first,
		InputIterator last, input_iterator_tag)
	{
		create_map(0);
		try
		{
			for (; first != last; ++first)
				push_back(*first);
		}
		catch (...)
		{
			clear();
		}
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class ForwardIterator>
	void deque<T, Alloc, BufSiz>::range_initialize(ForwardIterator first,
		ForwardIterator last, forward_iterator_tag)
	{
		auto n = myTinySTL::distance(first, last);
		create_map(n);
		map_pointer cur;
		try
		{
			for (cur = start.node; cur < finish.node; ++cur)
			{
				auto mid = first;
				std::advance(mid, buffer_size());
				myTinySTL::uninitialized_copy(first, mid, *cur);
				first = mid;
			}
			myTinySTL::uninitialized_copy(first, last, finish.first);
		}
		catch (...)
		{
			myTinySTL::destroy(start, iterator(*cur, cur));
		}
	}

	// initialize_dispatch 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class Integer>
	void deque<T, Alloc, BufSiz>::initialize_dispatch(Integer n, Integer value, _true_type)
	{
		fill_initialize(n, value);
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::initialize_dispatch(InputIterator first,
		InputIterator last, _false_type)
	{
		range_initialize(first, last, iterator_category(first));
	}



	// 重载比较操作符
	template <class T, class Alloc, size_t BufSiz>
	inline bool operator==(const deque<T, Alloc, BufSiz>& lhs, const deque<T, Alloc, BufSiz>& rhs)
	{
		return lhs.size() == rhs.size() && myTinySTL::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <class T, class Alloc, size_t BufSiz>
	inline bool operator!=(const deque<T, Alloc, BufSiz>& lhs, const deque<T, Alloc, BufSiz>& rhs)
	{
		return !(lhs == rhs);
	}

	// 重载swap
	template <class T, class Alloc, size_t BufSiz>
	inline void swap(deque<T, Alloc, BufSiz>& lhs, deque<T, Alloc, BufSiz>& rhs)
	{
		lhs.swap(rhs);
	}

	// fill_insert 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::fill_insert(iterator position, size_type n, const value_type& value)
	{
		if (position.cur == start.cur)
		{
			auto new_start = reserve_elements_at_front(n);
			try
			{
				myTinySTL::uninitialized_fill(new_start, start, value);
				start = new_start;
			}
			catch (...)
			{
				destroy_node(new_start.node, start.node);
			}
		}
		else if (position.cur == finish.cur)
		{
			auto new_finish = reserve_elements_at_back(n);
			try
			{
				myTinySTL::uninitialized_fill(finish, new_finish, value);
				finish = new_finish;
			}
			catch (...)
			{
				destroy_node(finish.node + 1, new_finish.node + 1);
			}
		}
		else
		{
			fill_insert_aux(position, n, value);
		}
	}

	// fill_insert_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::fill_insert_aux(iterator position, size_type n, const value_type& value)
	{
		const auto elems_before = position - start;
		auto len = size();
		auto value_copy = value;
		if (static_cast<size_type>(elems_before) < (len >> 1))
		{
			auto old_start = start;
			auto new_start = reserve_elements_at_front(n);
			position = start + elems_before;
			try
			{
				if (static_cast<size_type>(elems_before) >= n)
				{
					auto start_n = start + n;
					myTinySTL::uninitialized_copy(start, start_n, new_start);
					start = new_start;
					myTinySTL::copy(start_n, position, old_start);
					myTinySTL::fill(position - n, position, value_copy);
				}
				else
				{
					myTinySTL::uninitialized_fill(myTinySTL::
						uninitialized_copy(start, position, new_start),
						start, value_copy);
					start = new_start;
					myTinySTL::fill(old_start, position, value_copy);
				}
			}
			catch (...)
			{
				destroy_node(new_start.node, start.node);
			}
		}
		else
		{
			auto old_finish = finish;
			auto new_finish = reserve_elements_at_back(n);
			const auto elems_after = static_cast<difference_type>(len) - elems_before;
			position = finish - elems_after;
			try
			{
				if (static_cast<size_type>(elems_after) > n)
				{
					auto finish_n = finish - n;
					myTinySTL::uninitialized_copy(finish_n, finish, finish);
					finish = new_finish;
					std::copy_backward(position, finish_n, old_finish);
					myTinySTL::fill(position, position + n, value_copy);
				}
				else
				{
					myTinySTL::uninitialized_fill(finish, position + n, value_copy);
					myTinySTL::uninitialized_copy(position, finish, position + n);
					finish = new_finish;
					myTinySTL::fill(position, old_finish, value_copy);
				}
			}
			catch (...)
			{
				destroy_node(finish.node + 1, new_finish.node + 1);
			}
		}
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class ForwardIterator>
	void deque<T, Alloc, BufSiz>::fill_insert_aux(iterator position,
		ForwardIterator first, ForwardIterator last, size_type n)
	{
		const auto elems_before = position - start;
		auto len = size();
		if (static_cast<size_type>(elems_before) < (len >> 1))
		{
			auto old_start = start;
			auto new_start = reserve_elements_at_front(n);
			position = start + elems_before;
			try
			{
				if (static_cast<size_type>(elems_before) >= n)
				{
					auto start_n = start + n;
					myTinySTL::uninitialized_copy(start, start_n, new_start);
					start = new_start;
					myTinySTL::copy(start_n, position, old_start);
					myTinySTL::copy(first, last, position - n);
				}
				else
				{
					auto mid = first;
					std::advance(mid, static_cast<difference_type>(n) - elems_before);
					myTinySTL::uninitialized_copy(first, mid,
						myTinySTL::uninitialized_copy(start, position, new_start));
					start = new_start;
					myTinySTL::copy(mid, last, old_start);
				}
			}
			catch (...)
			{
				destroy_node(new_start.node, start.node);
			}
		}
		else
		{
			auto old_finish = finish;
			auto new_finish = reserve_elements_at_back(n);
			const auto elems_after = static_cast<difference_type>(len) - elems_before;
			position = finish - elems_after;
			try
			{
				if (static_cast<size_type>(elems_after) > n)
				{
					auto finish_n = finish - n;
					myTinySTL::uninitialized_copy(finish_n, finish, finish);
					finish = new_finish;
					std::copy_backward(position, finish_n, old_finish);
					myTinySTL::copy(first, last, position);
				}
				else
				{
					auto mid = first;
					std:: advance(mid, elems_after);
					myTinySTL::uninitialized_copy(position, finish,
						myTinySTL::uninitialized_copy(mid, last, finish));
					finish = new_finish;
					myTinySTL::copy(first, mid, position);
				}
			}
			catch (...)
			{
				destroy_node(finish.node + 1, new_finish.node + 1);
			}
		}
	}

	// __insert_dispatch 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class Integer>
	void deque<T, Alloc, BufSiz>::insert_dispatch(iterator position,
		Integer n, Integer x, _true_type)
	{
		fill_insert(position, n, x);
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::insert_dispatch(iterator position,
		InputIterator first, InputIterator last, _false_type)
	{
		insert_dispatch_aux(position, first, last, iterator_category(first));
	}

	// insert_dispatch_aux 函数
	template <class T, class Alloc, size_t BufSiz>
	template <class InputIterator>
	void deque<T, Alloc, BufSiz>::insert_dispatch_aux(iterator position,
		InputIterator first, InputIterator last, input_iterator_tag)
	{
		myTinySTL::copy(first, last, iterator(*this, position));
	}

	template <class T, class Alloc, size_t BufSiz>
	template <class ForwardIterator>
	void deque<T, Alloc, BufSiz>::insert_dispatch_aux(iterator position,
		ForwardIterator first, ForwardIterator last, forward_iterator_tag)
	{
		auto n = distance(first, last);
		if (position.cur == start.cur)
		{ //在头部插入
			iterator new_start = reserve_elements_at_front(n);
			try
			{
				myTinySTL::uninitialized_copy(first, last, new_start);
				start = new_start;
			}
			catch (...)
			{
				destroy_nodes(new_start.node, start.node);
			}
		}
		else if (position.cur == finish.cur)
		{ //在尾部插入
			auto new_finish = reserve_elements_at_back(n);
			try
			{
				myTinySTL::uninitialized_copy(first, last, finish);
				finish = new_finish;
			}
			catch (...)
			{
				destroy_nodes(finish.node + 1, new_finish.node + 1);
			}
		}
		else
			fill_insert_aux(position, first, last, n);
	}

	// reserve_elements_at_back 函数
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::reserve_elements_at_back(size_type n)
	{
		auto left = static_cast<size_type>(finish.last - finish.cur - 1);
		if (n > left)
		{
			auto new_node = (n - left + buffer_size() - 1) / buffer_size();
			reserve_map_at_back(new_node);
			size_type i;
			try
			{
				for (i = 1; i <= new_node; ++i)
					*(finish.node + i) = data_allocator::allocate(buffer_size());
			}
			catch (...)
			{
				for (size_type j = 1; j < i; ++j)
					map_allocator::deallocate(finish.node + j);
			}
		}
		return finish + static_cast<difference_type>(n);
	}

	// reserve_elements_at_front 函数
	template <class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator
		deque<T, Alloc, BufSiz>::reserve_elements_at_front(size_type n)
	{
		auto left = static_cast<size_type>(start.cur - start.first);
		if (n > left)
		{
			auto new_node = (n - left + buffer_size() - 1) / buffer_size();
			reserve_map_at_front(new_node);
			size_type i;
			try
			{
				for (i = 1; i <= new_node; ++i)
					*(start.node - i) = data_allocator::allocate(buffer_size());
			}
			catch (...)
			{
				for (size_type j = 1; j < i; ++j)
					map_allocator::deallocate(start.node - j);
			}
		}
		return start - static_cast<difference_type>(n);
	}
	

}


#endif