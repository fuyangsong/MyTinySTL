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
		// 大于128直接使用malloc分配
		if (n > EMaxBytes::_MAX_BYTES)
		{
			return malloc(n);
		}
		else
		{
			obj** my_free_list;
			obj* result;
			// 寻找16个free-list中适当的一个
			my_free_list = free_list + FREELIST_INDEX(n);
			result = *my_free_list;
			if (result == 0)
			{
				// 没有找到可用的free-list，准备重新填充free-list
				void* r = refill(ROUND_UP(n));
			}
			else
			{
				// 调整free-list
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

	// 返回一个大小为n的对象，并且有时候会为适当的free-list增加节点
	// 假设n以及适当上调至8的倍数
	void* alloc::refill(size_t n)
	{
		int nobjs = ENObjs::_NOBJS;
		// 调用chunk_alloc()，尝试取得nobjs个区块作为free-list的新节点
		// 注意参数nobjs是pass by reference
		char* chunk = chunk_alloc(n, nobjs);
		obj** my_free_list;
		obj* result;
		obj* current_obj;
		obj* next_obj;
		// 如果只获得一个区块，这个区块就分配给调用者用，free-list无新节点
		if (1 == nobjs) return chunk;
		// 否则准备调整free-list，纳入新节点
		my_free_list = free_list + FREELIST_INDEX(n);
		// 以下在chunk空间内建立free-list
		result = (obj*)chunk; // 这一块准备返回给客端
		// 以下导引free-list指向新配置的空间（取自内存池）
		*my_free_list = next_obj = (obj*)(chunk + n);
		// 以下将free-list的各节点串接起来
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

	// 假设size已经适当上调至8的倍数
	// 注意参数nobjs是pass by reference
	char* alloc::chunk_alloc(size_t size, int& nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free; // 内存池剩余空间
		if (bytes_left >= total_bytes)
		{
			// 内存池的剩余空间完全满足需求量
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size)
		{
			// 内存池剩余空间不能完全满足需求量，但足够供应一个（含）以上的区块
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else
		{
			// 内存池剩余空间连一个区块的大小都无法提供
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			// 以下试着让内存池中的残余零头还有利用价值
			if (bytes_left > 0)
			{
				// 内存池还有一些零头，先配给适当的free-list
				// 首先寻找适当的free-list
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				// 调整free-list，将内存池中的残余空间编入
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			// 配置heap空间，来补充内存池
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				// heap空间不足，malloc失败
				obj** my_free_list = 0, * p = 0;
				// 试着检视我们手上拥有的东西，这不会造成伤害，
				for (int i = size; i <= EMaxBytes::_MAX_BYTES; i += EAlign::_ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p) // free-list内尚有未用区块
					{
						// 调整free-list以释出未用区块
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						// 递归调用自己，为了修正nobjs
						return chunk_alloc(size, nobjs);
						// 注意，任何残余零头终将被编入适当的free-list中备用
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