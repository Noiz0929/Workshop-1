#include "Table.h"
#include "DBConnection.h"
#include "Customer.h"
#include<ctime>

using namespace std;

Table::Table()
{
	tableID = "";
}


Table ::Table(sql::ResultSet* data)
{
	tableID = data->getString("TABLEID");
}

bool Table::searchSeat()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM `table` WHERE TABLEID=?");
	db.stmt->setString(1, tableID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
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

void Table::addTable()
{
	DBConnection db;//instantiate
	db.prepareStatement("Insert into `table`(TABLEID) VALUES (?)");
	db.stmt->setString(1, tableID);
	db.QueryStatement();
	db.~DBConnection();
}


void Table::removeTable()
{
	DBConnection db;
	db.prepareStatement("DELETE FROM `table` WHERE TABLEID=?");
	db.stmt->setString(1, tableID);
	db.QueryStatement();
	db.~DBConnection();
}

vector < Table > Table ::listTable() {

	string query = "SELECT * FROM `table` "" ORDER BY TABLEID ";
	DBConnection db;
	db.prepareStatement(query);
	vector<Table> table;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Table tmpTable(db.res);
			table .push_back(tmpTable);

		}
	}

	db.~DBConnection();
	return table;
}

Table ::~Table()
{

}
