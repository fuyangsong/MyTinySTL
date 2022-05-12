#include "priorityqueuetest.h"

namespace myTinySTL
{
	namespace PriorityQueueTest
	{
		void testcase1()
		{
			std::cout << "*************This is priority_queue test !******************" << std::endl;
			int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
			priority_queue<int> ipq(ia, ia + 9);
			std::cout << "size=" << ipq.size() << std::endl;

			for (int i = 0; i < ipq.size(); i++)
				std::cout << ipq.top() << ' ';
			std::cout << std::endl;

			while (!ipq.empty())
			{
				std::cout << ipq.top() << ' ';
				ipq.pop();
			}
			std::cout << std::endl;
		}
		
	}
}