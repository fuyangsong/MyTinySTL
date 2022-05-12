#include <iostream>

#include "Test/PairTest.h"
#include "Test/TypeTraitsTest.h"
#include "Test/heaptest.h"
#include "Test/copytest.h"
#include "Test/VectorTest.h"
#include"Test/dequetest.h"
#include "Test/listtest.h"
#include "Test/priorityqueuetest.h"
#include "Test/stacktest.h"
#include "Test/queuetest.h"
#include "Test/hashsettest.h"

int main() {
	std::cout << __cplusplus << std::endl;
	myTinySTL::TypeTraitsTest::testAllCases();
	myTinySTL::PairTest::testAllCases();
	myTinySTL::HeapTest::testcase1();
	myTinySTL::CopyTest::testcase1();
	myTinySTL::VectorTest::testcase1();
	myTinySTL::DequeTest::testcase1();
	myTinySTL::ListTest::testcase1();
	myTinySTL::PriorityQueueTest::testcase1();
	myTinySTL::StackTest::testcase1();
	myTinySTL::QueueTest::testcase1();
	myTinySTL::HashsetTest::testcase1();


	system("pause");
	return 0;
}