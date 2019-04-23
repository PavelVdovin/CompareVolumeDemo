/*
* volumeCompare.h
*
* This file presents functions to perform comparison of files presented in the specified directory.
*
*  Created on: Apr 21, 2019
*      Author: pavel
*/


#ifndef SRC_VOLUME_COMPARE_H_
#define SRC_VOLUME_COMPARE_H_

#include <string>
#include <list>
#include <memory>
#include "indexComparator.h"

using namespace std;

struct SimilarFiles {
	string file1;
	string file2;
	double diff;
};

list<SimilarFiles> getSimilarFilesInFolder(
	const string& folderPath,
	double maxDiff, // from 0.0 to 1.0
	const unique_ptr<IndexComparator>& indexComparator,
	bool disableExtensionOptimization = false
);

class FileNotFoundException: public exception {
	virtual const char* what() const throw()
	{
		return "The specified file does not exist";
	}
};

list<SimilarFiles> getSimilarForSpecifiedFile(
	const string& folderPath,
	const string& filePath,
	double maxDiff, // from 0.0 to 1.0
	const unique_ptr<IndexComparator>& indexComparator,
	bool disableExtensionOptimization = false
);

#endif
