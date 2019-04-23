/*
 * testCompareFiles.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */
#include <iostream>
#include <iostream>
#include <memory>

#include "testSubfolder.h"

#include "volumeCompare.h"
#include "LCSIndexComparator.h"

using namespace std;



void TestSubfolder::run() {
	string path = "tests/assets/asset4";

	unique_ptr<IndexComparator> comparator = make_unique<LCSIndexComparator>();
	list<SimilarFiles> similarFiles = getSimilarFilesInFolder(path, 0.00001, comparator);
	for ( auto& simFiles: similarFiles ) {
		cout << simFiles.file1 << " " << simFiles.file2 << " " << simFiles.diff << endl;
	}

	// Two files are fully equal
	if ( similarFiles.size() != 1 ) {
		throw TestError("One file pair should be found for TestSubfolder");
	}
}
