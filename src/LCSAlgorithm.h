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
 * Taking two arrays A and B with sizes A_size and B_size, find a length of the longest common subsequence.
 */
template<class T> int getLCSLength(const T* A, unsigned A_size, const T* B, unsigned B_size) {

	// allocate storage for one-dimensional arrays X and Y
	// for  storing dynamic programming intermediate results
	vector<int> X(B_size + 1, 0), Y(B_size + 1, 0);

	for (int i = A_size; i >= 0; i--) {
		for (int j = B_size; j >= 0; j--) {
			if ( i == A_size || j == B_size ) {
				X[j] = 0;
			} else if ( A[i] == B[j] ) {
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
