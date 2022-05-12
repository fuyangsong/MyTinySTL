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
		T data;                 // ������
		_list_node<T>* prev;    // ָ��ǰһ���ڵ�
		_list_node<T>* next;    // ָ����һ���ڵ�
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
		link_type node; // �������ڲ�����ָͨ�룬ָ��list�Ľڵ�

		// ���캯��
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

		// ���¶Ե�����ȡֵ(dereference)��ȡ���ǽڵ����ֵ
		reference& operator *() const { return (*node).data; }
		// �����ǵ������ĳ�Ա��ȡ(member access)�����ӵı�׼����
		pointer* operator ->() { return &(operator*()); }

		// ���رȽϲ�����
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
		// ר��֮�ռ���������ÿ������һ���ڵ��С
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
		link_type node; // һ���ڵ��ʾ����list��ָ��β�˵Ŀհ׽ڵ�

	public:
		// Ĭ�Ϲ��졢��ʼ�����졢�������졢�ƶ����졢��ֵ������
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
		// �������������
		iterator begin() { return iterator(node->next); }
		const_iterator begin() const { return const_iterator(node->next); }
		iterator end() { return iterator(node); }
		const_iterator end() const { return const_iterator(node); }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		// ������ز���
		bool empty()const { return node->next == node; }
		size_type size()const { return static_cast<size_type>(distance(begin(), end())); }
		size_type max_size() const { return static_cast<size_type>(-1); }

		// ����Ԫ����ز���
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(--end()); }

		// ����������ز���
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
		link_type get_node();// ����һ���ڵ㲢����
		void put_node(link_type p); // �ͷ�һ���ڵ�
		link_type create_node(const value_type& value); // ����(���ò�����)һ���ڵ㣬����Ԫ��ֵ
		void destory_node(link_type p); // ����(�������ͷ�)һ���ڵ�
		void empty_initialize();
		// ��[first,last)�ڵ�����Ԫ���ƶ���position֮ǰ
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

	// Ĭ�Ϲ��캯��
	template<class T, class Alloc>
	list<T, Alloc>::list()
	{
		empty_initialize();
	}

	// ��ʼ�����캯��
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

	// �������캯��
	template<class T, class Alloc>
	list<T, Alloc>::list(const list& rhs)
	{
		empty_initialize();
		insert(begin(), rhs.begin(), rhs.end());
	}

	// �ƶ����캯��
	template<class T, class Alloc>
	list<T, Alloc>::list(list&& rhs)
	{
		node = rhs.node;
		rhs.node = nullptr;
	}

	// ��ֵ�����
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

	// ��������
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

	// �ڵ�����position��ָλ�ò���һ���ڵ㣬����Ϊx
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, const value_type& value)
	{
		link_type tmp = create_node(value); // ����һ���ڵ�(��������Ϊx)
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(position.node->prev))->next = tmp;
		position.node->prev = tmp;
		return iterator(tmp);

	}

	// ��position������n��Ԫ��
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

	// �Ƴ�������position��ָ�ڵ㣬���ر�ɾ���ڵ����һ���ڵ�ĵ�����
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

	// ɾ��[first, last)�ڵ�Ԫ��
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

	// ����������С
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

	// ������нڵ�
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

	// ��x�Ӻ���position��ָλ��֮ǰ��x���벻ͬ��*this
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& x)
	{
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}

	// ��i��ָԪ�ؽӺ���position��ָλ��֮ǰ��position��i��ָ��ͬһ��list
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list&, iterator i)
	{
		iterator j = i;
		++j;
		if (position == i || position == j) return; //i����positin����i����positionǰһ��λ��
		transfer(position, i, j);
	}

	//��[first,last)�ڵ�����Ԫ�ؽӺ���position��ָλ��֮ǰ
	//position��[first,last)����ָ��ͬһ��list
	//��position����λ��[first,last)��
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list&, iterator first, iterator last)
	{
		if (first != last)
			transfer(position, first, last);
	}

	//����ֵΪvalue֮����Ԫ���Ƴ�
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

	// ����һԪpredΪtrue������Ԫ��ɾ��
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

	//�Ƴ���ֵ��ͬ������Ԫ�ء�ע�⣬ֻ�С���������ͬ��Ԫ�ء����Żᱻ�Ƴ�ʣһ��
	template<class T, class Alloc>
	void list<T, Alloc>::unique()
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

	template<class T, class Alloc>
	template <class BinaryPredicate>
	void list<T, Alloc>::unique(BinaryPredicate binary_pred)
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

	// merge()��x�ϲ���*this���ϡ�����lists�����ݶ������Ⱦ�����������
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

	//reverse()��*this��������������
	template<class T, class Alloc>
	void list<T, Alloc>::reverse()
	{
		// �����жϣ�����ǿ��������ǽ���һ��Ԫ�أ��Ͳ������κβ���
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

	// ��[first,last)�ڵ�����Ԫ���ƶ���position֮ǰ
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

	// list ����ʹ��STL�㷨sort()������ʹ���Լ���sort()member function,
	// ��ΪSTL�㷨sort()ֻ����RamdonAccessIterator
	// ����������quick sort
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



	//******************���رȽϲ�����******************************************
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

