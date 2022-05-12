#include "stacktest.h"

namespace myTinySTL
{
	namespace StackTest
	{
		void testcase1()
		{
			std::cout << "**********This is stack test !************************" << std::endl;
			stack<int> dstack1;

			dstack1.push(1);
			dstack1.push(3);
			dstack1.push(4);
			dstack1.push(7);
			std::cout << "dstack1 size= " << dstack1.size() << std::endl;
			std::cout << dstack1.top() << std::endl;
			dstack1.pop(); std::cout << dstack1.top() << std::endl;
			dstack1.pop(); std::cout << dstack1.top() << std::endl;
			dstack1.pop(); std::cout << dstack1.top() << std::endl;
			std::cout << "dstack1 size= " << dstack1.size() << std::endl;
		}
	}
}