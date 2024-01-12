#include "Order.h"
#include"Food.h"
#include "DBConnection.h" 

#include<chrono>
#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>
using namespace std;

int Order::orderCount = 0;
string Order::tmp = "";

Order::Order()
{
	orderID = "";
	date = "";
	customerID = "";
	status = "";

}

Order::Order(sql::ResultSet* data)
{
	orderID = data->getString("ORDERID");
	date = data->getString("DATE");
	customerID = data->getString("CUSTOMER");
	status = data->getString("STATUS");
}

void Order::countOrder()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(ORDERID) AS ORDERCOUNT FROM `order` WHERE ORDERID LIKE 'O%'");
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			orderCount = db.res->getInt("ORDERCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving order count." << std::endl;
	}
}
void Order::createOrderID()
{
	countOrder();
	orderCount++;
	stringstream oo;
	oo << "O" << setfill('0') << setw(2) << orderCount;
	orderID = oo.str();
}

void Order::insert()
{
	DBConnection db;

	if(!addOrder()){
		db.prepareStatement("INSERT INTO `order`(ORDERID,DATE,CUSTOMERID,STATUS) VALUES (?,?,?,?)");
		db.stmt->setString(1, orderID);
		db.stmt->setString(2, date);
		db.stmt->setString(3, customerID);
		db.stmt->setString(4, status);
		db.QueryStatement();
	}

	string query = "INSERT INTO orderlist(ORDERID,FOODID,QUANTITY) VALUES ";
	for (int i = 0; i < items.size(); i++) {
		query += "(?,?,?),";
	}

	query.erase(query.size() - 1);
	db.prepareStatement(query);

	for (int i = 0; i < items.size(); i++) {

		db.stmt->setString(i * 3 + 1, orderID);
		db.stmt->setString(i * 3 + 2, items[i].first.foodID);
		db.stmt->setInt(i * 3 + 3, items[i].second);
	}
	db.QueryStatement();
}

void Order::staffInsert(string foodID, int quantity)
{
	DBConnection db;
	db.prepareStatement("INSERT INTO orderlist (ORDERID,FOODID,QUANTITY) VALUES (?,?,?)");
	db.stmt->setString(1, orderID);
	db.stmt->setString(2, foodID);
	db.stmt->setInt(3, quantity);
	db.QueryStatement();
}

bool Order::addOrder() //Check whether the customer want to add on
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM `order` WHERE CUSTOMERID=?");
	db.stmt->setString(1, customerID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			orderID = db.res->getString("ORDERID");
		}
		db.~DBConnection();
		return true;
	}
	else {

		db.~DBConnection();
		return false;
	}
}

void Order::addFood(Food food, int quantity)
{
	for (auto& item : items) {
		if (item.first.foodID == food.foodID) {
			// If found, update the quantity
			item.second += quantity;
			return;
		}
	}
	// If not found, add the food with the given quantity
	items.push_back({ food, quantity });
}

double Order::total()
{
	double total = 0;
	for (int i = 0; i < items.size(); i++)
	{
		total += items[i].first.price * items[i].second;
	}
	return total;
}

int Order::count() {
	int count = 0;
	for (int i = 0; i < items.size(); i++) {
		count += items[i].second;
	}
	return count;
}



void Order::getDate() {
	auto now = chrono::system_clock::now();

	time_t currentTime = chrono::system_clock::to_time_t(now);

	tm localTimeInfo;

	if (localtime_s(&localTimeInfo, &currentTime) != 0) {
		cerr << "Failed to get local time" << endl;
		return;
	}

	int currentYear = localTimeInfo.tm_year + 1900;
	int currentMonth = localTimeInfo.tm_mon + 1;
	int currentDay = localTimeInfo.tm_mday;

	stringstream dd;
	dd << currentYear << "-" << currentMonth << "-" << currentDay;
	date = dd.str();
}



void Order::newStatus()
{
	string sql = " UPDATE `order` "
		" SET STATUS = 'PAID' ";
    sql += " WHERE ORDERID = ? ";

	DBConnection db;
	db.prepareStatement(sql);
	db.stmt->setString(1, orderID);
	db.QueryStatement();
	db.~DBConnection();
}

bool Order::notPaid()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM `order` WHERE ORDERID=? AND STATUS='UNPAID'");
	db.stmt->setString(1, orderID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			orderID = db.res->getString("ORDERID");
		}
		db.~DBConnection();
		return true;
	}
	else {
		db.~DBConnection();
		return false;
	}
}


void Order::update(string foodID, int quantity)
{
	DBConnection db;
	db.prepareStatement("UPDATE ORDERLIST SET QUANTITY = ? WHERE FOODID =?");
		db.stmt->setInt(1, quantity);
		db.stmt->setString(2, foodID);
		db.QueryStatement();
		db.~DBConnection();

}


void Order::remove(string foodID)
{
	DBConnection db;
	db.prepareStatement("DELETE FROM orderlist WHERE foodID=?");
	db.stmt->setString(1, foodID);
	db.QueryStatement();
	db.~DBConnection();
}

bool Order::foodList(string foodID)
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM orderlist WHERE ORDERID=? AND FOODID=?");
	db.stmt->setString(1, orderID);
	db.stmt->setString(2, foodID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			foodID = db.res->getString("FOODID");
		}
		db.~DBConnection();
		return true;
	}
	else {
		db.~DBConnection();
		return false;
	}
}

void Order::cancelOrder()
{
	DBConnection db;
	db.prepareStatement("DELETE FROM `order` WHERE ORDERID=?");
	db.stmt->setString(1, orderID);
	db.QueryStatement();
	db.~DBConnection();
}

void Order::defineLastRow()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM ``order ORDER BY ORDERID DESC LIMIT 1");
	db.QueryResult();

	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			tmp = db.res->getString("ORDERID");
		}
	}
	else {
		std::cout << "Error retrieving staff count." << std::endl;
	}
}

void Order::updateLastRow()
{
	defineLastRow();
	DBConnection db;
	db.prepareStatement("UPDATE `order` SET ORDERID =? WHERE ORDERID=?");
	db.stmt->setString(1, orderID);
	db.stmt->setString(2, tmp);
	db.QueryStatement();
	db.~DBConnection();
}

void Order::searchOrder()
{
	string sql = " SELECT C.TABLEID AS TABLEID, O.ORDERID AS ORDERID "
		" FROM CUSTOMER C "
		" JOIN `ORDER` O ON O.CUSTOMERID = C.CUSTOMERID ";
	sql += " WHERE C.CUSTOMERID =? ";
	DBConnection db;
	db.prepareStatement(sql);
	db.stmt->setString(1, customerID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			orderID = db.res->getString("ORDERID");
		}
	}
	else {
		std::cout << "Error retrieving customer count." << std::endl;
	}
}

