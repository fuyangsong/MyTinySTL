#include "queuetest.h"

namespace myTinySTL
{
	namespace QueueTest
	{
		void testcase1()
		{
			std::cout << "**********This is queue test !************************" << std::endl;
			queue<int> dqueue;

			dqueue.push(1);
			dqueue.push(3);
			dqueue.push(4);
			dqueue.push(7);
			std::cout << "dqueue size= " << dqueue.size() << std::endl;
			std::cout << dqueue.front() << std::endl;
			dqueue.pop(); std::cout << dqueue.front() << std::endl;
			dqueue.pop(); std::cout << dqueue.front() << std::endl;
			dqueue.pop(); std::cout << dqueue.front() << std::endl;
			std::cout << "dqueue size= " << dqueue.size() << std::endl;
		}
	}
}