/*
 * testOnelineFiles.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: pavel
 */

#include "testOnelineFiles.h"

#include <iostream>
#include <memory>
#include "volumeCompare.h"
#include "LCSIndexComparator.h"

using namespace std;

void TestOnelineFiles::run() {
	string path = "tests/assets/asset5";

	unique_ptr<IndexComparator> comparator = make_unique<LCSIndexComparator>();
	list<SimilarFiles> similarFiles = getSimilarFilesInFolder(path, 0.2, comparator);

	if ( similarFiles.size() != 1 ) {
		throw TestError("One file pair should be found for TestOnelineFiles");
	}
}
