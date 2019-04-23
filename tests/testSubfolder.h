/*
 * testSubfolder.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef TESTS_TEST_SUBFOLDER_H_
#define TESTS_TEST_SUBFOLDER_H_

#include <iostream>
#include "test.h"

class TestSubfolder: public Test {

public:

	TestSubfolder(): Test("TestSubfolder") {}

	void run() override;
};



#endif /* TESTS_TEST_SUBFOLDER_H_ */
