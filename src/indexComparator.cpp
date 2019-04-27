/*
 * indexComparator.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <fstream>
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

	double contentDiff;
	int size1 = file1.getHashSize();
	int size2 = file2.getHashSize();
	int maxSize = max(size1, size2);
	double maxContentDiff = maxDiff / preferContentRatio;
	if ( maxSize == 0 ) {
		// No content
		contentDiff = 0.0;
	} else if ( (double)min(size1, size2) / maxSize < 1.0 - maxContentDiff ) {
		// No reason to compare files
		contentDiff = 1.0;
	} else if ( maxSize == 1 ) {
		// Both files have no line breaks - use LCS to compare text if hashes are not equal
		contentDiff = compareContentText(file1, file2, maxContentDiff);
	} else  {
		// Compare line hashes.
		contentDiff = compareContent(file1, file2);
	}

	double metadataDiff = compareMetadata(file1, file2);

	return preferContentRatio * contentDiff
		+ (1.0 - preferContentRatio) * metadataDiff;
}

double IndexComparator::compareContentText(
		const FileIndexArithmetic& file1,
		const FileIndexArithmetic& file2,
		double maxDiff
) const {
	// Call only for files with one line
	assert(file1.getHashSize() == 1 && file2.getHashSize() == 1);
	if ( abs(file1.getHash()[0] - file2.getHash()[0]) < EPS ) {
		// Hashes are equal
		return 0.0;
	}

	// Read files and save content
	string file1Content;
	string file2Content;

	std::ifstream infile1(file1.getFileInfo().path);
	getline(infile1, file1Content);
	infile1.close();

	std::ifstream infile2(file2.getFileInfo().path);
	getline(infile2, file2Content);
	infile2.close();

	// Compare lines length first
	int size1 = file1Content.size();
	int size2 = file2Content.size();
	int maxSize = max(size1, size2);
	if ( maxSize == 0 ) {
		// No content
		return 0.0;
	} else if ( (double)min(size1, size2) / maxSize < 1.0 - maxDiff ) {
		// No reason to compare files
		return 1.0;
	}

	int lcsLength = getLCSLength<char>(
		file1Content.c_str(),
		file1Content.size(),
		file2Content.c_str(),
		file2Content.size()
	);

	return 1.0 - (double)lcsLength / maxSize;
}

