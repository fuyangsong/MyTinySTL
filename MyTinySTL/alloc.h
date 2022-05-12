#pragma once
#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <cstdlib>

namespace myTinySTL
{
	class alloc
	{
	private:
		enum EAlign {_ALIGN = 8}; // 小型区块的上调边界
		enum EMaxBytes{_MAX_BYTES = 128 }; // 小型区块的上限
		enum ENFreeLists {_NFREELISTS = EMaxBytes::_MAX_BYTES / EAlign::_ALIGN}; // free-lists个数
		enum ENObjs {_NOBJS = 20}; // 每次增加的节点数
	private:
		union obj // free-lists的节点构造
		{
			union obj* free_list_link;
			char client_data[1];
		};
		// 16个free-lists
		static obj* free_list[ENFreeLists::_NFREELISTS];
	private:
		// Chunk allocation state
		static char* start_free; // 内存池起始位置。只在chunk_alloc()中变化
		static char* end_free; // 内存池结束位置。只在chunk_alloc()中变化
		static size_t heap_size;
	private:
		// 将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes)
		{
			return (((bytes)+EAlign::_ALIGN - 1) & ~(EAlign::_ALIGN - 1));
		}
		// 以下函数根据区块的大小，决定使用第n号free-list。n从0开始
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (((bytes) + EAlign::_ALIGN - 1) / EAlign::_ALIGN - 1);
		}
		// 返回一个大小为n的对象，并可能加入大小为n的其它区块到free-list
		static void* refill(size_t n);
		// 配置一大块空间，可容纳nobjs个大小为“size”的区块
		// 如果配置nobjs个区块有所不便，nobjs可能会降低
		static char* chunk_alloc(size_t n, int& nobjs);
	public:
		static void* allocate(size_t n);
		static void deallocate(void* p, size_t n);
		static void* reallocate(void* p, size_t old_sz, size_t new_sz);
	}; // 静态方法效率上比实例化要高，这种经常要用的函数静态是最好了，每次实例化都要成本
}


#endif
