/*
 * testCompareFiles.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */
#include <iostream>
#include <cmath>

#include "testCompareFiles.h"

#include "fileSystemUtilits.h"
#include "fileIndexArithmetic.h"
#include "LCSIndexComparator.h"

using namespace std;

// For comparing double numbers
#define EPS 0.00000001


void TestCompareFiles::run() {
	string path = "tests/assets/asset2";

	vector<FileIndexArithmetic> fileIndexes;
	fileIndexes.reserve(3);
	int ver1Index, ver2Index, ver3Index;
	list<FileInfo> files = FileSystemUtilits::getFolderFiles(path);
	for ( auto& file: files ) {
		cout << file.name << ": " << file.path << ": " << file.modificationTimestamp << endl;
		FileIndexArithmetic index(file);
		index.calculate();
		fileIndexes.push_back(move(index));

		if ( index.getFileInfo().name == "test_ver1.txt" ) {
			ver1Index = fileIndexes.size() - 1;
		} else if ( index.getFileInfo().name == "test_ver2.txt" ) {
			ver2Index = fileIndexes.size() - 1;
		} else {
			ver3Index = fileIndexes.size() - 1;
		}
	}

	if ( fileIndexes.size() != 3 ) {
		throw TestError("3 files should be found for TestCompareFiles");
	}

	double ver12CompareCost, ver13CompareCost, ver23CompareCost;
	LCSIndexComparator comparator;

	ver12CompareCost = comparator.compareContent(fileIndexes[ver1Index], fileIndexes[ver2Index], 1.0);
	ver13CompareCost = comparator.compareContent(fileIndexes[ver1Index], fileIndexes[ver3Index], 1.0);
	ver23CompareCost = comparator.compareContent(fileIndexes[ver2Index], fileIndexes[ver3Index], 1.0);

	if ( ver23CompareCost > 0.001 ) {
		throw TestError("Versions 2 and 3 should have equal content");
	}

	if ( ver12CompareCost < 0.001 ) {
		throw TestError("Versions 1 and 2 should not have equal content");
	}

	if ( ver13CompareCost < 0.001 ) {
		throw TestError("Versions 1 and 3 should not have equal content");
	}

	// Compare full
	ver12CompareCost = comparator.compare(fileIndexes[ver1Index], fileIndexes[ver2Index], 1.0);
	ver13CompareCost = comparator.compare(fileIndexes[ver1Index], fileIndexes[ver3Index], 1.0);
	ver23CompareCost = comparator.compare(fileIndexes[ver2Index], fileIndexes[ver3Index], 1.0);

	if ( ver23CompareCost > 0.1 ) {
		throw TestError("Versions 2 and 3 should be equal for < 10%");
	}

	if ( ver12CompareCost < 0.1 ) {
		throw TestError("Versions 1 and 2 should be equal for > 10%");
	}

	if ( ver13CompareCost < 0.1 ) {
		throw TestError("Versions 1 and 3 should be equal for > 10%");
	}
}
