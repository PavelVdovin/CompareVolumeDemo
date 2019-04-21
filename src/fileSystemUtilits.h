/*
 * fileSystemUtilits.h
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#ifndef SRC_FILESYSTEMUTILITS_H_
#define SRC_FILESYSTEMUTILITS_H_

#include <list>
#include <string>
using namespace std;

class DirectoryNotExistException: public exception {
	virtual const char* what() const throw()
	{
		return "The specified directory does not exist";
	}
};

struct FileInfo {
	string name;
	string path;
	long modificationTimestamp;
};

class FileSystemUtilits {
public:

	// Get list of files with paths
	static list<FileInfo> getFolderFiles(const string& path);
};


#endif /* SRC_FILESYSTEMUTILITS_H_ */
