#include "copytest.h"

namespace myTinySTL
{
	namespace CopyTest
	{
		template <class T>
		struct display
		{
			void operator() (const T& x)
			{
				std::cout << x << ' ';
			}
		};

		void testcase1()
		{
			{
				std::cout << "********** This is copy test! **********" << std::endl;
				int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
				copy(ia + 2, ia + 7, ia);
				myTinySTL::for_each(ia, ia + 9, display<int>());
				std::cout << std::endl;
			}

			{
				int ia[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
				copy(ia + 2, ia + 7, ia + 4);
				myTinySTL::for_each(ia, ia + 9, display<int>());
				std::cout << std::endl;
			}

			{
				const char ccs[5] = { 'a', 'b', 'c', 'd', 'e' };
				char ccd[5];
				copy(ccs, ccs + 5, ccd);
				myTinySTL::for_each(ccd, ccd + 5, display<char>());
				std::cout << std::endl;

				const wchar_t cwcs[5] = { 'a', 'b', 'c', 'd', 'e' };
				wchar_t cwcd[5];
				copy(cwcs, cwcs + 5, cwcd);
				myTinySTL::for_each(cwcd, cwcd + 5, display<wchar_t>());
				std::cout << std::endl;

				int ia[5] = { 0, 1, 2, 3, 4 };
				copy(ia, ia + 5, ia);
				myTinySTL::for_each(ia, ia + 5, display<int>());
				std::cout << std::endl;

			}
			
		}
	}
}