#pragma once
#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "Food.h"
#include<sstream>
#include<iomanip>
#include<chrono>

using namespace std;

class Order
{
private:
	static int orderCount;
	static string tmp;

public:
	string orderID, customerID, date, status;

	vector<pair<Food, int>> items;

	Order();
	Order(sql::ResultSet* data);

	void addFood(Food food, int quantity);
	void insert();
	void staffInsert(string foodID, int quantity);
	double total();
	int count();
	void countOrder();
	void createOrderID();
	void getDate();

	void update(string foodID,int quantity);
	void remove(string foodID);

	void cancelOrder();
	void defineLastRow();
	void updateLastRow();

	void newStatus();

	bool addOrder();

	bool notPaid();

	void searchOrder();

	bool foodList(string foodID);
	
};


#endif