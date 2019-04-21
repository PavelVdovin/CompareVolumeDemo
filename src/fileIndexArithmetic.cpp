/*
 * fileIndexArithmetic.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#include "fileIndexArithmetic.h"
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

void FileIndexArithmetic::calculate() {
	std::ifstream infile(m_fileInfo.path);
	std::string line;
	while (getline(infile, line)) {
		m_linesHash.push_back(getHashForLine(line));
	}
}

double FileIndexArithmetic::getHashForLine(const string& line) {
	double k = 0;
	for ( const char& ch: line ) {
		k += ch * m_charCoef;
	}

	return move(k);
}
