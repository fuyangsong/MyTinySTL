#pragma once
#ifndef _VECTOR_TEST_H_
#define _VECTOR_TEST_H_

#include "../vector.h"

#include <vector>
#include <list>
#include <array>
#include <cassert>
#include<iostream>
#include <iterator>
#include <string>
#include <algorithm>

namespace myTinySTL 
{
	namespace VectorTest 
	{
		template<class T>
		using stdVec = std::vector < T >;

		template<class T>
		using tsVec = myTinySTL::vector < T >;

		void testcase1();
	}
}

#endif

