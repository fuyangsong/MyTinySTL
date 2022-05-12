#pragma once
#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_

#include "../list.h"

namespace myTinySTL 
{

	//*********************** [list] ****************************************
	template<class T>
	typename list<T>::link_type list<T>::get_node()
	{
		return list_node_allocator::allocate();
	}

	template<class T>
	void list<T>::put_node(link_type p)
	{
		list_node_allocator::deallocate(p);
	}

	template<class T>
	typename list<T>::link_type list<T>::create_node(const T& x)
	{
		link_type p = get_node();
		list_node_allocator::construct(&p->data, x);
		return p;
	}

	template<class T>
	void list<T>::destory_node(link_type p)
	{
		p->prev = p->next = nullptr;
		list_node_allocator::destroy(&p->data);
		put_node(p);
	}

	template<class T>
	void list<T>::empty_initialize()
	{
		head.node = get_node();
		tail.node = head.node;
	}

	//****************构造、拷贝、赋值、析构***********************
	template<class T>
	list<T>::list() 
	{
		empty_initialize();
	}

	

	template<class T>
	list<T>::list(size_type n, const T& x) 
	{
		empty_initialize();
		while (n--)
			push_back(x);
	}

	template<class T>
	template <class InputIterator>
	list<T>::list(InputIterator first, InputIterator last) 
	{
		empty_initialize();
		for (; first != last; ++first)
			push_back(*first);
	}

	template<class T>
	list<T>::list(const list<T>& l) 
	{
		empty_initialize();
		for (link_type cur = l.head.node; cur != l.tail.node; cur = cur->next)
			push_back(cur->data);
	}

	template<class T>
	list<T>& list<T>::operator = (const list& l) 
	{
		if (this != &l) 
		{
			list(l).swap(*this);
		}
		return *this;
	}

	template<class T>
	list<T>::~list() 
	{
		for (; head != tail;) 
		{
			auto temp = head++;
			//bug fix
			list_node_allocator::destory(temp.node);
			list_node_allocator::deallocate(temp.node);
		}
		list_node_allocator::deallocate(tail.node);
	}

