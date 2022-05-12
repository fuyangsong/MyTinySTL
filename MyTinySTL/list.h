#pragma once
#ifndef _LIST_H_
#define _LIST_H_

#include "allocator.h"
#include "iterator.h"
#include "reverseiterator.h"
#include "uninitializedfunction.h"
#include "utility.h"

#include <type_traits>

namespace myTinySTL 
{

	//the class of list node
	template<class T>
	struct _list_node
	{
		T data;                 // 数据域
		_list_node<T>* prev;    // 指向前一个节点
		_list_node<T>* next;    // 指向下一个节点
		_list_node(T value = 0, _list_node<T> *p = nullptr, _list_node<T> *n = nullptr) 
			:data(value), prev(p), next(n) {}
		bool operator ==(const _list_node& rhs)
		{
			return data == rhs.data && prev == rhs.prev && next == rhs.next;
		}
	}; // the end

	//the class of list iterator
	template<class T, class Ref, class Ptr>
	struct _list_iterator :public iterator<bidirectional_iterator_tag, T> 
	{
		typedef _list_iterator<T, T&, T*> iterator;
		typedef _list_iterator<T, const T&, const T*> const_iterator;
		typedef _list_iterator < T, Ref, Ptr> self;

		typedef T value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef _list_node<T>* link_type;
		link_type node; // 迭代器内部的普通指针，指向list的节点

		// 构造函数
		_list_iterator() {}
		explicit _list_iterator(link_type x) : node(x) {}
		_list_iterator(const iterator& rhs) : node(rhs.node) {}

		self& operator++()
		{
			node = node->next;
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
			node = node->prev;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--* this;
			return tmp;
		}

		// 以下对迭代器取值(dereference)，取的是节点的数值
		reference& operator *() const { return (*node).data; }
		// 以下是迭代器的成员存取(member access)运算子的标准做法
		pointer* operator ->() { return &(operator*()); }

		// 重载比较操作符
		/*bool operator ==(const self& rhs) const { return node == rhs.node; }
		bool operator !=(const self& rhs) const { return node != rhs.node; }*/

		template<class T, class Ref, class Ptr>
		friend bool operator ==(const _list_iterator<T, Ref, Ptr>& lhs, const _list_iterator<T, Ref, Ptr>& rhs);
		template<class T, class Ref, class Ptr>
		friend bool operator !=(const _list_iterator<T, Ref, Ptr>& lhs, const _list_iterator<T, Ref, Ptr>& rhs);
	}; // the end

	//the class of list
	template<class T, class Alloc = myTinySTL::allocator<T>>
	class list 
	{
	protected:
		// 专属之空间配置器，每次配置一个节点大小
		typedef allocator<_list_node<T>> list_node_allocator;
		
	public:
		typedef T                     value_type;
		typedef value_type*           pointer;
		typedef const value_type*     const_pointer;
		typedef value_type&           reference;
		typedef const value_type&     const_reference;
		typedef size_t                size_type;
		typedef ptrdiff_t             difference_type;

		typedef _list_iterator<T, T&, T*>                     iterator;
		typedef _list_iterator<T, const T&, const T*>         const_iterator;
		typedef myTinySTL::reverse_iterator_t<iterator>       reverse_iterator;
		typedef myTinySTL::reverse_iterator_t<const_iterator> const_reverse_iterator;

		typedef _list_node<T>* link_type;
		
	protected:
		link_type node; // 一个节点表示整个list，指向尾端的空白节点

	public:
		// 默认构造、初始化构造、拷贝构造、移动构造、赋值、析构
		list();
		explicit list(size_type n);
		list(size_type n, const value_type& value);
		template <class InputIterator>
		list(InputIterator first, InputIterator last);

		list(const list& rhs);
		list(list&& rhs);

		list& operator=(const list& rhs);
		list& operator=(list&& rhs);

		~list();

	public:
		// 迭代器操作相关
		iterator begin() { return iterator(node->next); }
		const_iterator begin() const { return const_iterator(node->next); }
		iterator end() { return iterator(node); }
		const_iterator end() const { return const_iterator(node); }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		// 容量相关操作
		bool empty()const { return node->next == node; }
		size_type size()const { return static_cast<size_type>(distance(begin(), end())); }
		size_type max_size() const { return static_cast<size_type>(-1); }

