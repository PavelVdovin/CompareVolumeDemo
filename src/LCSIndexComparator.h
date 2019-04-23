/*
 * LCSIndexComparator.h
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#ifndef SRC_LCSINDEXCOMPARATOR_H_
#define SRC_LCSINDEXCOMPARATOR_H_

#include "indexComparator.h"

class LCSIndexComparator: public IndexComparator {
public:

	// Compare content based on LCS algorithm.
	virtual double compareContent(
		const FileIndexArithmetic& file1,
		const FileIndexArithmetic& file2,
		double maxDiff
	) const override;
};

#endif /* SRC_LCSINDEXCOMPARATOR_H_ */
