/*
* LCSAlgorithm.h
*
* Implements a dynamic programming version of the longest-common-subsequence problem.
* Both arithmetic and text-based sequences may be used.
*
* P.S. algorithm is taken based on http://algolist.ru/search/lcs/simple_lcs.php
*
* Calculates only the length of the found LCS.
*
*  Created on: Apr 21, 2019
*      Author: pavel
*/


#ifndef SRC_LCS_ALGORITHM_H_
#define SRC_LCS_ALGORITHM_H_

#include<vector>
using namespace std;

/*
 * Taking two containers A and B, find a length of the longest common subsequence.
 * Assumptions:
 * 	- A and B should have method size()
 * 	- A and B should have method at, which returns comparable elements.
 */
template<class T> int getLCSLength(const T& A, const T& B) {

	// allocate storage for one-dimensional arrays X and Y
	// for  storing dynamic programming intermediate results
	vector<int> X(B.size() + 1, 0), Y(B.size() + 1, 0);

	for (int i = A.size(); i >= 0; i--) {
		for (int j = B.size(); j >= 0; j--) {
			if ( i == A.size() || j == B.size() ) {
				X[j] = 0;
			} else if ( A.at(i) == B.at(j) ) {
				X[j] = 1 + Y[j+1];
			} else {
				X[j] = max(Y[j], X[j+1]);
			}
		}
		// This is optimization to be able to use one-dimension arrays, not a matrix
		Y = X;
	}

	return X[0];
}

#endif /* SRC_LCS_ALGORITHM_H_ */
