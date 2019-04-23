/*
 * indexComparator.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#include <iostream>
#include <string>
#include <cassert>
#include "LCSAlgorithm.h"
#include "indexComparator.h"

using namespace std;

// For comparing double numbers
#define EPS 0.00000001

string getShortFileName(const string& nameWithSubfolders) {
	std::size_t found = nameWithSubfolders.find_last_of("/");
	return move(
		found == string::npos
		? nameWithSubfolders
		: nameWithSubfolders.substr(found+1)
	);
}


double IndexComparator::compareMetadata(const FileIndexArithmetic& fileIndex1, const FileIndexArithmetic& fileIndex2) const {
	const FileInfo file1 = fileIndex1.getFileInfo();
	const FileInfo file2 = fileIndex2.getFileInfo();

	// get the file name without subDirectories
	string shortName1 = getShortFileName(file1.name);
	string shortName2 = getShortFileName(file2.name);

	double timestampDiff = abs(file1.modificationTimestamp -file2.modificationTimestamp);
	int lcsLength = getLCSLength<char>(
		shortName1.c_str(),
		shortName1.size(),
		shortName2.c_str(),
		shortName2.size()
	);

	double lcsDiff = (double)lcsLength / max(shortName1.size(), shortName2.size());

	// return 1.0 when lcsDiff = 1.0 and timestampDiff = 0.0
	// for simplicity, define max timestamp diff as 1 year
	const long maxTimestampDiff = 60 * 60 * 24 * 365;
	double timestampDiffCost = timestampDiff < maxTimestampDiff
			? timestampDiff / maxTimestampDiff
			: 1.0;

	return (1.0 - lcsDiff) * 0.5 + timestampDiffCost * 0.5;
}

double IndexComparator::compare(
		const FileIndexArithmetic& file1,
		const FileIndexArithmetic& file2,
		double maxDiff,
		const double preferContentRatio
) const {
	assert( preferContentRatio > -EPS && preferContentRatio < 1.0 + EPS );

	return preferContentRatio * compareContent(file1, file2, maxDiff / preferContentRatio)
		+ (1.0 - preferContentRatio) * compareMetadata(file1, file2);
}

