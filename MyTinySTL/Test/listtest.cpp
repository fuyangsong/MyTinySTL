#include "listtest.h"

namespace myTinySTL
{
	namespace ListTest
	{
		void testcase1()
		{
			std::cout << "***************This is list test !*********************" << std::endl;
			list<int> ilist;
			std::cout << "size= " << ilist.size() << std::endl;

			ilist.push_back(0);
			ilist.push_back(1);
			ilist.push_back(2);
			ilist.push_back(3);
			ilist.push_back(4);
			std::cout << "size=" << ilist.size() << std::endl;

			list<int>::iterator ite;
			for (ite = ilist.begin(); ite != ilist.end(); ++ite)
				std::cout << *ite << ' ';
			std::cout << std::endl;

			ite = find(ilist.begin(), ilist.end(), 3);
			if (ite != ilist.end())
				ilist.insert(ite, 99);

			std::cout << "size=" << ilist.size() << std::endl;
			std::cout << *ite << std::endl;

			for (ite = ilist.begin(); ite != ilist.end(); ++ite)
				std::cout << *ite << ' ';
			std::cout << std::endl;

			ite = find(ilist.begin(), ilist.end(), 1);
			if (ite != ilist.end())
				std::cout << *(ilist.erase(ite)) << std::endl;

			for (ite = ilist.begin(); ite != ilist.end(); ++ite)
				std::cout << *ite << ' ';
			std::cout << std::endl;
		}
	}
}