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

	//****************���졢��������ֵ������***********************
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

	//***************listԪ�ز������******************************
	// �ڵ�����position��ָλ�ò���һ���ڵ㣬����Ϊx
	template<class T>
	typename list<T>::iterator list<T>::insert(iterator position, const T& x)
	{
		link_type tmp = create_node(x); // ����һ���ڵ�(��������Ϊx)
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

	// ����һ���ڵ㣬��Ϊͷ�ڵ�
	template<class T>
	void list<T>::push_front(const T& x)
	{
		link_type node = create_node(x);
		head.node->prev = node;
		node->next = head.node;
		head.node = node;
	}

	// ����һ���ڵ㣬��Ϊβ�ڵ�
	template<class T>
	void list<T>::push_back(const T& x)
	{
		link_type node = get_node();
		tail.node->data = x;
		tail.node->next = node;
		node->prev = tail.node;
		tail.node = node;
	}

	// �Ƴ�������position��ָ�ڵ㣬���ر�ɾ���ڵ����һ���ڵ�ĵ�����
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

	// �Ƴ�ͷ�ڵ�
	template<class T>
	void list<T>::pop_front()
	{
		link_type oldNode = head.node;
		head.node = oldNode->next;
		head.node->prev = nullptr;
		destory_node(oldNode);
	}

	// �Ƴ�β���
	template<class T>
	void list<T>::pop_back()
	{
		link_type newTail = tail.node->prev;
		newTail->next = nullptr;
		destory_node(tail.node);
		tail.node = newTail;
	}

	// ������нڵ�
	template<class T>
	void list<T>::clear()
	{
		erase(begin(), end());
	}

	//����ֵΪvalue֮����Ԫ���Ƴ�
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

	//�Ƴ���ֵ��ͬ������Ԫ�ء�ע�⣬ֻ�С���������ͬ��Ԫ�ء����Żᱻ�Ƴ�ʣһ��
	template<class T>
	void list<T>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return; // ������ʲô������
		iterator next = first;
		while (++next != last) // ����ÿһ���ڵ�
		{
			if (*first == *next) // ����ڴ�����������ͬ�Ľڵ�
				erase(next); // �Ƴ�֮
			else
				first = next; // ����ָ��
			next = first; // �������η�Χ
		}
	}

	template<class T>
	template <class BinaryPredicate>
	void list<T>::unique(BinaryPredicate binary_pred) 
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return; // ������ʲô������
		iterator next = first;
		while (++next != last) // ����ÿһ���ڵ�
		{
			if (binary_pred(*first, *next)) 
				erase(next); // �Ƴ�֮
			else
				first = next; // ����ָ��
			next = first; // �������η�Χ
		}
	}

	// ��[first,last)�ڵ�����Ԫ���ƶ���position֮ǰ
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

	// ��x�Ӻ���position��ָλ��֮ǰ��x���벻ͬ��*this
	template<class T>
	void list<T>::splice(iterator position, list<T>& x)
	{
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}

	// ��i��ָԪ�ؽӺ���position��ָλ��֮ǰ��position��i��ָ��ͬһ��list
	template<class T>
	void list<T>::splice(iterator position, list<T>&, iterator i)
	{
		iterator j = i;
		++j;
		if (position == i || position == j) return; //i����positin����i����positionǰһ��λ��
		transfer(position, i, j);
	}

	//��[first,last)�ڵ�����Ԫ�ؽӺ���position��ָλ��֮ǰ
	//position��[first,last)����ָ��ͬһ��list
	//��position����λ��[first,last)��
	template<class T>
	void list<T>::splice(iterator position, list<T>&, iterator first, iterator last)
	{
		if (first != last)
			transfer(position, first, last);
	}

	// merge()��x�ϲ���*this���ϡ�����lists�����ݶ������Ⱦ�����������
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

	//reverse()��*this��������������
	template<class T>
	void list<T>::reverse()
	{
		// �����жϣ�����ǿ��������ǽ���һ��Ԫ�أ��Ͳ������κβ���
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
	

	// list ����ʹ��STL�㷨sort()������ʹ���Լ���sort()member function,
	// ��ΪSTL�㷨sort()ֻ����RamdonAccessIterator
	// ����������quick sort
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

	

	//*****************Ԫ�ظ������*********************************
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

	//******************��Ԫ����******************************************
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
