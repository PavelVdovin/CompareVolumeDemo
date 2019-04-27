/*
 * LCSIndexComparator.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#include "LCSIndexComparator.h"
#include "LCSAlgorithm.h"

#include <iostream>

double LCSIndexComparator::compareContent(
	const FileIndexArithmetic& fileIndex1,
	const FileIndexArithmetic& fileIndex2
) const {
	double lcsLength = getLCSLength<double>(
		fileIndex1.getHash(),
		fileIndex1.getHashSize(),
		fileIndex2.getHash(),
		fileIndex2.getHashSize()
	);

	return 1.0 - (double) lcsLength / max(fileIndex1.getHashSize(), fileIndex2.getHashSize());
}
