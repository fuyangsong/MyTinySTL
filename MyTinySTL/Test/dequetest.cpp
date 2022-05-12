#include "dequetest.h"

namespace myTinySTL
{
	namespace DequeTest
	{
		void testcase1()
		{
			std::cout << "******This is deque test !********************" << std::endl;
			deque<int, allocator<int>, 8> ideq(20, 9);
			std::cout << "size = " << ideq.size() << std::endl;

			for (int i = 0; i < ideq.size(); ++i)
				ideq[i] = i;

			for (int i = 0; i < ideq.size(); ++i)
				std::cout << ideq[i] << ' ';
			std::cout << std::endl;

			for (int i = 0; i < 3; ++i)
				ideq.push_back(i);

			for (int i = 0; i < ideq.size(); ++i)
				std::cout << ideq[i] << ' ';
			std::cout << std::endl;
			std::cout << "size = " << ideq.size() << std::endl;

			ideq.push_back(3);
			for (int i = 0; i < ideq.size(); ++i)
				std::cout << ideq[i] << ' ';
			std::cout << std::endl;
			std::cout << "size = " << ideq.size() << std::endl;

			ideq.push_front(99);
			for (int i = 0; i < ideq.size(); ++i)
				std::cout << ideq[i] << ' ';
			std::cout << std::endl;
			std::cout << "size = " << ideq.size() << std::endl;

			ideq.push_front(98);
			ideq.push_front(97);
			for (int i = 0; i < ideq.size(); ++i)
				std::cout << ideq[i] << ' ';
			std::cout << std::endl;
			std::cout << "size = " << ideq.size() << std::endl;

			deque<int, allocator<int>, 8>::iterator itr;
			itr = find(ideq.begin(), ideq.end(), 99);
			std::cout << *itr << std::endl;
			std::cout << *(itr.cur) << std::endl;
		}
	}
}