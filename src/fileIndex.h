/*
 * fileIndex.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef SRC_FILEINDEX_H_
#define SRC_FILEINDEX_H_

#include <string>
#include "fileSystemUtilits.h"
using namespace std;

class FileIndex {

protected:

	FileInfo m_fileInfo;

public:

	FileIndex(const FileInfo& fileInfo): m_fileInfo(fileInfo) {}
	virtual ~FileIndex() {}

	// Calculate and keep the index
	virtual void calculate() = 0;

	const FileInfo getFileInfo() const {
		return m_fileInfo;
	}

};

#endif /* SRC_FILEINDEX_H_ */