	//***************list元素操作相关******************************
	// 在迭代器position所指位置插入一个节点，内容为x
	template<class T>
	typename list<T>::iterator list<T>::insert(iterator position, const T& x)
	{
		link_type tmp = create_node(x); // 产生一个节点(设妥内容为x)
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(position.node->prev))->next = tmp;
		position.node->prev = tmp;
		return iterator(tmp);

	}


	template<class T>
	void list<T>::insert(iterator position, size_type n, const T& x) 
	{
		for (auto i = n; i != 0; --i)
			position = insert(position, x);
	}

	template<class T>
	template <class InputIterator>
	void list<T>::insert(iterator position, InputIterator first, InputIterator last) 
	{
		iterator tmp = first;
		for (; tmp != last; ++tmp)
			insert(position, *tmp);
	}

	// 插入一个节点，作为头节点
	template<class T>
	void list<T>::push_front(const T& x)
	{
		link_type node = create_node(x);
		head.node->prev = node;
		node->next = head.node;
		head.node = node;
	}

	// 插入一个节点，作为尾节点
	template<class T>
	void list<T>::push_back(const T& x)
	{
		link_type node = get_node();
		tail.node->data = x;
		tail.node->next = node;
		node->prev = tail.node;
		tail.node = node;
	}

	// 移除迭代器position所指节点，返回被删除节点的下一个节点的迭代器
	template<class T>
	typename list<T>::iterator list<T>::erase(iterator position)
	{
		link_type next_node = link_type(position.node->next);
		link_type prev_node = link_type(position.node->prev);
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destory_node(position.node);
		return iterator(next_node);
	}

	template<class T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last) 
	{
		typename list<T>::iterator res;
		for (; first != last;) 
		{
			auto temp = first++;
			res = erase(temp);
		}
		return res;
	}

	// 移除头节点
	template<class T>
	void list<T>::pop_front()
	{
		link_type oldNode = head.node;
		head.node = oldNode->next;
		head.node->prev = nullptr;
		destory_node(oldNode);
	}

	// 移除尾结点
	template<class T>
	void list<T>::pop_back()
	{
		link_type newTail = tail.node->prev;
		newTail->next = nullptr;
		destory_node(tail.node);
		tail.node = newTail;
	}

	// 清除所有节点
	template<class T>
	void list<T>::clear()
	{
		erase(begin(), end());
	}

	//将数值为value之所有元素移除
	template<class T>
	void list<T>::remove(const T& value)
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

	template<class T>
	template <class Predicate>
	void list<T>::remove_if(Predicate pred) 
	{
		for (auto it = begin(); it != end();) 
		{
			if (pred(*it))
				it = erase(it);
			else
				++it;
		}
	}

	//移除数值相同的连续元素。注意，只有“连续而相同的元素“，才会被移除剩一个
	template<class T>
	void list<T>::unique()
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

	template<class T>
	template <class BinaryPredicate>
	void list<T>::unique(BinaryPredicate binary_pred) 
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

	// 将[first,last)内的所有元素移动到position之前
	template<class T>
	void list<T>::transfer(iterator position, iterator first, iterator last)
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

	// 将x接合于position所指位置之前。x必须不同于*this
	template<class T>
	void list<T>::splice(iterator position, list<T>& x)
	{
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}

	// 将i所指元素接合于position所指位置之前。position和i可指向同一个list
	template<class T>
	void list<T>::splice(iterator position, list<T>&, iterator i)
	{
		iterator j = i;
		++j;
		if (position == i || position == j) return; //i就是positin或者i就在position前一个位置
		transfer(position, i, j);
	}

	//将[first,last)内的所有元素接合于position所指位置之前
	//position和[first,last)可能指向同一个list
	//但position不能位于[first,last)内
	template<class T>
	void list<T>::splice(iterator position, list<T>&, iterator first, iterator last)
	{
		if (first != last)
			transfer(position, first, last);
	}

	// merge()将x合并到*this身上。两个lists的内容都必须先经过递增排序
	template<class T>
	void list<T>::merge(list<T>& x)
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

	template<class T>
	template <class Compare>
	void list<T>::merge(list<T>& x, Compare comp) 
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
	template<class T>
	void list<T>::reverse()
	{
		// 以下判断，如果是空链表，或是仅有一个元素，就不进行任何操作
		if (empty() || head.node->next == tail.node)
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

	template<class T>
	void list<T>::swap(list<T>& x) 
	{
		myTinySTL::swap(head.node, x.node);
		myTinySTL::swap(tail.node, x.node);
	}
	

	// list 不能使用STL算法sort()，必须使用自己的sort()member function,
	// 因为STL算法sort()只接受RamdonAccessIterator
	// 本函数采用quick sort
	template<class T>
	template <class Compare>
	void list<T>::sort(Compare comp) 
	{
		if (empty() || head.node->next == tail.node)
			return;

		list<T> carry;
		list<T> counter[64];
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

	template<class T>
	void list<T>::sort()
	{
		sort(myTinySTL::less<T>());
	}

	

	//*****************元素个数相关*********************************
	template<class T>
	typename list<T>::size_type list<T>::size()const
	{
		size_type length = 0;
		for (auto h = head; h != tail; ++h)
			++length;
		return length;
	}

	template<class T>
	typename list<T>::reverse_iterator list<T>::rbegin() 
	{
		return reverse_iterator(tail);
	}
	template<class T>
	typename list<T>::reverse_iterator list<T>::rend() 
	{
		return reverse_iterator(head);
	}

	//******************友元函数******************************************
	template <class T>
	bool operator== (const list<T>& lhs, const list<T>& rhs) 
	{
		typename list<T>::iterator first1 = lhs.begin();
		typename list<T>::iterator first2 = rhs.begin();
		typename list<T>::iterator last1 = lhs.end();
		typename list<T>::iterator last2 = rhs.end();
		for (; first1 != last1, first2 != last2; ++first1, ++first2)
		{
			if (first1.node->data != first2.node->data)
				break;
		}
		if (first1 == last1 && first2 == last2)
			return true;
		return false;
	}

	template <class T>
	bool operator!= (const list<T>& lhs, const list<T>& rhs) 
	{
		return !(lhs == rhs);
	}

	template<class T>
	void swap(list<T>& x, list<T>& y) 
	{
		x.swap(y);
	}

}

#endif
