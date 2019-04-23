/*
 * DBAdapter.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: pavel
 */

#include <cassert>
#include <iostream>
#include <string.h>

#include "DBAdapter.h"
#include "fileSystemUtilits.h"

using namespace std;

#define MAX_DB_MAP_SIZE 100 * 1000000


void DBAdapter::parseDirectory(const string& path) {
	if ( m_connectionOpened ) {
		closeConnection();
	}

	m_path = path;

	list<FileInfo> files = FileSystemUtilits::getFolderFiles(path);

	// opening connection
	mdb_env_create(&m_env);
	mdb_env_set_mapsize(m_env, MAX_DB_MAP_SIZE);
	mdb_env_open(m_env, path.c_str(), 0, 0664);
	mdb_txn_begin(m_env, NULL, 0, &m_txn);
	mdb_open(m_txn, NULL, 0, &m_dbi);

	m_connectionOpened = true;
	for ( auto& file: files ) {
		MDB_val key, data;

		key.mv_size = strlen(file.name.c_str());
		key.mv_data = (void*)file.name.c_str();
		int result = mdb_get(m_txn, m_dbi, &key, &data);
		if ( result != 0 ) {
			// key not found,
			// add file to db
			parseFile(file);
			continue;
		}

		// data is actually a last modification date of the file
		long* lastModificationDate = static_cast<long*>(data.mv_data);
		// FileInfo dbInfo = indexFromDB->getFileInfo();
		if ( (*lastModificationDate) != file.modificationTimestamp ) {
			// file was modified, recreate it
			mdb_del(m_txn, m_dbi, &key, NULL);
			mdb_txn_commit(m_txn);
			mdb_txn_begin(m_env, NULL, 0, &m_txn);
			parseFile(file);
			continue;
		}
	}

	cout << files.size() << " files parsed" << endl;

}

void DBAdapter::closeConnection() {
	if ( m_connectionOpened ) {
		mdb_txn_commit(m_txn);
		mdb_close(m_env, m_dbi);
		mdb_env_close(m_env);

		m_connectionOpened = false;
	}
}

FileIndexArithmetic DBAdapter::getFileIndex(const string& fileName) const {

	MDB_val key, data;
	string keySize = fileName + "_s";
	key.mv_size = strlen(keySize.c_str());
	key.mv_data = (void*)keySize.c_str();
	int result = mdb_get(m_txn, m_dbi, &key, &data);
	if ( result != 0 ) {
		throw DBKeyMissingException(keySize);
	}

	size_t dataSize = *(static_cast<size_t*>(data.mv_data));

	key.mv_size = strlen(fileName.c_str());
	key.mv_data = (void*)fileName.c_str();
	result = mdb_get(m_txn, m_dbi, &key, &data);
	if ( result != 0 ) {
		throw DBKeyMissingException(fileName);
	}

	long modificationTimestamp = *static_cast<long*>(data.mv_data);

	FileInfo fileInfo;
	fileInfo.name = fileName;
	fileInfo.path= m_path + "/" + fileName;
	fileInfo.modificationTimestamp = modificationTimestamp;

	FileIndexArithmetic index(fileInfo);

	string keyData = fileName + "_d";
	key.mv_size = strlen(keyData.c_str());
	key.mv_data = (void*)keyData.c_str();
	result = mdb_get(m_txn, m_dbi, &key, &data);
	if ( result != 0 ) {
		throw DBKeyMissingException(keyData);
	}

	double * indexData = static_cast<double*>(data.mv_data);

	index.setIndexFromMemory(dataSize, indexData);
	return move(index);
}

void DBAdapter::parseFile(const FileInfo& file) {
	assert(m_connectionOpened);

	FileIndexArithmetic index(file);
	index.calculate();

	// Save modification date
	const FileInfo& fileInfo = index.getFileInfo();
	MDB_val key, data;
	key.mv_size = strlen(fileInfo.name.c_str());
	key.mv_data = (void*)fileInfo.name.c_str();
	data.mv_size = sizeof(long);
	data.mv_data = (void*)&fileInfo.modificationTimestamp;

	int result = mdb_put(m_txn, m_dbi, &key, &data, 0);
	if ( result != 0 ) {
		throw DBKeyWrightException(fileInfo.name, result);
	}

	// Save data size and data
	string keyDataSize = fileInfo.name + "_s";
	int size = index.getHashSize();
	key.mv_size = strlen(keyDataSize.c_str());
	key.mv_data = (void*)keyDataSize.c_str();
	data.mv_size = sizeof(int);
	data.mv_data = &size;
	result = mdb_put(m_txn, m_dbi, &key, &data, 0);

	if ( result != 0 ) {
		throw DBKeyWrightException(keyDataSize, result);
	}

	string keyData = fileInfo.name + "_d";
	key.mv_size = strlen(keyData.c_str());
	key.mv_data = (void*)keyData.c_str();
	data.mv_size = sizeof(double) * index.getHashSize();
	data.mv_data = index.getHash();

	result = mdb_put(m_txn, m_dbi, &key, &data, 0);

	if ( result != 0 ) {
		throw DBKeyWrightException(keyData, result);
	}


	mdb_txn_commit(m_txn);
	mdb_txn_begin(m_env, NULL, 0, &m_txn);
}
