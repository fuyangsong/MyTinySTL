#include "VectorTest.h"

namespace myTinySTL
{
	namespace VectorTest
	{
		void testcase1()
		{
			std::cout << "************This is vector test! **********************" << std::endl;
			tsVec<int> tv(2, 9);
			stdVec<int> sv(2, 9);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			tv.push_back(1);
			sv.push_back(1);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			tv.push_back(2);
			sv.push_back(2);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			tv.push_back(3);
			sv.push_back(3);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			tsVec<int>::iterator itite = find(tv.begin(), tv.end(), 1);
			if (itite != tv.end()) tv.erase(itite);
			stdVec<int>::iterator isite = std::find(sv.begin(), sv.end(), 1);
			if (isite != sv.end()) sv.erase(isite);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			itite = find(tv.begin(), tv.end(), 2);
			if (itite != tv.end()) tv.insert(itite, 6, 7);
			isite = std::find(sv.begin(), sv.end(), 2);
			if (isite != sv.end()) sv.insert(isite, 6, 7);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			tv.resize(5);
			sv.resize(5);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			tv.resize(20);
			sv.resize(20);
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;

			for (int i = 0; i < tv.size(); i++)
			{
				std::cout << tv[i] << ' ';
			}
			std::cout << std::endl;

			for (int i = 0; i < sv.size(); i++)
			{
				std::cout << sv[i] << ' ';
			}
			std::cout << std::endl;


			tv.clear();
			sv.clear();
			std::cout << "my size = " << tv.size() << std::endl;
			std::cout << "my capacity =" << tv.capacity() << std::endl;
			std::cout << "std size = " << sv.size() << std::endl;
			std::cout << "std capacity =" << sv.capacity() << std::endl;


		}
	}
}