#include "volumeCompare.h"
#include "DBAdapter.h"
#include <iostream>

using namespace std;

bool filesHasSimilarExtension(const string& fileName1, const string& fileName2) {
	std::size_t dotIndex1 = fileName1.find_last_of(".");
	std::size_t dotIndex2 = fileName2.find_last_of(".");

	return dotIndex1 != string::npos
		&& dotIndex2 != string::npos
		&& fileName1.substr(dotIndex1) == fileName2.substr(dotIndex2)
	;
}

list<SimilarFiles> getSimilarFilesInFolder(
		const string& folderPath,
		double maxDiff,
		const unique_ptr<IndexComparator>& indexComparator,
		bool disableExtensionOptimization
) {
	list<SimilarFiles> result;
	DBAdapter dbAdapter;

	cout << "Processing folder " << folderPath << endl;
	dbAdapter.parseDirectory(folderPath);
	cout << "Folder processing finished" << endl;

	list<FileInfo> files = FileSystemUtilits::getFolderFiles(folderPath);

	cout << "Comparing files" << endl;

	vector<FileIndexArithmetic> indexes;
	indexes.reserve(files.size());
	for ( auto& file: files ) {
		indexes.push_back(dbAdapter.getFileIndex(file.name));
	}

	if ( files.size() == 0 ) {
		return move(result);
	}

	for ( int i = 0; i < indexes.size()-1; ++i ) {
		for ( int j = i+1; j < indexes.size(); ++j ) {

			// skip if we compare only files with the same extension
			if ( !disableExtensionOptimization
				&& !filesHasSimilarExtension(
						indexes[i].getFileInfo().name,
						indexes[j].getFileInfo().name
					)
			) {
				continue;
			}

			double diff = indexComparator->compare(indexes[i], indexes[j], maxDiff);
			if ( diff < maxDiff ) {
				result.push_back({
					indexes[i].getFileInfo().name,
					indexes[j].getFileInfo().name,
					diff
				});
			}
		}
	}

	return move(result);
}

list<SimilarFiles> getSimilarForSpecifiedFile(
	const string& folderPath,
	const string& fileName,
	double maxDiff, // from 0.0 to 1.0
	const unique_ptr<IndexComparator>& indexComparator,
	bool disableExtensionOptimization
) {
	list<SimilarFiles> result;
	list<FileInfo> files = FileSystemUtilits::getFolderFiles(folderPath);

	// Check that file exists
	bool found = false;
	for ( auto& file: files ) {
		if ( file.name == fileName ) {
			found = true;
			break;
		}
	}

	if ( !found ) {
		throw FileNotFoundException();
	}

	cout << "Processing folder " << folderPath << endl;
	DBAdapter dbAdapter;
	dbAdapter.parseDirectory(folderPath);
	cout << "Folder processing finished" << endl;
	cout << "Comparing files" << endl;

	FileIndexArithmetic fileIndex = dbAdapter.getFileIndex(fileName);

	for ( auto& file: files ) {
		if ( file.name != fileName ) {
			FileIndexArithmetic index = dbAdapter.getFileIndex(file.name);

			// skip if we compare only files with the same extension
			if ( !disableExtensionOptimization
				&& !filesHasSimilarExtension(
						fileName,
						file.name
					)
			) {
				continue;
			}

			double diff = indexComparator->compare(fileIndex, index, maxDiff);
			if ( diff < maxDiff ) {
				result.push_back({
					file.name,
					fileName,
					diff
				});
			}
		}
	}

	return move(result);
}
