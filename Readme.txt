This is the demo program for comparing various files from the specified directory.
Program parses the specified directory recursevly, builds file indexes, saves them in lmdb database
and searches for paired files using Longest-Common-Subsequence (LCS) length based heuristic.
All indexes are saved in db and only new/modified files are indexed each time.

Files are compared both for metadata (file names and modification dates) and for content.
File content index represents an array of arithmetic hashes calculated for each line of file.
LSC length of two files are compared using effective algorithm (based on dynamic-programing approach) and the difference is calculated based on this length.
The common difference between files is a weighted sum of metatadata difference and content difference (content is taken with higher ratio).

Dependencies:
- lmdb dev lib ('sudo apt-get install liblmdb-dev' for ubuntu)
- gcc with c++17, cmake (>= 3.5), make.

To build the demo program, use cmake and make utilits:
> mkdir _buildDemo
> cd _buildDemo
> cmake ..
> make

file CompareVolumeDemo is then available in _buildDemo folder.

Usage:
CompareVolumeDemo [-f filename] [-x number] [-d] path_to_folder
	if -f key is NOT specified, all paired files are printed.
	if -f key is specified with the filename, then the file with the corresponding name is compared with other files in the folder
		 Specify file name using path relative to the directory, without any path prefixes (ex.: subfolder/file.txt)
 	-x key is used to specify the maximum difference between files (%), value should be within [0, 100] range (default 10).
	-d key disables optimization of comparing files only with the similar extension.

Examples:

Generate indexes and find all paired files in the current folder (this may take a long time):
> ./CompareVolumeDemo .

Generate indexes and find all paried files for file 'assets/test.txt' in the folder 'folder':
> ./CompareVolumeDemo -f assets/test.txt ./folder

Generate indexes and output differences with all files with '.txt' extension for file assets/test.txt:
> ./CompareVolumeDemo -f assets/test.txt -x 100 .

The program is tested with folders with up to 10000 files.
