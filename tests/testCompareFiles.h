/*
 * testCompareFiles.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef TESTS_TEST_COMPARE_FILES_H_
#define TESTS_TEST_COMPARE_FILES_H_

#include <iostream>
#include "test.h"

class TestCompareFiles: public Test {

public:

	TestCompareFiles(): Test("TestCompareFiles") {}

	void run() override;
};



#endif /* TESTS_TEST_H_ */
