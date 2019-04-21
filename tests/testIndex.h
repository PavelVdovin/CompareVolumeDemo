/*
 * testIndex.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef TESTS_TEST_INDEX_H_
#define TESTS_TEST_INDEX_H_

#include <iostream>
#include "test.h"

class TestIndex: public Test {

public:

	TestIndex(): Test("TestIndex") {}

	void run() override;
};



#endif /* TESTS_TEST_H_ */
