#include "Payment.h"
#include"DBConnection.h"

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;
using namespace sql;

int Payment::paymentCount = 0;

Payment::Payment()
{
	paymentID = "";
	method = "";
	tableID = "";
	orderID = "";
	staffID = "";
	totalPrice = 0;
	date = "";
}

Payment::Payment(ResultSet* data) {
	paymentID = data->getString("PAYMENTID");
	method = data->getString("PAYMENTMETHOD");
	orderID = data->getString("ORDERID");;
	staffID = data->getString("STAFFID");
	totalPrice = 0;
	date = data->getString("DATE");
	date = data->getString("TABLEID");
}

void Payment::countPayment()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(PAYMENTID) AS PAYMENTCOUNT FROM payment WHERE PAYMENTID LIKE 'P%'");
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			paymentCount = db.res->getInt("PAYMENTCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving payment count." << std::endl;
	}
}
void Payment::createPaymentID()
{
	countPayment();
	paymentCount++;
	stringstream pp;
	pp << "P" << setfill('0') << setw(2) << paymentCount;
	paymentID = pp.str();
}

void Payment::insert()
{
	DBConnection db;//instantiate
	db.prepareStatement("Insert into payment(PAYMENTID,PAYMENTMETHOD,ORDERID,STAFFID,TOTALPRICE,DATE,TABLEID) VALUES (?,?,?,?,?,?,?)");
	db.stmt->setString(1, paymentID);
	db.stmt->setString(2, method);
	db.stmt->setString(3, orderID);
	db.stmt->setString(4, staffID);
	db.stmt->setDouble(5, totalPrice);
	db.stmt->setString(6, date);
	db.stmt->setString(7, tableID);
	db.QueryStatement();
	db.~DBConnection();
}

void Payment::update()
{
	DBConnection db;
	db.prepareStatement("UPDATE payment SET PAYMENTMETHOD=? WHERE ORDERID=?");
	db.stmt->setString(1, method);
	db.stmt->setString(2, orderID);
	db.QueryStatement();
	db.~DBConnection();
}

void Payment::remove()
{
	DBConnection db;
	db.prepareStatement("DELETE FROM payment WHERE ORDERID=?");
	db.stmt->setString(1, orderID);
	db.QueryStatement();
	db.~DBConnection();
}

void Payment::getTable()
{
	string sql = " SELECT C.TABLEID AS TABLEID, O.DATE AS DATE "
		" FROM `order` O "
		" JOIN customer C ON C.CUSTOMERID = O.CUSTOMERID "
		" WHERE O.ORDERID = ? ";

	DBConnection db;
	db.prepareStatement(sql);
	db.stmt->setString(1, orderID);
	db.QueryResult();

	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			tableID = db.res->getString("TABLEID");
			date = db.res->getString("DATE");
		}
	}
	else {
		std::cout << "Error retrieving: " << endl;
	}
}

bool Payment::notPaid()
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

Payment::~Payment()
{

}