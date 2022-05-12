#include "../alloc.h"


namespace myTinySTL
{
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;

	alloc::obj* alloc::free_list[alloc::ENFreeLists::_NFREELISTS] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	void* alloc::allocate(size_t n)
	{
		// ����128ֱ��ʹ��malloc����
		if (n > EMaxBytes::_MAX_BYTES)
		{
			return malloc(n);
		}
		else
		{
			obj** my_free_list;
			obj* result;
			// Ѱ��16��free-list���ʵ���һ��
			my_free_list = free_list + FREELIST_INDEX(n);
			result = *my_free_list;
			if (result == 0)
			{
				// û���ҵ����õ�free-list��׼���������free-list
				void* r = refill(ROUND_UP(n));
			}
			else
			{
				// ����free-list
				*my_free_list = result->free_list_link;
				return result;
			}
		}
	}

	void alloc::deallocate(void* p, size_t n)
	{
		if (n > EMaxBytes::_MAX_BYTES)
		{
			free(p);
			return;
		}
		else
		{
			obj* q = static_cast<obj*>(p);
			obj** my_free_list = free_list + FREELIST_INDEX(n);
			q->free_list_link = *my_free_list;
			*my_free_list = q;
		}
	}

	void* alloc::reallocate(void* p, size_t old_sz, size_t new_sz)
	{
		deallocate(p, old_sz);
		p = allocate(new_sz);
		return p;
	}

	// ����һ����СΪn�Ķ��󣬲�����ʱ���Ϊ�ʵ���free-list���ӽڵ�
	// ����n�Լ��ʵ��ϵ���8�ı���
	void* alloc::refill(size_t n)
	{
		int nobjs = ENObjs::_NOBJS;
		// ����chunk_alloc()������ȡ��nobjs��������Ϊfree-list���½ڵ�
		// ע�����nobjs��pass by reference
		char* chunk = chunk_alloc(n, nobjs);
		obj** my_free_list;
		obj* result;
		obj* current_obj;
		obj* next_obj;
		// ���ֻ���һ�����飬�������ͷ�����������ã�free-list���½ڵ�
		if (1 == nobjs) return chunk;
		// ����׼������free-list�������½ڵ�
		my_free_list = free_list + FREELIST_INDEX(n);
		// ������chunk�ռ��ڽ���free-list
		result = (obj*)chunk; // ��һ��׼�����ظ��Ͷ�
		// ���µ���free-listָ�������õĿռ䣨ȡ���ڴ�أ�
		*my_free_list = next_obj = (obj*)(chunk + n);
		// ���½�free-list�ĸ��ڵ㴮������
		for (int i = 1; ; i++)
		{
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i)
			{
				current_obj->free_list_link = 0;
				break;
			}
			else
			{
				current_obj->free_list_link = next_obj;
			}
		}
		return result;
	}

	// ����size�Ѿ��ʵ��ϵ���8�ı���
	// ע�����nobjs��pass by reference
	char* alloc::chunk_alloc(size_t size, int& nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free; // �ڴ��ʣ��ռ�
		if (bytes_left >= total_bytes)
		{
			// �ڴ�ص�ʣ��ռ���ȫ����������
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size)
		{
			// �ڴ��ʣ��ռ䲻����ȫ���������������㹻��Ӧһ�����������ϵ�����
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else
		{
			// �ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			// �����������ڴ���еĲ�����ͷ�������ü�ֵ
			if (bytes_left > 0)
			{
				// �ڴ�ػ���һЩ��ͷ��������ʵ���free-list
				// ����Ѱ���ʵ���free-list
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				// ����free-list�����ڴ���еĲ���ռ����
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			// ����heap�ռ䣬�������ڴ��
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				// heap�ռ䲻�㣬mallocʧ��
				obj** my_free_list = 0, * p = 0;
				// ���ż�����������ӵ�еĶ������ⲻ������˺���
				for (int i = size; i <= EMaxBytes::_MAX_BYTES; i += EAlign::_ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p) // free-list������δ������
					{
						// ����free-list���ͳ�δ������
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						// �ݹ�����Լ���Ϊ������nobjs
						return chunk_alloc(size, nobjs);
						// ע�⣬�κβ�����ͷ�ս��������ʵ���free-list�б���
					}
				}
				end_free = 0;
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
}