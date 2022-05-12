#include "hashsettest.h"

namespace myTinySTL
{
	namespace HashsetTest
	{
		void testcase1()
		{
			hash_set<int> Set;
			Set.insert(59);
			Set.insert(63);
			Set.insert(108);
			Set.insert(2);
			Set.insert(53);
			Set.insert(55);

			hash_set<int>::iterator ite1 = Set.begin();
			hash_set<int>::iterator ite2 = Set.end();
			for (; ite1 != ite2; ++ite1)
			{
				std::cout << *ite1 << ' ';
			}
			std::cout << std::endl;
		}
	}
}