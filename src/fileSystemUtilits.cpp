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


list<FileInfo> FileSystemUtilits::getFolderFiles(const string& rootFolderPath, string relativeFolderPath) {
	list<string> fileNames;
	list<string> subFolders;
	DIR* dirp = opendir((rootFolderPath + "/" + relativeFolderPath).c_str());
	if ( !dirp ) {
		// Directory does not exist
		throw DirectoryNotExistException();
	}

	struct dirent * dp;
	while ((dp = readdir(dirp)) != NULL) {
		string name = dp->d_name;
		if ( name == "." || name == ".."
			|| name == "CompareVolumeDemo"
			|| name[0] == '.'
		) {
			continue;
		}

		if ( name.size() > 0
			&& dp->d_type != DT_DIR
		) {
			fileNames.push_back(relativeFolderPath + name);
		}

		if (dp->d_type == DT_DIR) {
			// Parse sub-folders later
			subFolders.push_back(relativeFolderPath + name);
		}
	}
	closedir(dirp);

	list<FileInfo> result;
	for ( auto& fileName: fileNames ) {
		struct stat fileStat;
		if( stat((rootFolderPath + '/' + fileName).c_str(), &fileStat) != 0 )
		{
			// skip file
			cout << "Failed to load stat for file " << rootFolderPath << endl;
			continue;
		}

		// Should skip directories and "*.mdb" files (database)
		if ( fileStat.st_mode & S_IFREG
				&& (fileName.size() >= 4
					&& fileName.substr(fileName.size()-4) != ".mdb"
					|| fileName.size() < 4
				)
		) {
			FileInfo info;
			info.name = fileName;
			info.path = rootFolderPath + '/' + fileName;
			info.modificationTimestamp = static_cast<long>(fileStat.st_mtime);
			result.push_back(info);
		}
	}

	for ( auto& subFolder: subFolders ) {
		list<FileInfo> subfolderFileNames = getFolderFiles(rootFolderPath, subFolder + "/");
		result.insert(result.end(), subfolderFileNames.begin(), subfolderFileNames.end());
	}

	return move(result);
}
