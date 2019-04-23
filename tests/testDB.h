/*
 * testDB.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef TESTS_TEST_DB_H_
#define TESTS_TEST_DB_H_

#include "test.h"

class TestDB: public Test {

public:

	TestDB(): Test("TestDB") {}

	void run() override;
};



#endif /* TESTS_TEST_DB_H_ */
