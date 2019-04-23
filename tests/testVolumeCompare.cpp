/*
 * testVolumeCompare.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#include "testVolumeCompare.h"

#include <iostream>
#include <memory>
#include "volumeCompare.h"
#include "LCSIndexComparator.h"

using namespace std;

#define EPS 0.00000001


void TestVolumeCompare::run() {
	string path = "tests/assets/asset3";

	unique_ptr<IndexComparator> comparator = make_unique<LCSIndexComparator>();
	list<SimilarFiles> similarFiles = getSimilarFilesInFolder(path, 0.1, comparator);
	for ( auto& simFiles: similarFiles ) {
		cout << simFiles.file1 << " " << simFiles.file2 << " " << simFiles.diff << endl;
	}

	if ( similarFiles.size() != 1 ) {
		throw TestError("Only one file pair should be found for TestVolumeCompare");
	}

	similarFiles = getSimilarFilesInFolder(path, 0.9, comparator);
	for ( auto& simFiles: similarFiles ) {
		cout << simFiles.file1 << " " << simFiles.file2 << " " << simFiles.diff << endl;
	}

	if ( similarFiles.size() != 3 ) {
		throw TestError("3 file pair should be found for TestVolumeCompare");
	}

	// Searching similar files for test_ver1.txt
	similarFiles = getSimilarForSpecifiedFile(path, "test_ver1.txt", 0.1, comparator);
	for ( auto& simFiles: similarFiles ) {
		cout << simFiles.file1 << " " << simFiles.file2 << " " << simFiles.diff << endl;
	}

	if ( similarFiles.size() != 0 ) {
		throw TestError("No file pairs should be found for TestVolumeCompare");
	}

}
