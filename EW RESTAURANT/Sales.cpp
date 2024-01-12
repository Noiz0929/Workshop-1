#include "Sales.h"
#include"DBConnection.h"

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>
using namespace std;
using namespace sql;

Sales::Sales(ResultSet* data) {
	value = data->getDouble("VALUE");
	date = data->getString("DATE");
	typeName = data->getString("TYPE");
}


vector<Sales> Sales::salesReport(string start, string end, vector<string> typeName, bool sortByDate, bool ascending)
{


	string sql = " SELECT o.date as DATE, f.type as TYPE, SUM(f.price * oi.QUANTITY) as VALUE "
		" FROM `order` o"
		" JOIN orderlist oi ON o.ORDERID = oi.ORDERID "
		" JOIN food f ON f.FOODID = oi.FOODID ";

	sql += " WHERE  o.DATE >= ? AND o.DATE <= ?  AND o.STATUS = 'PAID'";

	sql += " GROUP BY f.TYPE, ";

	sql += " CAST(MONTH(o.DATE) AS VARCHAR(2)) + '-' + CAST(YEAR(o.DATE) AS VARCHAR(4)) ";

	sql += " ORDER BY  ";
	if (sortByDate) {

		sql += " DATE";
	}
	else {
		sql += " VALUE ";
	}

	if (ascending) {
		sql += " ASC ";
	}
	else {
		sql += " DESC ";
	}

	vector <Sales> salesReport;

	DBConnection db;

	db.prepareStatement(sql);

	int index = 1;

	while (index <= typeName.size()) {

		db.stmt->setString(index, typeName[index - 1]);
		index++;
	}

	db.stmt->setString(index, start);
	index++;
	db.stmt->setString(index, end);
	db.QueryResult();


	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Sales tmpSale(db.res);
			salesReport.push_back(tmpSale);

		}
	}

	db.~DBConnection();
	return salesReport;
}

Sales::~Sales()
{

}