		// 访问元素相关操作
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(--end()); }

		// 调整容器相关操作
		iterator insert(iterator position) { return insert(position, value_type()); }
		iterator insert(iterator position, const value_type& value);
		void insert(iterator position, size_type n, const value_type& value);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void clear();
		void push_front(const value_type& value) { insert(begin(), value); }
		void pop_front() { erase(begin()); }
		void push_back(const value_type& value) { insert(end(), value); }
		void pop_back()
		{
			iterator tmp = end();
			erase(--tmp);
		}
		void resize(size_type new_size) { resize(new_size, value_type()); }
		void resize(size_type new_size, const value_type& value);
		void swap(list& rhs) { myTinySTL::swap(node, rhs.node); }
		
		
		void remove(const value_type& value);
		template <class Predicate>
		void remove_if(Predicate pred);
		void unique();
		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void splice(iterator position, list& x);
		void splice(iterator position, list&, iterator i);
		void splice(iterator position, list&, iterator first, iterator last);
		void merge(list& x);
		template <class Compare>
		void merge(list& x, Compare comp);
		void reverse();
		void sort();
		template <class Compare>
		void sort(Compare comp);
		
	private:
		link_type get_node();// 配置一个节点并传回
		void put_node(link_type p); // 释放一个节点
		link_type create_node(const value_type& value); // 产生(配置并构造)一个节点，带有元素值
		void destory_node(link_type p); // 销毁(析构并释放)一个节点
		void empty_initialize();
		// 将[first,last)内的所有元素移动到position之前
		void transfer(iterator position, iterator first, iterator last);
		void fill_insert(iterator position, size_type n, const value_type& value);
		template<class Integer>
		void insert_dispatch(iterator position, Integer n, Integer value, _true_type);
		template<class InputIterator>
		void insert_dispatch(iterator position, InputIterator first, InputIterator last, _false_type);
	};//end of List

	/***************************************************************************************************/

	template<class T, class Ref, class Ptr>
	bool operator ==(const _list_iterator<T, Ref, Ptr>& lhs, const _list_iterator<T, Ref, Ptr>& rhs)
	{
		return lhs.node == rhs.node;
	}
	template<class T, class Ref, class Ptr>
	bool operator !=(const _list_iterator<T, Ref, Ptr>& lhs, const _list_iterator<T, Ref, Ptr>& rhs)
	{
		return !(lhs == rhs);
	}

	// 默认构造函数
	template<class T, class Alloc>
	list<T, Alloc>::list()
	{
		empty_initialize();
	}

	// 初始化构造函数
	template<class T, class Alloc>
	list<T, Alloc>::list(size_type n)
	{
		empty_initialize();
		insert(begin(), n, value_type());
	}

	template<class T, class Alloc>
	list<T, Alloc>::list(size_type n, const value_type& value)
	{
		empty_initialize();
		insert(begin(), n, value);
	}

	template<class T, class Alloc>
	template <class InputIterator>
	list<T, Alloc>::list(InputIterator first, InputIterator last)
	{
		empty_initialize();
		insert(begin(), first, last);
	}

	// 拷贝构造函数
	template<class T, class Alloc>
	list<T, Alloc>::list(const list& rhs)
	{
		empty_initialize();
		insert(begin(), rhs.begin(), rhs.end());
	}

	// 移动构造函数
	template<class T, class Alloc>
	list<T, Alloc>::list(list&& rhs)
	{
		node = rhs.node;
		rhs.node = nullptr;
	}

	// 赋值运算符
	template<class T, class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator = (const list& rhs)
	{
		if (this != &rhs)
		{
			auto first1 = begin();
			auto last1 = end();
			auto first2 = begin();
			auto last2 = end();
			while (first1 != last1 && first2 != last2)
				*first1++ = *first2++;
			if (first2 == last2)
				erase(first1, last1);
			else
				insert(last1, first2, last2);
		}
		return *this;
	}

	// 析构函数
	template<class T, class Alloc>
	list<T, Alloc>::~list()
	{
		if (node)
		{
			clear();
			put_node(node);
			node = nullptr;
		}
	}

	// 在迭代器position所指位置插入一个节点，内容为x
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, const value_type& value)
	{
		link_type tmp = create_node(value); // 产生一个节点(设妥内容为x)
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(position.node->prev))->next = tmp;
		position.node->prev = tmp;
		return iterator(tmp);

	}

	// 在position处插入n个元素
	template<class T, class Alloc>
	void list<T, Alloc>::insert(iterator position, size_type n, const value_type& value)
	{
		fill_insert(position, n, value);
	}

	template<class T, class Alloc>
	template <class InputIterator>
	void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last)
	{
		typedef typename _is_integer<InputIterator>::is_integer Integer;
		insert_dispatch(position, first, last, Integer());
	}

	// 移除迭代器position所指节点，返回被删除节点的下一个节点的迭代器
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position)
	{
		link_type next_node = link_type(position.node->next);
		link_type prev_node = link_type(position.node->prev);
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destory_node(position.node);
		return iterator(next_node);
	}

	// 删除[first, last)内的元素
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last)
	{
		typename list<T, Alloc>::iterator res;
		for (; first != last;)
		{
			auto temp = first++;
			res = erase(temp);
		}
		return res;
	}

	// 重置容器大小
	template<class T, class Alloc>
	void list<T, Alloc>::resize(size_type new_size, const value_type& value)
	{
		auto i = begin();
		size_type len = 0;
		while (i != end() && len < new_size)
		{
			++i;
			+len;
		}
		if (len == new_size)
			erase(i, end());
		else
			insert(end(), new_size - len, value);
	}

	// 清除所有节点
	template<class T, class Alloc>
	void list<T, Alloc>::clear()
	{
		if (node)
		{
			auto cur = node->next;
			while (cur != node)
			{
				auto tmp = cur;
				cur = cur->next;
				destory_node(tmp);
			}
			node->prev = node->next = node;
		}
	}

	// 将x接合于position所指位置之前。x必须不同于*this
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& x)
	{
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}

	// 将i所指元素接合于position所指位置之前。position和i可指向同一个list
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list&, iterator i)
	{
		iterator j = i;
		++j;
		if (position == i || position == j) return; //i就是positin或者i就在position前一个位置
		transfer(position, i, j);
	}

	//将[first,last)内的所有元素接合于position所指位置之前
	//position和[first,last)可能指向同一个list
	//但position不能位于[first,last)内
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list&, iterator first, iterator last)
	{
		if (first != last)
			transfer(position, first, last);
	}

	//将数值为value之所有元素移除
	template<class T, class Alloc>
	void list<T, Alloc>::remove(const value_type& value)
	{
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			iterator next = first;
			++next;
			if (*first == value)
				erase(first);
			first = next;
		}
	}

	// 将另一元pred为true的所有元素删除
	template<class T, class Alloc>
	template <class Predicate>
	void list<T, Alloc>::remove_if(Predicate pred)
	{
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			iterator next = first;
			++next;
			if (pred(*first))
				erase(first);
			first = next;
		}
	}

	//移除数值相同的连续元素。注意，只有“连续而相同的元素“，才会被移除剩一个
	template<class T, class Alloc>
	void list<T, Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return; // 空链表，什么都不做
		iterator next = first;
		while (++next != last) // 遍历每一个节点
		{
			if (*first == *next) // 如果在此区段中有相同的节点
				erase(next); // 移除之
			else
				first = next; // 调整指针
			next = first; // 修正区段范围
		}
	}

	template<class T, class Alloc>
	template <class BinaryPredicate>
	void list<T, Alloc>::unique(BinaryPredicate binary_pred)
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return; // 空链表，什么都不做
		iterator next = first;
		while (++next != last) // 遍历每一个节点
		{
			if (binary_pred(*first, *next))
				erase(next); // 移除之
			else
				first = next; // 调整指针
			next = first; // 修正区段范围
		}
	}

	// merge()将x合并到*this身上。两个lists的内容都必须先经过递增排序
	template<class T, class Alloc>
	void list<T, Alloc>::merge(list& x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
				++first1;
		}
		if (first2 != last2) transfer(last1, first2, last2);
	}

	template<class T, class Alloc>
	template <class Compare>
	void list<T, Alloc>::merge(list& x, Compare comp)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first2, *first1))
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
				++first1;
		}
		if (first2 != last2) transfer(last1, first2, last2);
	}

	//reverse()将*this的内容逆向重置
	template<class T, class Alloc>
	void list<T, Alloc>::reverse()
	{
		// 以下判断，如果是空链表，或是仅有一个元素，就不进行任何操作
		if (node == node->next || node == node->next->next)
			return;
		iterator first = begin();
		++first;
		while (first != end())
		{
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::link_type list<T, Alloc>::get_node()
	{
		return list_node_allocator::allocate();
	}

	template<class T, class Alloc>
	void list<T, Alloc>::put_node(link_type p)
	{
		list_node_allocator::deallocate(p);
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::link_type list<T, Alloc>::create_node(const value_type& value)
	{
		link_type p = get_node();
		try
		{
			myTinySTL::construct(p, _list_node<T>(value));
		}
		catch (...)
		{
			list_node_allocator::deallocate(p);
		}
		return p;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::destory_node(link_type p)
	{
		myTinySTL::destroy(&p->data);
		put_node(p);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::empty_initialize()
	{
		node = get_node();
		node->prev = node->next = node;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::fill_insert(iterator position, size_type n, const value_type& value)
	{
		for (; n > 0; --n)
			insert(position, value);
	}

	template<class T, class Alloc>
	template<class Integer>
	void list<T, Alloc>::insert_dispatch(iterator position, Integer n, Integer value, _true_type)
	{
		fill_insert(position, static_cast<size_type>(n), static_cast<T>(value));
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void list<T, Alloc>::insert_dispatch(iterator position, InputIterator first,
		InputIterator last, _false_type)
	{
		for (; first != last; ++first)
			insert(position, *first);
	}

	// 将[first,last)内的所有元素移动到position之前
	template<class T, class Alloc>
	void list<T, Alloc>::transfer(iterator position, iterator first, iterator last)
	{
		if (position != last)
		{
			(*(link_type((*last.node).prev))).next = position.node;
			(*(link_type((*first.node).prev))).next = last.node;
			(*(link_type((*position.node).prev))).next = first.node;
			link_type tmp = link_type((*position.node).prev);
			(*position.node).prev = (*last.node).prev;
			(*last.node).prev = (*first.node).prev;
			(*first.node).prev = tmp;
		}
	}

	// list 不能使用STL算法sort()，必须使用自己的sort()member function,
	// 因为STL算法sort()只接受RamdonAccessIterator
	// 本函数采用quick sort
	template<class T, class Alloc>
	template <class Compare>
	void list<T, Alloc>::sort(Compare comp)
	{
		if (node->next == node || node->next->next == node)
			return;

		list<T, Alloc> carry;
		list<T, Alloc> counter[64];
		int fill = 0;
		while (!empty())
		{
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry, comp);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
				++fill;
		}
		for (int i = 1; i != fill; ++i)
		{
			counter[i].merge(counter[i - 1], comp);
		}
		swap(counter[fill - 1]);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::sort()
	{
		sort(myTinySTL::less<T>());
	}



	//******************重载比较操作符******************************************
	template <class T, class Alloc>
	inline bool operator== (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		typedef typename list<T, Alloc>::const_iterator const_iterator;
		const_iterator first1 = lhs.begin();
		const_iterator first2 = rhs.begin();
		const_iterator last1 = lhs.end();
		const_iterator last2 = rhs.end();
		for (; first1 != last1, first2 != last2; ++first1, ++first2)
		{
			if (first1.node->data != first2.node->data)
				break;
		}
		if (first1 == last1 && first2 == last2)
			return true;
		return false;
	}

	template <class T, class Alloc>
	inline bool operator!= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template<class T, class Alloc>
	void swap(list<T, Alloc>& x, list<T, Alloc>& y)
	{
		x.swap(y);
	}
}

#endif //_LIST_H_ 

