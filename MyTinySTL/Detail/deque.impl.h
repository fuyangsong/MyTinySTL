#pragma once
#ifndef _DEQUE_IMPL_H_
#define _DEQUE_IMPL_H_

#include "../deque.h"

namespace myTinySTL 
{

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::create_map_and_nodes(size_type num_elements)
	{
		// ��Ҫ�Ľڵ���=(Ԫ�ظ���/ÿ�������������ɵ�Ԫ�ظ���)+1
		// ����պ������������һ���ڵ�
		size_type num_nodes = num_elements / buffer_size() + 1;
		// һ��mapҪ�������ڵ㡣����8���������"����ڵ�����2"
		// (ǰ���Ԥ��һ��������ʱ����)
		map_size = max(initial_map_size(), num_nodes + 2);
		map = map_allocator::allocate(map_size);
		// �������ó�һ��"����map_size���ڵ�"��map

		// ������nstart��nfinishָ��map��ӵ��֮ȫ���ڵ������������
		// �����������룬��ʹͷβ���˵���������һ����ÿ���ڵ�ɶ�Ӧһ��������
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		for (cur = nstart; cur <= nfinish; ++cur)
		{
			*cur = data_allocator::allocate(_deque_buf_size(BufSiz,sizeof(T)));
		}

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % iterator::buffer_size();
	}

	// deque�Ĺ������ڴ����
	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type& value)
	{
		create_map_and_nodes(n);
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
	deque<T, Alloc, BufSiz>::deque() :start(), finish(), map_size(0), map(0)
	{
		create_map_and_nodes(0);
	}

	template<class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::deque(size_type n, const value_type& value) 
	{
		deque();
		fill_initialize(n, value);
	}

	

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_back(const value_type& val) 
	{
		if (finish.cur != finish.last - 1)
		{
			// ��󻺳�����������(��)���ϵ�Ԫ�ر��ÿռ�
			construct(finish.cur, val);
			++finish.cur;
		}
		else // ��󻺳���ֻʣһ��Ԫ�ر��ÿռ�
		{
			push_back_aux(val);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_back_aux(const value_type& t)
	{
		value_type t_copy = t;
		reserve_map_at_back(1);
		*(finish.node + 1) = data_allocator::allocate(_deque_buf_size(BufSiz, sizeof(T)));
		construct(finish.cur, t_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_front(const value_type& val) 
	{
		if (start.cur != start.first)
		{
			construct(start.cur - 1, val);
			--start.cur;
		}
		else
		{
			push_front_aux(val);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_front_aux(const value_type& t)
	{
		value_type t_copy = t;
		reserve_map_at_front(1);
		*(start.node - 1) = data_allocator::allocate(_deque_buf_size(BufSiz, sizeof(T)));
		start.set_node(start.node - 1);
		start. cur = start.last - 1;
		construct(start.cur, t_copy);
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::reserve_map_at_back(size_type nodes_to_add)
	{
		if (nodes_to_add + 1 > map_size - (finish.node - map))
		{
			// ���mapβ�˵Ľڵ㱸�ÿռ䲻��
			// ������������������ػ�һ��map(���ø���ģ�����ԭ���ģ��ͷ�ԭ����)
			reallocate_map(nodes_to_add, false);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::reserve_map_at_front(size_type nodes_to_add)
	{
		if (nodes_to_add > start.node - map)
		{
			// ���mapǰ�˵Ľڵ㱸�ÿռ䲻��
			// ������������������ػ�һ��map(���ø���ģ�����ԭ���ģ��ͷ�ԭ����)
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
		deallocate_node(start.first);
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
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		myTinySTL::destroy(finish.cur);
	}

	// ��ע�⣬deque�����״̬(���κ�Ԫ��)����һ����������
	// ��ˣ�clear()���֮��ظ���ʼ״̬��Ҳһ��Ҫ����һ����������
	template<class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::clear() 
	{
		// �������ͷβ�����ÿһ����������
		for (map_pointer node = start.node + 1; node < finish.node; ++node)
		{
			myTinySTL::destroy(*node, *node + _deque_buf_size(BufSiz, sizeof(T)));
			data_allocator::deallocate(*node, _deque_buf_size(BufSiz, sizeof(T)));
		}
		if (start.node != finish.node)// ������ͷβ������
		{
			myTinySTL::destroy(start.cur, start.last);
			myTinySTL::destroy(finish.first, finish.last);
			data_allocator::deallocate(finish.first, iterator::buffer_size());
		}
		else // ֻ��һ��������
		{
			myTinySTL::destroy(start.cur, finish.cur);
		}
		finish = start;
	}

	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::erase(iterator pos)
	{
		iterator next = pos;
		difference_type index = pos - start; // �����֮ǰ��Ԫ�ظ���
		if (index < (size() >> 1)) 
		{
			// ��������֮ǰ��Ԫ�رȽ���
			std::copy_backward(start, pos, next); // �ƶ������֮ǰ��Ԫ��
			pop_front(); // �ƶ���ϣ���ǰһ��Ԫ�����࣬ȥ��֮
		}
		else 
		{
			// �����֮���Ԫ�رȽ���
			myTinySTL::copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	// ���[firast, last)�����ڵ�Ԫ��
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
			difference_type n = last - first; // �������ĳ���
			difference_type elems_before = first - start; // �������ǰ����Ԫ�ظ���
			if (elems_before < (size() - n) / 2)
			{
				std::copy_backward(start, first, last);
				iterator new_start = start + n;
				destory(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
				{
					data_allocator::deallocate(*cur, _deque_buf_size(BufSiz, sizeof(T)));
				}
				start = new_start;
			}
			else
			{
				myTinySTL::copy(last, finish, first);
				iterator new_finish = finish - n;
				myTinySTL::destroy(new_finish, finish);
				for (map_pointer cur = finish.node; cur < new_finish.node; ++cur)
				{
					data_allocator::deallocate(*cur, _deque_buf_size(BufSiz, sizeof(T)));
				}
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert(iterator position,
		const value_type& x)
	{
		if (position.cur == start.cur)
		{
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur)
		{
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(position, x);
		}
	}

	template<class T, class Alloc, size_t BufSiz>
	typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert_aux(iterator pos,
		const value_type& x)
	{
		difference_type index = pos - start;
		value_type x_copy = x;
		if (index < size() / 2)
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			std::copy_backward(pos, back2, back1);
		}
		*pos = x_copy;
		return pos;
	}

	template<class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::~deque()
	{
		for (map_pointer cur = map; cur != cur + map_size; ++cur)
		{
			myTinySTL::destroy(*cur, *(cur + _deque_buf_size(BufSiz, sizeof(T))));
			data_allocator::deallocate(*cur, _deque_buf_size(BufSiz, sizeof(T)));
		}
		map_allocator::deallocate(map, map_size);

	}

	

}
#endif
