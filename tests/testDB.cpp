/*
 * testDB.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include "testDB.h"

#include "../src/DBAdapter.h"
#include "fileSystemUtilits.h"
#include "fileIndexArithmetic.h"

using namespace std;

#define EPS 0.00000001


void TestDB::run() {
	string path = "tests/assets/asset3";

	DBAdapter dbAdapter;
	dbAdapter.parseDirectory(path);

	list<FileInfo> files = FileSystemUtilits::getFolderFiles(path);

	for ( auto& file: files ) {
		FileIndexArithmetic index = dbAdapter.getFileIndex(file.name);

		if ( index.getFileInfo().name != file.name ) {
			throw TestError(string("Database should contain index for file from directory: ") + file.name);
		}

		FileIndexArithmetic indexCalculated(file);
		indexCalculated.calculate();

		if ( indexCalculated.getHashSize() != index.getHashSize() ) {
			throw TestError(string("Index size from database should be the same as calculated index for: ") + file.name);
		}

		// Comparing indexes
		for ( int i = 0; i < indexCalculated.getHashSize(); ++ i ) {
			if ( abs(indexCalculated.getHash()[0] - index.getHash()[0]) > EPS ) {
				throw TestError(string("Index from database should be the same as calculated index for: ") + file.name);
			}
		}
	}

	dbAdapter.closeConnection();

	const string newFileName = "test_ver4.txt";
	// Copy file
	ifstream source(path + "/test_ver1.txt");
	ofstream dest(path + "/" + newFileName);

	dest << source.rdbuf();

	source.close();
	dest.close();

	dbAdapter.parseDirectory(path);

	FileIndexArithmetic index = dbAdapter.getFileIndex(newFileName);

	if ( index.getFileInfo().name != newFileName ) {
		throw TestError(string("Database should contain index for file from directory: ") + newFileName);
	}

	dbAdapter.closeConnection();

	remove((path + "/test_ver4.txt").c_str());
}
