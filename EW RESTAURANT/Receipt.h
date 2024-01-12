#pragma once
#ifndef RECEIPT_H
#define RECEIPT_H

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

class Receipt
{

public:
	string name,foodID;
	int quantity;
	double total;

	Receipt(sql::ResultSet* data);

	//static Receipt searchFood(string foodID);
	static vector <Receipt> showReceipt(string orderID);
	~Receipt();
};


#endif