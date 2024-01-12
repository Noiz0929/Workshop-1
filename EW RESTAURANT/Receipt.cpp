#include "Receipt.h"
#include"Order.h"
#include"DBConnection.h"

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;
using namespace sql;



Receipt::Receipt(ResultSet* data) {
	foodID = data->getString("FOODID");
	name = data->getString("NAME");
	quantity = data->getInt("QTY");
	total = data->getDouble("AMT");
}

vector <Receipt> Receipt::showReceipt(string orderID) {
	string sql = " SELECT F.FOODID AS FOODID,F.FOODNAME AS NAME,OI.QUANTITY AS QTY,SUM(F.PRICE*OI.QUANTITY) AS AMT "
		" FROM ORDERLIST OI"
		" JOIN FOOD F ON OI.FOODID= F.FOODID "
		"JOIN `ORDER` O ON O.ORDERID = OI.ORDERID";

	sql += " WHERE O.ORDERID = ? ";

	sql += " GROUP BY FOODNAME";

	vector<Receipt> receipt;
	DBConnection db;
	db.prepareStatement(sql);
	db.stmt->setString(1, orderID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Receipt tmpReceipt(db.res);
			receipt.push_back(tmpReceipt);

		}
	}

	db.~DBConnection();
	return receipt;
}

/*Receipt Receipt::searchFood(string foodID)
{
	DBConnection db;
	db.prepareStatement("SELECT FOODID FROM ORDERLIST WHERE FOODID=?");
	db.stmt->setString(1, foodID);
	db.QueryResult();

	Receipt result;
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			Receipt found(db.res);
			result = found;
		}
	}
	db.~DBConnection();
	return result;
}*/



Receipt::~Receipt() {

}