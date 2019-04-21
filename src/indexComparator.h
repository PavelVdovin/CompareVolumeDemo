/*
 * indexComparator.h
 *
 *	Done only for FileIndexArithmetic now.
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#ifndef SRC_INDEXCOMPARATOR_H_
#define SRC_INDEXCOMPARATOR_H_

#include "fileIndexArithmetic.h"

class IndexComparator {
public:
	IndexComparator() {}
	virtual ~IndexComparator() {}

	// Compare content only, without files metadata.
	// Should return heuristic value between 0.0 and 1.0.
	// 0.0 = content is equal.
	// 1.0 = content is fully different.
	virtual double compareContent(const FileIndexArithmetic& file1, const FileIndexArithmetic& file2) const = 0;

	// Compare metadata only.
	// Default implementation compares file names using LCS and calcs modification  dates abs diff.
	// Should return heuristic value between 0.0 and 1.0.
	// 0.0 = content is equal.
	// 1.0 = content is fully different.
	virtual double compareMetadata(const FileIndexArithmetic& file1, const FileIndexArithmetic& file2) const;

	// Compare a * compareContent + (1-a) * compareMetadata.
	// a in [0.0, 1.0].
	// Default a = 0.9.
	// Should return heuristic value between 0.0 and 1.0.
	// 0.0 = content is equal.
	// 1.0 = content is fully different.
	virtual double compare(
			const FileIndexArithmetic& file1,
			const FileIndexArithmetic& file2,
			const double preferContentRatio = 0.9
	) const;
};

#endif /* SRC_INDEXCOMPARATOR_H_ */
