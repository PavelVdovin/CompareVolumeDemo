/*
 * fileIndexArithmetic.h
 *
 * Algorithm used here is based on building hash for each line and is taken from https://www.osp.ru/pcworld/2006/10/3403705/
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef SRC_FILEINDEXARITHMETIC_H_
#define SRC_FILEINDEXARITHMETIC_H_

#include "fileIndex.h"
#include <vector>
using namespace std;

class FileIndexArithmetic: public FileIndex {

	vector<double> m_linesHash;

	// Coefficient is taken from https://www.osp.ru/pcworld/2006/10/3403705/
	const double m_charCoef = 1.1111111;

public:

	FileIndexArithmetic(const FileInfo& fileInfo): FileIndex(fileInfo) {}
	virtual ~FileIndexArithmetic() {}

	void calculate() override;

	double getHashByLine(int i) const {
		return m_linesHash.at(i);
	}

	int getHashSize() const {
		return m_linesHash.size();
	}

	const vector<double> getHash() const {
		return m_linesHash;
	}

private:

	double getHashForLine(const string& line);
};

#endif /* SRC_FILEINDEXARITHMETIC_H_ */
