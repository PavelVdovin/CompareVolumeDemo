/*
 * testRunner.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#include "testRunner.h"

void TestRunner::run() {
	for (auto& test: m_tests) {
		try {
			cout << "Running test '" << test->getName() << "'" << endl;
			test->run();
			cout << "Test '" << test->getName() << "' is passed" << endl;
		} catch (exception& ex) {
			std::cout << "Test '" << test->getName() << "' failed: " << ex.what() << std::endl;
		}
	}
}



