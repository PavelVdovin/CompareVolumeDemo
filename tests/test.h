/*
 * test.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef TESTS_TEST_H_
#define TESTS_TEST_H_
#include <string>
using namespace std;

class TestError: public exception {

	string m_message;

public:

	TestError(string message): m_message(message) {}

	virtual const char* what() const throw()
	{
		 return m_message.c_str();
	}
};

class Test {

	string m_name;

public:

	Test(string name): m_name(name) {}
	Test(const Test&) = delete;
	virtual ~Test() {}


	// Override your test here, throw exception if it fails
	virtual void run() = 0;

	string getName() const {
		return m_name;
	}
};



#endif /* TESTS_TEST_H_ */
