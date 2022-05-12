#pragma once
#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <cstdlib>

namespace myTinySTL
{
	class alloc
	{
	private:
		enum EAlign {_ALIGN = 8}; // С��������ϵ��߽�
		enum EMaxBytes{_MAX_BYTES = 128 }; // С�����������
		enum ENFreeLists {_NFREELISTS = EMaxBytes::_MAX_BYTES / EAlign::_ALIGN}; // free-lists����
		enum ENObjs {_NOBJS = 20}; // ÿ�����ӵĽڵ���
	private:
		union obj // free-lists�Ľڵ㹹��
		{
			union obj* free_list_link;
			char client_data[1];
		};
		// 16��free-lists
		static obj* free_list[ENFreeLists::_NFREELISTS];
	private:
		// Chunk allocation state
		static char* start_free; // �ڴ����ʼλ�á�ֻ��chunk_alloc()�б仯
		static char* end_free; // �ڴ�ؽ���λ�á�ֻ��chunk_alloc()�б仯
		static size_t heap_size;
	private:
		// ��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes)
		{
			return (((bytes)+EAlign::_ALIGN - 1) & ~(EAlign::_ALIGN - 1));
		}
		// ���º�����������Ĵ�С������ʹ�õ�n��free-list��n��0��ʼ
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (((bytes) + EAlign::_ALIGN - 1) / EAlign::_ALIGN - 1);
		}
		// ����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void* refill(size_t n);
		// ����һ���ռ䣬������nobjs����СΪ��size��������
		// �������nobjs�������������㣬nobjs���ܻή��
		static char* chunk_alloc(size_t n, int& nobjs);
	public:
		static void* allocate(size_t n);
		static void deallocate(void* p, size_t n);
		static void* reallocate(void* p, size_t old_sz, size_t new_sz);
	}; // ��̬����Ч���ϱ�ʵ����Ҫ�ߣ����־���Ҫ�õĺ�����̬������ˣ�ÿ��ʵ������Ҫ�ɱ�
}


#endif
