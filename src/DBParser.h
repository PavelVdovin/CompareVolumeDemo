/*
 * DBParser.h
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#ifndef SRC_DBPARSER_H_
#define SRC_DBPARSER_H_

#include <lmdb.h>
#include <string>
#include <map>

#include "fileIndexArithmetic.h"
using namespace std;

class DBParser {

	// file name: index
	//
	// File indexes are mapped to db memory and are available only
	// when db connection is open!
	map<string, FileIndexArithmetic*> m_fileIndexes;

	bool m_connectionOpened;

	MDB_env *m_env;
	MDB_txn *m_txn;
	MDB_dbi m_dbi;

public:
	DBParser(): m_connectionOpened(false) {}
	virtual ~DBParser() {
		closeConnection();
	}

	void parseDirectory(const string& path);

	const map<string, FileIndexArithmetic*>& getFileIndexes() const {
		return m_fileIndexes;
	}

	void closeConnection();

private:

	// This should parse a new file from directory and add it to database
	void parseFile(const FileInfo& file);

};

#endif /* SRC_DBPARSER_H_ */
