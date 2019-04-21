/*
 * indexComparator.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#include <string>
#include <cassert>
#include "LCSAlgorithm.h"
#include "indexComparator.h"

using namespace std;

// For comparing double numbers
#define EPS 0.00000001


double IndexComparator::compareMetadata(const FileIndexArithmetic& fileIndex1, const FileIndexArithmetic& fileIndex2) const {
	const FileInfo file1 = fileIndex1.getFileInfo();
	const FileInfo file2 = fileIndex2.getFileInfo();

	double timestampDiff = abs(file1.modificationTimestamp -file2.modificationTimestamp);
	int lcsLength = getLCSLength<string>(file1.name, file2.name);
	double lcsDiff = (double)lcsLength / max(file1.name.size(), file2.name.size());

	// return 1.0 when lcsDiff = 1.0 and timestampDiff = 0.0
	// for simplicity, define max timestamp diff as 1 year
	const long maxTimestampDiff = 60 * 60 * 24 * 365;
	double timestampDiffCost = timestampDiff < maxTimestampDiff
			? 1.0 - timestampDiff / maxTimestampDiff
			: 0.0;

	return lcsDiff * 0.5 + timestampDiffCost * 0.5;
}

double IndexComparator::compare(const FileIndexArithmetic& file1, const FileIndexArithmetic& file2, const double preferContentRatio) const {
	assert( preferContentRatio > -EPS && preferContentRatio < 1.0 + EPS );

	return preferContentRatio * compareContent(file1, file2)
		+ (1.0 - preferContentRatio) * compareMetadata(file1, file2);
}

