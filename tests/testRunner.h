/*
 * testRunner.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef TESTS_TEST_RUNNER_H_
#define TESTS_TEST_RUNNER_H_

#include <list>
#include <memory>

#include "testIndex.h"
#include "testCompareFiles.h"

using namespace std;

class TestRunner {

	list<unique_ptr<Test>> m_tests;

public:

	TestRunner() {
		m_tests.push_back(make_unique<TestIndex>());
		m_tests.push_back(make_unique<TestCompareFiles>());
	}

	void run();
};



#endif /* TESTS_TEST_H_ */
