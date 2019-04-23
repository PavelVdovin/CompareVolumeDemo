#include <iostream>
#include <stdlib.h>
#include <memory>
#include <algorithm>
#include "volumeCompare.h"
#include "LCSIndexComparator.h"

using namespace std;

void printUsage() {
	cout << "Demo program for comparing files in the specified folder." << endl;
	cout << "Usage: CompareVolumeDemo [-f filename] [-x number] [-d] path_to_folder" << endl;
	cout << "\tif -f key is NOT specified, all paired files are printed." << endl;
	cout << "\tif -f key is specified with the filename, then the file with the corresponding name is compared with other files in the folder" << endl;
	cout << "\t\t Specify file name using path relative to the directory, without any path prefixes (ex.: subfolder/file.txt)" << endl;
	cout << "\t-x key is used to specify the maximum difference between files (%), value should be within [0, 100] range (default 10)." << endl;
	cout << "\t-d key disables optimization of comparing files only with the similar extension." << endl;
}

void printResult(list<SimilarFiles>& similarFiles) {
	similarFiles.sort(
		[] (SimilarFiles& a, SimilarFiles& b) -> bool {
			return a.file1 < b.file1 || a.file1 == b.file1 && a.file2 < b.file2;
		}
	);

	cout.precision(2);
	for ( auto& simFiles: similarFiles ) {
		double diff = simFiles.diff;
		if ( diff < 0.01 ) {
			diff = 0; // to print double as number
		}
		cout << simFiles.file1 << "\t\t" << simFiles.file2 << "\t\t" << diff * 100 << "%" << endl;
	}
}

int main(int argc, char** argv) {
	// Parse command line
	if ( argc < 2 ) {
		printUsage();
		return 0;
	}

	string folderPath;

	bool compareWithFile = false;
	bool disableExtensionOptimization = false;
	string fileNameToCompareWith;
	int maxPercantage = 10;

	for ( int i = 1; i < argc; ++i ) {
		string param = argv[i];
		if ( param == "-d" ) {
			disableExtensionOptimization = true;
			continue;
		}

		if ( param == "-f" ) {
			compareWithFile = true;
			i++;
			if ( i == argc ) {
				printUsage();
				return 0;
			}

			fileNameToCompareWith = argv[i];
			continue;
		}

		if ( param == "-x" ) {
			i++;
			if ( i == argc ) {
				printUsage();
				return 0;
			}

			maxPercantage = atoi(argv[i]);
			if ( maxPercantage < 0 || maxPercantage > 100 ) {
				cout << "Specified -x value should be a number within range [0, 100]." << endl;
				printUsage();
				return 0;
			}
			continue;
		}

		if ( folderPath.size() ) {
			cout << "Folder is specified more then once" << endl;
			printUsage();
			return 0;
		}

		folderPath = argv[i];
	}

	if ( folderPath.size() == 0) {
		cout << "Folder is not specified" << endl;
		printUsage();
		return 0;
	}

	// Run an algorithm
	double maxDiff = (double)maxPercantage / 100;
	list<SimilarFiles> similarFiles;
	unique_ptr<IndexComparator> comparator = make_unique<LCSIndexComparator>();

	try {
		if ( compareWithFile ) {
			similarFiles = getSimilarForSpecifiedFile(folderPath, fileNameToCompareWith, maxDiff, comparator, disableExtensionOptimization);
		} else {
			similarFiles = getSimilarFilesInFolder(folderPath, maxDiff, comparator, disableExtensionOptimization);
		}

		printResult(similarFiles);
	} catch (exception& ex) {
		cout << ex.what() << endl;
		return -1;
	}

	return 0;
}
