/*
 * DBParser.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#include <cassert>
#include <iostream>
#include "fileSystemUtilits.h"
#include "DBParser.h"


void DBParser::parseDirectory(const string& path) {
	if ( m_connectionOpened ) {
		closeConnection();
	}

	list<FileInfo> files = FileSystemUtilits::getFolderFiles(path);

	// opening connection
	mdb_env_create(&m_env);
	mdb_env_open(m_env, path.c_str(), 0, 0664);
	mdb_txn_begin(m_env, NULL, 0, &m_txn);
	mdb_open(m_txn, NULL, 0, &m_dbi);

	m_connectionOpened = true;
	for ( auto& file: files ) {
		MDB_val key, data;
		key.mv_size = sizeof(file.name);
		key.mv_data = &file.name;
		int result = mdb_get(m_txn, m_dbi, &key, &data);
		if ( result != 0 ) {
			// key not found,
			// add file to db
			parseFile(file);
			continue;
		}

		FileIndexArithmetic* indexFromDB = static_cast<FileIndexArithmetic*>(data.mv_data);
		FileInfo dbInfo = indexFromDB->getFileInfo();
		if ( dbInfo.modificationTimestamp != file.modificationTimestamp ) {
			// file was modified, recreate it
			mdb_del(m_txn, m_dbi, &key, NULL);
			//mdb_txn_commit(m_txn);
			//mdb_txn_begin(m_env, NULL, 0, &m_txn);
			parseFile(file);
			continue;
		}

		m_fileIndexes[file.name] = indexFromDB;
	}

	for ( auto& item: m_fileIndexes) {
		cout << "DB: " << item.second->getFileInfo().name << endl;
	}

}

void DBParser::closeConnection() {
	if ( m_connectionOpened ) {
		m_fileIndexes.clear();
		mdb_txn_commit(m_txn);
		mdb_close(m_env, m_dbi);
		mdb_env_close(m_env);

		m_connectionOpened = false;
	}
}

void DBParser::parseFile(const FileInfo& file) {
	assert(m_connectionOpened);

	FileIndexArithmetic index(file);
	index.calculate();

	MDB_val key, data;
	key.mv_size = sizeof(index.getFileInfo().name);
	key.mv_data = (void*)&index.getFileInfo().name;
	data.mv_size = sizeof(index);
	data.mv_data = &index;

	cout << "Adding " << index.getFileInfo().name << " to db" << endl;

	mdb_put(m_txn, m_dbi, &key, &data, 0);
	//mdb_txn_commit(m_txn);
	//mdb_txn_begin(m_env, NULL, 0, &m_txn);

	int result = mdb_get(m_txn, m_dbi, &key, &data);
	assert(result == 0);

	FileIndexArithmetic* indexFromDB = static_cast<FileIndexArithmetic*>(data.mv_data);
	m_fileIndexes[file.name] = indexFromDB;
	cout << "Added " << indexFromDB->getFileInfo().name << " to db" << endl;
	cout << indexFromDB << " " << &index << endl;
}
