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
#include <iostream>

using namespace std;

void FileIndexArithmetic::calculate() {
	if ( m_ownHash ) {
		delete[] m_linesHash;
	}

	std::ifstream infile(m_fileInfo.path);
	std::string line;
	m_hashSize = 0;
	while (getline(infile, line)) {
		++m_hashSize;
	}

	infile.clear();
	infile.seekg(0, infile.beg);

	m_linesHash = new double[m_hashSize];
	int i = 0;
	while (getline(infile, line)) {
		m_linesHash[i] = generateHashForLine(line);
		++i;
	}

	m_ownHash = true;
}

double FileIndexArithmetic::generateHashForLine(const string& line) {
	double k = 0.0;
	for ( const char& ch: line ) {
		k += ch * m_charCoef;
	}

	return move(k);
}

void FileIndexArithmetic::setIndexFromMemory(int dataSize, double* data) {
	if ( m_ownHash ) {
		delete[] m_linesHash;
	}

	m_hashSize = dataSize;
	m_linesHash = data;
	m_ownHash = false;
}
