/*
 * LCSIndexComparator.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#include "LCSIndexComparator.h"
#include "LCSAlgorithm.h"

#include <iostream>

double LCSIndexComparator::compareContent(const FileIndexArithmetic& fileIndex1, const FileIndexArithmetic& fileIndex2) const {
	double lcsLength = getLCSLength<vector<double>>(fileIndex1.getHash(), fileIndex2.getHash());

	return 1.0 - (double) lcsLength / max(fileIndex1.getHash().size(), fileIndex2.getHash().size());
}
