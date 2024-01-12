#include "Customer.h"
#include "DBConnection.h" 
#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

int Customer::customerCount = 0;

Customer::Customer()
{
	customerID = "";
	nickname = "";
	cusphoneNo = "";
	cusEmail = "";
	paxNo = 0;
	tableID = "";
	orderID = "";
}


Customer::Customer(sql::ResultSet* data)
{
	customerID = data->getString("CUSTOMERID");
	nickname = data->getString("NICKNAME");
	cusphoneNo = data->getString("PHONENO");
	cusEmail = data->getString("EMAIL");
	paxNo = data->getInt("PAXNO");
	tableID = data->getString("TABLEID");
}


void Customer::countCustomer()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(CUSTOMERID) AS CUSTOMERCOUNT FROM customer");
	db.QueryResult();

	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			customerCount = db.res->getInt("CUSTOMERCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving customer count." << std::endl;
	}
}



void Customer::createCustomerID()
{
	countCustomer();
	customerCount++;
	stringstream cc;
	cc << "C" << setfill('0') << setw(2) << customerCount;
	customerID = cc.str();
}

void Customer::addCustomer()
{
	DBConnection db;//instantiate
	db.prepareStatement("Insert into customer(CUSTOMERID,NICKNAME,PHONENO,EMAIL,PAXNO,TABLEID) VALUES (?,?,?,?,?,?)");
	db.stmt->setString(1, customerID);
	db.stmt->setString(2, nickname);
	db.stmt->setString(3, cusphoneNo);
	db.stmt->setString(4, cusEmail);
	db.stmt->setInt(5, paxNo);
	db.stmt->setString(6, tableID);
	db.QueryStatement();
	db.~DBConnection();
}


void Customer::update()
{
	DBConnection db;
	db.prepareStatement("UPDATE customer SET NICKNAME=?,PHONENO=?,EMAIL=?,PAXNO =?,TABLEID =? WHERE CUSTOMERID=?");
	db.stmt->setString(1, nickname);
	db.stmt->setString(2, cusphoneNo);
	db.stmt->setString(3, cusEmail);
	db.stmt->setInt(4, paxNo);
	db.stmt->setString(5, tableID);
	db.stmt->setString(6, customerID);
	db.QueryStatement();
	db.~DBConnection();
}


bool Customer::searchAcc()  //Detect whether the customerID valid in system
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM customer WHERE CUSTOMERID=?");
	db.stmt->setString(1, customerID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			tableID = db.res->getString("TABLEID");
			customerID = db.res->getString("CUSTOMERID");
			cusphoneNo = db.res->getString("PHONENO");
			cusEmail = db.res->getString("EMAIL");
			nickname = db.res->getString("NICKNAME");
			paxNo = db.res->getInt("PAXNO");
		}
		db.~DBConnection();
		return true;
	}
	else {

		db.~DBConnection();
		return false;
	}
}

bool Customer ::newCustomer() //Avoid log in of customer who leave
{
	string sql = " SELECT C.CUSTOMERID AS CUSTOMERID,O.ORDERID AS ORDERID,C.TABLEID AS TABLEID  "
		" FROM CUSTOMER C "
		" JOIN `ORDER` O ON O.CUSTOMERID = C.CUSTOMERID ";
        sql += " WHERE O.STATUS = 'UNPAID' AND C.CUSTOMERID =?";

	DBConnection db;
	db.prepareStatement(sql);
	db.stmt->setString(1, customerID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			customerID = db.res->getString("CUSTOMERID");
			tableID = db.res->getString("TABLEID");
		}
		db.~DBConnection();
		return true;
	}
	else {

		db.~DBConnection();
		return false;
	}
}

vector < Customer > Customer ::listCustomer() {

	string query = "SELECT * FROM customer ORDER BY CUSTOMERID ";
	DBConnection db;
	db.prepareStatement(query);
	vector<Customer> customer;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Customer tmpCustomer(db.res);
			customer.push_back(tmpCustomer);

		}
	}

	db.~DBConnection();
	return customer;
}



Customer::~Customer()
{

}
