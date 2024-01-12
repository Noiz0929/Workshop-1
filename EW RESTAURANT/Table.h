#pragma once
#ifndef TABLE_H
#define TABLE_H

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

class Table
{
public://access specifier
	string tableID;

	Table();
	Table(sql::ResultSet* dataD);

	bool searchSeat();
	void addTable();
	void removeTable();
	static vector < Table > listTable ();

	~Table();

};

#endif
