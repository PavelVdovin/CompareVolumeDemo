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
#include "testDB.h"
#include "testVolumeCompare.h"
#include "testSubfolder.h"

using namespace std;

class TestRunner {

	list<unique_ptr<Test>> m_tests;

public:

	TestRunner() {
		m_tests.push_back(make_unique<TestIndex>());
		m_tests.push_back(make_unique<TestCompareFiles>());
		m_tests.push_back(make_unique<TestDB>());
		m_tests.push_back(make_unique<TestVolumeCompare>());
		m_tests.push_back(make_unique<TestSubfolder>());
	}

	void run();
};



#endif /* TESTS_TEST_H_ */
