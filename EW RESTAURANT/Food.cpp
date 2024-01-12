#include "Food.h"
#include "DBConnection.h" 
#include<ctime>
#include<vector>
#include<string>

using namespace std;

int Food::foodCount = 0;
string Food::tmp = "";

Food::Food()
{
	foodID = "";
	type = "";
	name = "";
	description = "";
	price = 0;
}



/*Food::Food(string foodID, string type, string name, string description, double price)
{
	this->foodID = foodID;
	this->type = type;
	this->name = name;
	this->description = description;
	this->price = price;
}*/

Food::Food(sql::ResultSet* data)
{
	foodID = data->getString("FOODID");
	type = data->getString("TYPE");
	name = data->getString("FOODNAME");
	description = data->getString("DESCRIPTION");
	price = data->getDouble("PRICE");
}


void Food::countNoodle()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(FOODID) AS FOODCOUNT FROM food WHERE TYPE LIKE 'N%'");
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			foodCount = db.res->getInt("FOODCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving food count." << std::endl;
	}
}
void Food::createNoodleID()
{
	countNoodle();
	foodCount++;
	stringstream ff;
	ff << "N" << setfill('0') << setw(2) << foodCount;
	foodID = ff.str();
}

void Food::countRice()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(FOODID) AS FOODCOUNT FROM food WHERE TYPE LIKE 'R%'");
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			foodCount = db.res->getInt("FOODCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving food count." << std::endl;
	}
}
void Food::createRiceID()
{
	countRice();
	foodCount++;
	stringstream ff;
	ff << "R" << setfill('0') << setw(2) << foodCount;
	foodID = ff.str();
}

void Food::countBeverage()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(FOODID) AS FOODCOUNT FROM food WHERE TYPE LIKE 'B%'");
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			foodCount = db.res->getInt("FOODCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving food count." << std::endl;
	}
}
void Food::createBeverageID()
{
	countBeverage();
	foodCount++;
	stringstream ff;
	ff << "B" << setfill('0') << setw(2) << foodCount;
	foodID = ff.str();
}

void Food::addFood()
{
	DBConnection db;//instantiate
	db.prepareStatement("Insert into food(FOODID,TYPE,FOODNAME,DESCRIPTION,PRICE) VALUES (?,?,?,?,?)");
	db.stmt->setString(1, foodID);
	db.stmt->setString(2, type);
	db.stmt->setString(3, name);
	db.stmt->setString(4, description);
	db.stmt->setDouble(5, price);
	db.QueryStatement();
	db.~DBConnection();
}

void Food::updateFood()
{
	DBConnection db;
	db.prepareStatement("UPDATE food SET FOODNAME=?,DESCRIPTION=?,PRICE=? WHERE FOODID=?");
	db.stmt->setString(1, name);
	db.stmt->setString(2, description);
	db.stmt->setDouble(3, price);
	db.stmt->setString(4, foodID);
	db.QueryStatement();
	db.~DBConnection();
}

void Food::foodRemove()
{
	DBConnection db;
	db.prepareStatement("DELETE FROM food WHERE foodID=?");
	db.stmt->setString(1, foodID);
	db.QueryStatement();
	db.~DBConnection();
}

void Food::defineLastRow()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM food WHERE TYPE = ? ORDER BY FOODID DESC LIMIT 1");
	db.stmt->setString(1, type);
	db.QueryResult();

	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			tmp = db.res->getString("FOODID");
		}
	}
	else {
		std::cout << "Error retrieving food count." << std::endl;
	}
}

void Food::updateLastRow()
{
	defineLastRow();
	DBConnection db;
	db.prepareStatement("UPDATE food SET FOODID =? WHERE FOODID=?");
	db.stmt->setString(1, foodID);
	db.stmt->setString(2, tmp);
	db.QueryStatement();
	db.~DBConnection();
}


/*void Food::getFood()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM food WHERE TYPE=?");
	db.stmt->setString(1, type);
	db.QueryResult();
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			foodID = db.res->getString("FOODID");
			type = db.res->getString("FOODTYPE");
			name = db.res->getString("FOODNAME");
			description = db.res->getString("DESCRIPTION");
			price = db.res->getDouble("PRICE");
		}
	}
	db.~DBConnection();
}*/

vector <Food> Food::searchFood(string type, string sortColumn, bool ascending) {

	string query = "SELECT * FROM food WHERE TYPE =? "" ORDER BY " + sortColumn;
	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}
	// 
	DBConnection db;
	db.prepareStatement(query);
	db.stmt->setString(1, type);
	vector<Food> foods;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Food tmpFood(db.res);
			foods.push_back(tmpFood);

		}
	}

	db.~DBConnection();
	return foods;
}


Food Food::searchFood(string foodID)
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM food WHERE FOODID=?");
	db.stmt->setString(1, foodID);
	db.QueryResult();

	Food result;
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			Food found(db.res);
			result = found;
		}
	}
	db.~DBConnection();
	return result;
}

bool Food::validFood()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM food WHERE FOODID=? ");
	db.stmt->setString(1, foodID);
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

Food::~Food()
{

}
