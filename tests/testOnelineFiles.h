/*
 * testOnelineFiles.h
 *
 *  Created on: Apr 27, 2019
 *      Author: pavel
 */

#ifndef TESTS_TEST_ONELINE_FILES_H_
#define TESTS_TEST_ONELINE_FILES_H_

#include "test.h"

class TestOnelineFiles: public Test {

public:

	TestOnelineFiles(): Test("TestOnelineFiles") {}

	void run() override;
};



#endif /* TESTS_TEST_ONELINE_FILES_H_ */
