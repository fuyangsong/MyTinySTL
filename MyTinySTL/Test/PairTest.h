#pragma once
#ifndef _PAIR_TEST_H_
#define _PAIR_TEST_H_

#include "TestUtil.h"

#include "../utility.h"
#include <utility>

#include <cassert>
#include <iostream>
#include <string>

namespace myTinySTL 
{
	namespace PairTest 
	{
		template<typename T>
		using stdPair = std::pair < T, T >;
		template<typename T>
		using tsPair = myTinySTL::pair < T, T >;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();

		void testAllCases();
	}
}
#endif
