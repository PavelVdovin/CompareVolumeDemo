/*
 * fileSystemUtilits.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: pavel
 */

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

#include "fileSystemUtilits.h"


list<FileInfo> FileSystemUtilits::getFolderFiles(const string& path) {
	list<string> fileNames;
	DIR* dirp = opendir(path.c_str());
	if ( !dirp ) {
		// Directory does not exist
		throw DirectoryNotExistException();
	}

	struct dirent * dp;
	while ((dp = readdir(dirp)) != NULL) {
		string name = dp->d_name;
		if ( name != "." && name != ".."
				&& name.size() > 0 && name[0] != '.'
		) {
			fileNames.push_back(name);
		}
	}
	closedir(dirp);

	list<FileInfo> result;
	for ( auto& fileName: fileNames ) {
		struct stat fileStat;
		if( stat((path + '/' + fileName).c_str(), &fileStat) != 0 )
		{
			// skip file
			cout << "Failed to load stat for file " << path << endl;
			continue;
		}

		if ( fileStat.st_mode & S_IFREG ) {
			FileInfo info;
			info.name = fileName;
			info.path = path + '/' + fileName;
			info.modificationTimestamp = static_cast<long>(fileStat.st_mtime);
			result.push_back(info);
		}
	}

	return move(result);
}
