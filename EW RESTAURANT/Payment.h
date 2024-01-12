#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

class Payment
{
private:
	static int paymentCount;

public://access specifier
	double totalPrice;
	string paymentID, method, orderID, staffID, date, tableID;

	Payment();
	Payment(sql::ResultSet* data);

	void insert();
	void countPayment();
	void createPaymentID();
	void update();
	void remove();
	void getTable();
	bool notPaid();

	~Payment();
};


#endif