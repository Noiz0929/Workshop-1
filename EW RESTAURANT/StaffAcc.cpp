#include "StaffAcc.h"
#include "DBConnection.h" 
#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

int StaffAcc::staffCount = 0;
string StaffAcc::tmp = "";

StaffAcc::StaffAcc()
{
	staffID = "";
	name = "";
	passwd = "";
	staffphoneNo = "";
	staffemail = "";
	address = "";
	position = "";
}


StaffAcc::StaffAcc(sql::ResultSet* data) {
	staffID = data->getString("StaffID");
	name = data->getString("NAME");
	passwd = data->getString("PASSWORD");
	staffphoneNo = data->getString("PHONENO");
	staffemail = data->getString("EMAIL");
	address = data->getString("ADDRESS");
	position = data->getString("POSITION");
}

void StaffAcc::countStaff()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(STAFFID) AS STAFFCOUNT FROM staff");
	db.QueryResult();

	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			staffCount = db.res->getInt("STAFFCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving staff count." << std::endl;
	}
}

void StaffAcc::createStaffID()
{
	countStaff();
	staffCount++;
	stringstream ss;
	ss << "S" << setfill('0') << setw(2) << staffCount;
	staffID = ss.str();
}

bool StaffAcc::login()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM staff WHERE STAFFID=? AND PASSWORD=?");
	db.stmt->setString(1, staffID);
	db.stmt->setString(2, passwd);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			staffID = db.res->getString("STAFFID");
			name = db.res->getString("NAME");
			passwd = db.res->getString("PASSWORD");
			staffphoneNo = db.res->getString("PHONENO");
			staffemail = db.res->getString("EMAIL");
			address = db.res->getString("ADDRESS");
			position = db.res->getString("POSITION");
		}
		db.~DBConnection();
		return true;
	}
	else {

		db.~DBConnection();
		return false;
	}
}

void StaffAcc::add()
{
	DBConnection db;//instantiate
	db.prepareStatement("Insert into staff(STAFFID,NAME,PASSWORD,PHONENO,EMAIL,ADDRESS,POSITION) VALUES (?,?,?,?,?,?,?)");
	db.stmt->setString(1, staffID);
	db.stmt->setString(2, name);
	db.stmt->setString(3, passwd);
	db.stmt->setString(4, staffphoneNo);
	db.stmt->setString(5, staffemail);
	db.stmt->setString(6, address);
	db.stmt->setString(7, position);
	db.QueryStatement();
	db.~DBConnection();
}

void StaffAcc::update()
{
	DBConnection db;
	db.prepareStatement("UPDATE staff SET NAME=?,PASSWORD=?,PHONENO=?,EMAIL=?,ADDRESS=?,POSITION=? WHERE STAFFID=?");
	db.stmt->setString(1, name);
	db.stmt->setString(2, passwd);
	db.stmt->setString(3, staffphoneNo);
	db.stmt->setString(4, staffemail);
	db.stmt->setString(5, address);
	db.stmt->setString(6, position);
	db.stmt->setString(7, staffID);
	db.QueryStatement();
	db.~DBConnection();
}

/*void StaffAcc::defineDeletion()
{
	DBConnection db;
	db.prepareStatement("SELECT COUNT(STAFFID) AS STAFFCOUNT FROM staff WHERE STAFFID <= ?");
	db.stmt->setString(1, staffID);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			staffCount = db.res->getInt("STAFFCOUNT");
		}
	}
	else {
		std::cout << "Error retrieving food count." << std::endl;
	}
}*/

void StaffAcc::defineLastRow()
{
	DBConnection db;
	db.prepareStatement("SELECT * FROM staff ORDER BY STAFFID DESC LIMIT 1");
	db.QueryResult();

	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			tmp = db.res->getString("STAFFID");
		}
	}
	else {
		std::cout << "Error retrieving staff count." << std::endl;
	}
}

void StaffAcc::updateLastRow()
{
	defineLastRow();
	DBConnection db;
	db.prepareStatement("UPDATE staff SET STAFFID =? WHERE STAFFID=?");
	db.stmt->setString(1, staffID);
	db.stmt->setString(2, tmp);
	db.QueryStatement();
	db.~DBConnection();
}



void StaffAcc::remove()
{
	DBConnection db;
	db.prepareStatement("DELETE FROM staff WHERE STAFFID=?");
	db.stmt->setString(1, staffID);
	db.QueryStatement();
	db.~DBConnection();
}

StaffAcc::~StaffAcc()
{

}

void StaffAcc::setPassword(string pass) {
	passwd = encrypt(pass);

}
string StaffAcc::getPassword() {
	return passwd;
}

// to check if a string matches with the encrypted password
bool  StaffAcc::MatcPasswordWith(string plainText) {
	return isMatch(passwd, plainText);
}


// simple shifting encryption where the character are shifted by its ASCII decimal code depending on its index
string  StaffAcc::encrypt(string input) {
	string ciphertext = "";
	for (int i = 0; i < input.length(); i++) {
		ciphertext += toChar(input[i] + ((i ^ 2 + 1) * input.length()));
	}
	return ciphertext;
}

// since we are using our own simple encryption, just re ecnrypt the target plain text and compare with the encrypted text
bool  StaffAcc::isMatch(string encrypted, string testText) {
	return (bool)(encrypt(testText) == encrypted);
}

char  StaffAcc::toChar(int asciDecimal) {
	// convert int to reeadbale char based on ASCII
	// characters in ASCII decimal are 32 (space) ~ 125 (~)
	while (asciDecimal < 33) {
		asciDecimal = asciDecimal + asciDecimal + 1;
	}
	while (asciDecimal > 125) {
		asciDecimal = (asciDecimal % 125) + 32;
	}
	return (char)asciDecimal;
}


