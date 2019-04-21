/*
 * testHash.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */
#include <iostream>
#include <cmath>

#include "../src/fileSystemUtilits.h"
#include "fileIndexArithmetic.h"
#include "testIndex.h"

using namespace std;

// For comparing double numbers
#define EPS 0.00000001


void TestIndex::run() {
	string path = "tests/assets/asset1";

	list<FileIndexArithmetic> fileIndexes;
	list<FileInfo> files = FileSystemUtilits::getFolderFiles(path);
	for ( auto& file: files ) {
		cout << file.name << ": " << file.path << ": " << file.modificationTimestamp << endl;
		FileIndexArithmetic index(file);
		index.calculate();
		fileIndexes.push_back(move(index));
	}

	if ( fileIndexes.size() != 2 ) {
		throw TestError("Only two files should be found for TestHash");
	}

	FileIndexArithmetic& first = fileIndexes.front();
	FileIndexArithmetic& second = fileIndexes.back();
	if ( first.getHashSize() != 2 ) {
		throw TestError("The first index should have hash with size 2");
	}

	if ( second.getHashSize() != 3 ) {
		throw TestError("The second index should have hash with size 3");
	}

	if ( abs(first.getHashByLine(0) - second.getHashByLine(0)) > EPS ) {
		throw TestError("Hashes should be equal for files first lines");
	}

	if ( abs(first.getHashByLine(1) - second.getHashByLine(1)) < EPS ) {
		throw TestError("Hashes should be different for files first lines");
	}

	if ( abs(first.getHashByLine(1) - second.getHashByLine(2)) > EPS ) {
		throw TestError("Hashes should be equal for files last lines");
	}
}
