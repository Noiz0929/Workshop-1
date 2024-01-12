#pragma once
#ifndef SALE_H
#define SALE_h

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

class Sales
{
public:
	string date, typeName;
	double value;

	Sales(sql::ResultSet* data);
	//since sale data is computed from database, it does not need normal constructor
	//it will not exist without result set 


	static std::vector<Sales> salesReport(string start, string end, vector<string> typeName, bool sortByDate, bool ascending);


	~Sales();
};


#endif