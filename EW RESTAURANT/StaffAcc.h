#pragma once
#ifndef STAFFACC_H
#define STAFFACC_H

#include<string>
#include<iomanip>
#include<sstream>
#include<vector>
#include<iostream>
#include <mysql/jdbc.h>

using namespace std;

class StaffAcc
{

public://access specifier
	std::string staffID, name,staffphoneNo, staffemail, address, position; 

	StaffAcc();
	StaffAcc(sql::ResultSet* data);
	void createStaffID();
	bool login();
	void add();
	void update();
	void remove();
	void countStaff();
	//void defineDeletion();
	void defineLastRow();
	void updateLastRow();

	~StaffAcc();

	void setPassword(string pass);
	std::string getPassword();

	// to check if a string matches with the encrypted password
	bool MatcPasswordWith(string plainText);

private:
	static int staffCount;
	static string tmp;
	string passwd; //encapsulate password inside private because we want to ensure it is encrypted when set
	string encrypt(string input);
	char toChar(int asciDecimal);
	bool isMatch(string encrypted, string testText);


};

#endif
