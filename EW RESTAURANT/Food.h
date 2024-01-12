#pragma once
#ifndef FOOD_H
#define FOOD_H

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

class Food {
private:
	static int foodCount;
	static string tmp;

public://access specifier
	double price;
	string foodID, type, name, description;

	Food();
	Food(sql::ResultSet* data);

	void addFood();
	void countNoodle();
	void countRice();
	void countBeverage();
	void createNoodleID();
	void createRiceID();
	void createBeverageID();

	void updateFood();

	void foodRemove();
	void defineLastRow();
	void updateLastRow();

	bool validFood();

	//void getFood();
	static Food searchFood(string foodID);
	static vector <Food> searchFood(string type,string sortColumn, bool ascending);
	~Food();

};

#endif
