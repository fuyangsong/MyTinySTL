#include "heaptest.h"

namespace myTinySTL
{
	namespace HeapTest
	{
		void testcase1()
		{
			std::cout << "**************This is heap test!*************" << std::endl;
			int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
			std::vector<int> ivec(ia, ia + 9);
			myTinySTL::make_heap(ivec.begin(), ivec.end());
			for (int i = 0; i < ivec.size(); i++)
				std::cout << ivec[i] << ' ';
			std::cout << std::endl;

			ivec.push_back(7);
			myTinySTL::push_heap(ivec.begin(), ivec.end());
			for (int i = 0; i < ivec.size(); i++)
				std::cout << ivec[i] << ' ';
			std::cout << std::endl;

			myTinySTL::pop_heap(ivec.begin(), ivec.end());
			std::cout << ivec.back() << std::endl;
			ivec.pop_back();
			for (int i = 0; i < ivec.size(); i++)
				std::cout << ivec[i] << ' ';
			std::cout << std::endl;

			myTinySTL::sort_heap(ivec.begin(), ivec.end());
			for (int i = 0; i < ivec.size(); i++)
				std::cout << ivec[i] << ' ';
			std::cout << std::endl;

		}
	}
}