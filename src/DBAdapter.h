/*
 * DBAdapter.h
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#ifndef SRC_DBADAPTER_H_
#define SRC_DBADAPTER_H_

#include <lmdb.h>
#include <string>
#include <string.h>
#include <map>

#include "fileIndexArithmetic.h"
using namespace std;

class DBKeyMissingException: public exception {

	string m_errText;

	virtual const char* what() const throw()
	{
		return m_errText.c_str();
	}

public:

	DBKeyMissingException(string keyName)
	: exception()
	{
		m_errText = "The specified key '" + keyName + "' does not exist in database";
	}

};

class DBKeyWrightException: public exception {

	string m_errText;

	virtual const char* what() const throw()
	{
		return m_errText.c_str();
	}

public:

	DBKeyWrightException(string keyName, int code)
	: exception()
	{
		if ( code == MDB_MAP_FULL  ) {
			m_errText = "The database is full, max map size is reached";
		} else if ( code == MDB_MAP_FULL ) {
			m_errText = "Transaction has too many dirty entries, skipping";
		} else {
			m_errText = string("Key ") + keyName + string(" write failed with code: ") + to_string(code);
		}
	}

};

class DBAdapter {

	bool m_connectionOpened;
	string m_path;

	MDB_env *m_env;
	MDB_txn *m_txn;
	MDB_dbi m_dbi;

public:
	DBAdapter(): m_connectionOpened(false) {}
	virtual ~DBAdapter() {
		closeConnection();
	}

	// This function checks the directory
	// for new and modified files and updates the database
	// in a corresponding way.
	void parseDirectory(const string& path);

	// This function allows to get file index from database memory
	// directly.
	// Be careful! Data from the memory may be changed after db update or db transaction commit!
	//
	// Assumption: connection should be opened!
	//
	FileIndexArithmetic getFileIndex(const string& fileName) const;

	void closeConnection();

private:

	// This should parse a new/modified file from directory and add it to database
	void parseFile(const FileInfo& file);

};

#endif /* SRC_DBADAPTER_H_ */
