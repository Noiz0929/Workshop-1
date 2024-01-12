#pragma once
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

class Customer
{
private:
	static int customerCount;

public://access specifier
	int paxNo;
	string customerID, nickname, cusEmail, cusphoneNo, tableID, orderID;

	Customer();
	Customer(sql::ResultSet* dataD);

	void addCustomer();
	void createCustomerID();
	void update();
	void countCustomer();
	bool searchAcc();
	bool newCustomer();

	static vector < Customer > listCustomer();

	~Customer();

};


#endif