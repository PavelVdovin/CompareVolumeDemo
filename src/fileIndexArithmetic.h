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

	// Keeping data in raw array to be able to access db memory in a fast way
	int m_hashSize;
	double* m_linesHash;

	// hash may be owned by db or directly by the indexer
	bool m_ownHash;

	// Coefficient is taken from https://www.osp.ru/pcworld/2006/10/3403705/
	// (it is moved to long to effectively store data in database)
	static constexpr double m_charCoef = 1.1111111;

public:

	FileIndexArithmetic(const FileInfo& fileInfo):
		FileIndex(fileInfo),
		m_hashSize(0),
		m_linesHash(nullptr),
		m_ownHash(false)
	{}

	FileIndexArithmetic(FileIndexArithmetic& other) = delete;
	FileIndexArithmetic& operator=(const FileIndexArithmetic&) = delete;


	FileIndexArithmetic(FileIndexArithmetic&& other)
	: FileIndex(move(other.m_fileInfo)){
		m_hashSize = other.m_hashSize;
		m_linesHash = other.m_linesHash;
		m_ownHash = other.m_ownHash;

		other.m_ownHash = false;
	}

	virtual ~FileIndexArithmetic() {
		if ( m_ownHash ) {
			delete[] m_linesHash;
		}
	}

	void calculate() override;

	long getHashByLine(int i) const {
		return m_linesHash[i];
	}

	int getHashSize() const {
		return m_hashSize;
	}

	double* getHash() const {
		return m_linesHash;
	}

	// Set index data from memory
	void setIndexFromMemory(int dataSize, double* data);

private:

	double generateHashForLine(const string& line);
};

#endif /* SRC_FILEINDEXARITHMETIC_H_ */
