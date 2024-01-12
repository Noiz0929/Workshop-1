#include<iostream>
#include<conio.h>
#include<iomanip>
#include<sstream>
#include<string>
#include <chrono>
#include<fstream>
#include<regex>

#include "Page.h"
#include "StaffAcc.h" 
#include "Table.h"
#include "Customer.h"
#include "Food.h"
#include "Order.h"
#include "Sales.h"
#include "Receipt.h"
#include "Payment.h"


using namespace std;

string hiddenInput(string = "");
string hideString(string);
string hideString(int);
bool validDate(const string& date);

//Staff
void StaffMode();
void StaffRegister();
void StaffLogIn();


void manageTable(StaffAcc staff);
void deleteTable(StaffAcc staff);
void addTable(StaffAcc staff);

void StaffMenu(StaffAcc staff);

StaffAcc StaffInfo(StaffAcc staff);

//Order
void OrderMenu(StaffAcc staff);
void orderManage(StaffAcc staff,Order order);
void orderInfo(StaffAcc staff, string foodID,Order order);
void staffOrder(StaffAcc staff, Order order);

//Payment
void PaymentMenu(StaffAcc staff);
void viewReceipt(StaffAcc staff, Payment payment);

void CustomerInfo(StaffAcc staff);
void UpdateCustomer(StaffAcc staff, Customer customer);

void SaleReportMenu(StaffAcc staff);

//Menu
void menuOption(StaffAcc staff);
void foodMenu(StaffAcc staff);
void addNoodle(StaffAcc staff);
void addRice(StaffAcc staff);
void addBeverage(StaffAcc staff);

void foodUpdate(StaffAcc staff);
void displayFood(StaffAcc staff, string type);
void foodInfo(StaffAcc staff, string foodID,string type);

//Customer
void CustomerMode();
void CustomerLogIn();
void ChooseTable(Customer customer);
void insertPax(Customer customer,Table table);
void CustomerRegister();


void shop(Customer);
void displayReceipt(Customer customer, Order order);
Order CustomerMenu(Customer customer, string type, Order order);

//Cart
Order foodDetail(Customer customer, string foodID, Order order);
Order cartMenu(Customer customer, Order order);

void Exit();

int main()
{
	Page homepage;
	stringstream ss;
	ss << setw(30) << "" << " Welcome to Eternal Weaving Restaurant " << "" << setw(30);
	homepage.header = ss.str();
	homepage.addOption("Staff Mode");
	homepage.addOption("Customer Mode");

	while (1)
	{
		switch (homepage.prompt())
		{
		case -1:
			return 0;
			break;
		case 0:
			StaffMode();
			break;
		case 1:
			CustomerMode();
			break;
		}
	}
}

void StaffMode()
{
	Page staffMenu;
	stringstream ss;
	ss << setw(45) << "" << " Staff Mode " << "" << setw(44);
	staffMenu.header = ss.str();
	staffMenu.addOption("Register");
	staffMenu.addOption("Log In");
	int option = 0;
	while (1)
	{
		option = staffMenu.prompt(option);
		switch (option)
		{
		case -1:
			return;
			break;
		case 0:
			StaffRegister();
			break;
		case 1:
			StaffLogIn();
			break;
		}
	}
}

void CustomerMode()
{
	Page seatMenu;
	stringstream ss;
	ss << setw(30) << "" << " Welcome to Eternal Weaving Restaurant " << "" << setw(30);
	seatMenu.header = ss.str();
	seatMenu.addOption("Registration");
	seatMenu.addOption("Log In");

	switch (seatMenu.prompt())
	{
	case -1:
		return;
		break;
	case 0:
		CustomerRegister();
		break;
	case 1:
		CustomerLogIn();
		break;
	}
}

void CustomerRegister()
{
	string date;
	Customer newAcc;
	Page customerRegister;
	stringstream ss;
	ss << setw(30) << "" << " Welcome to Eternal Weaving Restaurant " << "" << setw(30);
	customerRegister.header = ss.str();
	customerRegister.addOption("Nickname");
	customerRegister.addOption("Phone No");
	customerRegister.addOption("Email");
	customerRegister.addOption("Confirm");

	string tmpinput;

	while (1)
	{
		switch (customerRegister.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Nickname: ";
			getline(cin, newAcc.nickname);
			customerRegister.setValue(0, newAcc.nickname);
			break;
		case 1:
			cout << "Phone No: ";
			cin >> newAcc.cusphoneNo;
			customerRegister.setValue(1, newAcc.cusphoneNo);
			break;
		case 2:
			cout << "Email: ";
			cin >> newAcc.cusEmail;
			customerRegister.setValue(2, newAcc.cusEmail);
			break;
		case 3:
			if (newAcc.nickname.empty() || newAcc.cusphoneNo.empty() || newAcc.cusEmail.empty())
			{
				cout << "Please complete you information";
				_getch();
			}
			else
			{
				newAcc.createCustomerID();
				cout << "Your customer ID is "<< newAcc.customerID;
				_getch();
				ChooseTable(newAcc);
			}
			break;
		}
	}

}

void ChooseTable(Customer customer)
{
	vector<Table>table;
	Table newTable;
	string tableID;
	Page TL;//table list
	TL.addOption("Table ID:");
	table = Table::listTable ();
	while (1)
	{
		stringstream ss;
		ss << setw(38) << "" << " Please select a table " << "" << setw(38)<<endl;
		ss << "---------------------------------------------------------------------------------------------------------";
		ss << endl<< "Table ID\n---------------------------------------------------------------------------------------------------------" << endl;
		for (int i = 0; i < table.size(); i++)
		{
			ss << left << setw(8) << table[i].tableID << endl;;
		}
		TL.header = ss.str();

		switch (TL.prompt())
		{
		case 0:
			cout << "Table ID: ";
			cin >> tableID;
			newTable.tableID = tableID;
			if (newTable.searchSeat())
			{
				customer.tableID = newTable.tableID;
				insertPax(customer, newTable);
				_getch();
			}
			else
			{
				cout << "Please insert a valid Table no";
				_getch();
			}
			break;
		}
	}
}

void insertPax(Customer customer,Table table)
{
	Order order;
	Page assignSeat;
	stringstream ss;
	ss << setw(38) << "" << " Please insert pax number " << "" << setw(37);
	assignSeat.header = ss.str();
	assignSeat.addOption("Pax no");
	assignSeat.addOption("Confirm");
	bool valid = true;
	while (1)
	{
		switch (assignSeat.prompt())
		{
		case 0:
			cout << "Pax No: ";
			cin >> customer.paxNo;
			assignSeat.setValue(0, to_string(customer.paxNo));
			break;
		case 1:
			if (customer.paxNo == 0)
			{
				cout << "Please check your information";
				_getch();
			}
			else
			{
				customer.addCustomer();
				_getch();
				shop(customer);
			}
			break;
		}
	}
}

void CustomerLogIn()
{
	Page CLogin; //customer log in
	Customer customer;
	stringstream ss;
	ss << setw(47) << "" << " Login " << "" << setw(47);
	CLogin.header = ss.str();
	CLogin.addOption("Customer ID");
	CLogin.addOption("Enter");

	while (1)
	{
		switch (CLogin.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Customer ID: ";
			cin >> customer.customerID;
			CLogin.setValue(0, customer.customerID);
			break;
		case 1:
			if (customer.newCustomer())
			{
				shop(customer);
			}
			else {
				cout << "Please insert a valid Customer ID";
				_getch();
			}
			break;
		}
	}
}

void shop(Customer customer)
{
	Order order;
	string type;
	Page shopMenu;
	stringstream headerStream;
	order.customerID = customer.customerID;
	order.searchOrder();
	customer.orderID = order.orderID;
	shopMenu.addOption("Noodle");
	shopMenu.addOption("Rice");
	shopMenu.addOption("Beverage");
	shopMenu.addOption("Cart");
	shopMenu.addOption("Receipt");
	shopMenu.addOption("Log out");
	while (1)
	{
		headerStream.str("");
		headerStream << setw(48)  << "" << " SHOP " << "" << setw(47)<<endl;
		headerStream << "---------------------------------------------------------------------------------------------------------\n";
		headerStream << "Items in cart : " << order.count() << "  \nTotal Price   : " << fixed << setprecision(2) << order.total();
		shopMenu.header = headerStream.str();
		switch (shopMenu.prompt())
		{
		case 0:
			type = "Noodle";
			order = CustomerMenu(customer, "Noodle", order);
			break;
		case 1:
			type = "Rice";
			order = CustomerMenu(customer, "Rice", order);
			break;
		case 2:
			type = "Beverage";
			order = CustomerMenu(customer, "Beverage", order);
			break;
		case 3:
			order = cartMenu(customer, order);
			break;
		case 4:
			displayReceipt(customer, order);
			break;
		case 5:
			main();
			break;
		}
	}
}

Order CustomerMenu(Customer customer, string type, Order order)
{
	vector<Food> food;
	string displayString = "", sortColumn = "FOODID";
	bool ascending = true;
	string foodID;
	Page foodList;
	foodList.addOption("Select");
	food = Food::searchFood(type, sortColumn, ascending);
	while (1)
	{
			stringstream ss;
			ss << setw(48) << "" << " Menu " << "" << setw(47) << endl;
			if (type == "Noodle")
			{
				ss << setw(47) << "" << " " << type << " "<< "" << setw(46) << endl;
			}
			else if (type == "Beverage")
			{
				ss << setw(46) << "" << " " << type << " " << "" << setw(45) << endl;
			}
			else if (type == "Rice")
			{
				ss << setw(48) << "" << " " << type << " " << "" << setw(47) << endl;
			}
			ss << "---------------------------------------------------------------------------------------------------------\n";
			ss << fixed << setprecision(2) << setw(15) << "FOOD ID" << "|" << setw(65) << "NAME"
				<< "|" << setw(20) << "PRICE" << "|" << endl;

			for (int i = 0; i < food.size(); i++)
			{
				ss << setw(15) << food[i].foodID << "|" << setw(65) << food[i].name
					<< "|" <<fixed <<setprecision(2)<< setw(20) << food[i].price << "|" << endl;
			}
			foodList.header = ss.str();
		

		switch (foodList.prompt())
		{
		case -1:
			return order;
			break;
		case 0:
			cout << "Insert Food ID to select: ";
			cin >> foodID;
			order = foodDetail(customer, foodID, order);
			break;
		}
	}
}

Order foodDetail(Customer customer, string foodID, Order order)
{
	Food food = Food::searchFood(foodID);
	if (food.foodID.empty())
	{
		cout << "Food not found";
		_getch();
		return order;
	}

	Page foodMenu;
	stringstream ss;
	ss << setw(44) << "" << " Food Detail " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss<< "Action"<< endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Name        : " << food.name << endl;
	ss << "Description : " << food.description << endl;
	ss << "Price       : " << fixed << setprecision(2) << food.price;
	foodMenu.header = ss.str();

	foodMenu.addOption("Add to cart");
	while (1)
	{
		switch (foodMenu.prompt())
		{
		case -1:
			return order;
			break;
		case 0:
			cout << "Insert quantity: ";
			int qty;
			cin >> qty;
			if (qty > 0)
			{
				order.addFood(food, qty);
			}
			cout << endl << "Food Added Into Cart";
			_getch();
			break;
	
		}
	}
}

Order cartMenu(Customer customer, Order order)
{
	Page cartMenu;
	cartMenu.addOption("Checkout");
	cartMenu.addOption("Empty Cart");
	stringstream ss;

	ss << setw(48) << "" << " Cart " << "" << setw(47) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << fixed << setprecision(2) << setw(40) << "Food" << "|" << setw(20) << "Price" << "|"
		<< setw(20) << "Quantity" << "|" << setw(20) << "Subtotal" << "|" << endl;
	for (int i = 0; i < order.items.size(); i++) {
		ss << setw(40) << order.items[i].first.name << "|" << setw(20) << fixed << setprecision(2) << order.items[i].first.price << "|"
			<< setw(20) << order.items[i].second << "|" << setw(20) << fixed << setprecision(2) << (order.items[i].first.price * order.items[i].second) << "|"<<endl;
	}
	ss << setw(40) << "SUM" << "|" << setw(21) << " " << setw(20) << order.count() << "|" << setw(20) << order.total() << "|" << endl;;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Action";
	cartMenu.header = ss.str();
	char confirm;
	while (1)
	{
		switch (cartMenu.prompt())
		{
		case -1:
			return order;
			break;
		case 0:
			cout << "Check out? (y/n)";
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') {
				order.createOrderID();
				order.customerID = customer.customerID;
				order.status = "UNPAID";
				order.getDate();
				order.insert();
				cout << "\nOrder saved";
				_getch();
				shop(customer); // go back to shop with empty cart
			}
			break;
		case 1:
			cout << "Clear your cart? (y/n)";
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') {
				shop(customer); // go back to shop with empty cart
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				return order;
			}
			break;
		}

	}
}

//Customer view receipt
void displayReceipt(Customer customer, Order order)
{
	vector<Receipt>result;
	string tableID = customer.tableID;
	Page receiptMenu;
	result = Receipt::showReceipt(order.orderID);

	while (1)
	{
		tableID = customer.tableID;
		stringstream ss;
		ss << setw(47) << "" << " Receipt " << "" << setw(46) << endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << "Table No: " << customer.tableID << endl;
		ss << "Order ID: " << order.orderID << endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << fixed << setw(60) << "NAME" << "|" << setw(20) << "QUANTITY" << "|" << setw(20) << "AMT" << "|" << endl;
		double totalPrice = 0;
		for (int i = 0; i < result.size(); i++)
		{
			ss << fixed << setw(60) << result[i].name << "|" << setw(20) << result[i].quantity << "|" << setw(20) << setprecision(2) << result[i].total << "|" << endl;
			totalPrice += result[i].total;
		}
		receiptMenu.header = ss.str();
		
		stringstream ff;
		ff << "TOTAL: " << fixed << setprecision(2) << totalPrice;
		receiptMenu.footer = ff.str();

		switch (receiptMenu.prompt())
		{
		case -1:
			return;
			break;
		}
	}

}

void StaffRegister()
{
	StaffAcc newAcc;

	Page registerMenu;
	stringstream ss;
	ss << setw(44) << "" << " Registration " << "" << setw(43);
	registerMenu.header = ss.str();
	registerMenu.addOption("Name");
	registerMenu.addOption("Password");
	registerMenu.addOption("Hide/show password");
	registerMenu.addOption("Phone No");
	registerMenu.addOption("Email");
	registerMenu.addOption("Address");
	registerMenu.addOption("Position");
	registerMenu.addOption("Confirm Register");

	string tmpinput;
	bool showPassword = false;
	int option = 0;
	string tmpPwd = "";
	while (1)
	{
		if (showPassword) {

			registerMenu.setValue(2, "SHOW");
			registerMenu.setValue(1, tmpPwd);
		}
		else {
			registerMenu.setValue(2, "HIDE");
			registerMenu.setValue(1, hideString(tmpPwd));
		}
		option = registerMenu.prompt(option);
		switch (option)
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Name: ";
			getline(cin, newAcc.name);
			registerMenu.setValue(0, newAcc.name);
			break;
		case 1:
			cout << "Password: ";
			tmpinput = hiddenInput(tmpinput);
			if (tmpinput.length() < 6)
			{
				cout << "Password must be at least 6 character long";
				_getch();
			}
			else
			{
				tmpPwd = tmpinput;
			}
			break;
		case 2: 
			showPassword = !showPassword;
			break;
		case 3:
			cout << "Phone No: ";
			cin >> newAcc.staffphoneNo;
			registerMenu.setValue(3, newAcc.staffphoneNo);
			break;
		case 4:
			cout << "Email: ";
			cin >> newAcc.staffemail;
			registerMenu.setValue(4, newAcc.staffemail);
			break;
		case 5:
			cout << "Address: ";
			getline(cin, newAcc.address);
			registerMenu.setValue(5, newAcc.address);
			break;
		case 6:
			cout << "Position: ";
			getline(cin, newAcc.position);
			registerMenu.setValue(6, newAcc.position);
			break;
		case 7:
			if (newAcc.name.empty() || newAcc.staffphoneNo.empty() || newAcc.staffemail.empty() || newAcc.address.empty() || newAcc.position.empty())
			{
				cout << "Please complete your informations";
				_getch();
			}
			else
			{
				newAcc.setPassword(tmpPwd);
				newAcc.createStaffID();
				newAcc.add();
				cout << "Registered\nYour staff ID is " + newAcc.staffID;
				_getch();
				return;
			}
			break;
		}
	}
}
void StaffLogIn()
{
	Page loginPage;
	stringstream ss;
	ss << setw(47) << "" << " Login " << "" << setw(47);
	loginPage.header = ss.str();
	loginPage.addOption("Staff ID");
	loginPage.addOption("Password");
	loginPage.addOption("Login");

	StaffAcc staff;
	int option = 0;
	string tmpinput = "";
	string tmpPassword = "";

	while (1)
	{
		option = loginPage.prompt(option);
		switch (option)
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Staff ID: ";
			cin >> staff.staffID;
			loginPage.setValue(0, staff.staffID);
			break;
		case 1:
			cout << "Password: ";
			tmpPassword = hiddenInput(tmpPassword);
			loginPage.setValue(1, hideString(tmpPassword));
			break;
		case 2:
			staff.setPassword(tmpPassword);
			if (staff.login())
			{
				StaffMenu(staff);
			}
			else
			{
				cout << "Invalid Login";
				_getch();
			}
			break;
		}
	}

}
void StaffMenu(StaffAcc staff)
{
	Page staffPage;
	stringstream ss;
	ss << setw(45) << "" << " Staff Menu " << "" << setw(44) <<endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	staffPage.header = ss.str();
	staffPage.addOption("Table");
	staffPage.addOption("Menu");
	staffPage.addOption("Order");
	staffPage.addOption("Payment");
	staffPage.addOption("Customer Information");
	staffPage.addOption("Staff Information");
	staffPage.addOption("Sales Report");
	staffPage.addOption("Log out");
	char confirm;
	while (1)
	{
		switch (staffPage.prompt())
		{
		case 0 :
			manageTable(staff);
			break;
		case 1:
			menuOption(staff);
			break;
		case 2:
			OrderMenu(staff);
			break;
		case 3:
			PaymentMenu(staff);
			break;
		case 4:
			CustomerInfo(staff);
			break;
		case 5:
			staff = StaffInfo(staff);
			break;
		case 6:
			SaleReportMenu(staff);
			break;
		case 7:
			cout << "Check out? (y/n)";
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') {
				main();
			}
			else if(confirm == 'N' || confirm == 'n')
			{
				StaffMenu(staff);
			}
			break;
		}
	}
}

//Staff manage table
void manageTable(StaffAcc staff)
{
	Page table;
	stringstream ss;
	ss << setw(44) << "" << " Table Action " << "" << setw(43) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	table.header = ss.str();
	table.addOption("Add table");
	table.addOption("Delete table");

	while (1)
	{
		switch (table.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			addTable(staff);
			break;
		case 1:
			deleteTable(staff);
			break;
		}
	}
}
void addTable(StaffAcc staff)
{
	vector <Table> list;
	Page table;
	Table newTable;
	list = Table::listTable();
	stringstream ss;
	ss << setw(45) << "" << " Add Table " << "" << setw(45) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID <<endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Table No\n---------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < list.size(); i++)
	{
		ss << left << setw(8) << list[i].tableID << endl;
	}
	table.header = ss.str();
	table.addOption("Table No");
	table.addOption("Confirm");

	while (1)
	{
		switch (table.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Table No: ";
			cin >> newTable.tableID;
			table.setValue(0, newTable.tableID);
			break;
		case 1:
			if (newTable.searchSeat())
			{
				cout << "Table no is already exist";
				_getch();
			}
			else
			{
				newTable.addTable();
				cout << "New table is added";
				_getch();
				return;
			}
			break;
		}
	}
}
void deleteTable(StaffAcc staff)
{
	Page table;
	Table RT; //remove table
	vector <Table> list;
	Table newTable;
	list = Table::listTable();
	stringstream ss;
	ss << setw(44) << "" << " Delete Table " << "" << setw(43) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Table No\n---------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < list.size(); i++)
	{
		ss << left << setw(8) << list[i].tableID << endl;
	}
	table.header = ss.str();
	table.addOption("Table No");
	table.addOption("Confirm");
	char confirm;
	while (1)
	{
		switch (table.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Table No: ";
			cin >> RT.tableID;
			if (RT.searchSeat())
			{
				table.setValue(0, RT.tableID);
			}
			else
			{
				cout << "Please insert a valid Table no";
				_getch();
			}
			break;
		case 1:
			cout << "Delete the current table? (y/n)";
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') 
			{
					RT.removeTable();
					cout << endl << RT.tableID << "is removed";
					_getch();
					return;
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				return;
			}
			break;
		}
	}
}


//Staff manage menu
void menuOption(StaffAcc staff)
{
	Page menuOption;
	stringstream ss;
	ss << setw(45) << "" << " Menu Action " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	menuOption.header = ss.str();
	menuOption.addOption("Add food");
	menuOption.addOption("Update food information");

	while (1)
	{
		switch (menuOption.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			foodMenu(staff); //settle
			break;
		case 1:
			foodUpdate(staff);
			break;
		case 2:
			//foodRemove(staff);
			break;
		}
	}
}
//Add food
void foodMenu(StaffAcc staff)
{
	Food newFood;
	Page foodPage;
	stringstream ss;
	ss << setw(48) << "" << " Menu " << "" << setw(47) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	foodPage.header = ss.str();
	foodPage.addOption("Noodle");
	foodPage.addOption("Rice");
	foodPage.addOption("Beverage");

	while (1)
	{
		switch (foodPage.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			addNoodle(staff);
			break;
		case 1:
			addRice(staff);
			break;
		case 2:
			addBeverage(staff);
			break;
		}
	}
}
void addNoodle(StaffAcc staff)
{
	Food newNoodle;
	Page foodPage;
	stringstream nn; //noodle price
	stringstream ss; //header

	ss << setw(47) << "" << " Noodle " << "" << setw(46) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	foodPage.header = ss.str();

	foodPage.addOption("Name");
	foodPage.addOption("Description");
	foodPage.addOption("Price");
	foodPage.addOption("Add");


	bool valid = true;
	while (1)
	{
		switch (foodPage.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Name: ";
			getline(cin, newNoodle.name);
			foodPage.setValue(0, newNoodle.name);
			break;
		case 1:
			cout << "Description: ";
			getline(cin, newNoodle.description);
			foodPage.setValue(1, newNoodle.description);
			break;
		case 2:
			cout << "Price: ";
			nn.str("");
			cin >> newNoodle.price;
			nn << fixed << setprecision(2) << newNoodle.price;
			foodPage.setValue(2, nn.str());
			break;
		case 3:
			valid = true;
			if (valid)
			{
				newNoodle.type = "Noodle";
				foodPage.setValue(3, newNoodle.type);
				newNoodle.createNoodleID();
				foodPage.setValue(4, newNoodle.foodID);
				newNoodle.addFood();
				cout << "Added";
				_getch();
				foodMenu(staff);
			}
			else
			{
				cout << "Please check you information";
				_getch();
			}
			break;
		}
	}
}
void addRice(StaffAcc staff)
{
	Food newRice;
	Page foodPage;
	stringstream rr; //rice price
	stringstream ss; //header
	ss << setw(48) << "" << " Rice " << "" << setw(47) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	foodPage.header = ss.str();

	foodPage.addOption("Name");
	foodPage.addOption("Description");
	foodPage.addOption("Price");
	foodPage.addOption("Add");

	bool valid = true;
	while (1)
	{
		switch (foodPage.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Name: ";
			getline(cin, newRice.name);
			foodPage.setValue(0, newRice.name);
			break;
		case 1:
			cout << "Description: ";
			getline(cin, newRice.description);
			foodPage.setValue(1, newRice.description);
			break;
		case 2:
			cout << "Price: ";
			rr.str("");
			cin >> newRice.price;
			rr << fixed << setprecision(2) << newRice.price;
			foodPage.setValue(2, rr.str());
			break;
		case 3:
			valid = true;
			if (valid)
			{
				newRice.type = "Rice";
				foodPage.setValue(3, newRice.type);
				newRice.createRiceID();
				foodPage.setValue(4, newRice.foodID);
				newRice.addFood();
				cout << "Added";
				_getch();
			}
			else
			{
				cout << "Please check you information";
				_getch();
			}
			break;
		}
	}
}
void addBeverage(StaffAcc staff)
{
	Food newBeverage;
	Page foodPage;
	stringstream bb; // beverage price
	stringstream ss; //header

	ss << setw(46) << "" << " Beverage " << "" << setw(45) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	foodPage.header = ss.str();
	foodPage.addOption("Name");
	foodPage.addOption("Description");
	foodPage.addOption("Price");
	foodPage.addOption("Add");

	bool valid = true;
	while (1)
	{
		switch (foodPage.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Name: ";
			getline(cin, newBeverage.name);
			foodPage.setValue(0, newBeverage.name);
			break;
		case 1:
			cout << "Description: ";
			getline(cin, newBeverage.description);
			foodPage.setValue(1, newBeverage.description);
			break;
		case 2:
			cout << "Price: ";
			bb.str("");
			cin >> newBeverage.price;
			bb << fixed << setprecision(2) << newBeverage.price;
			foodPage.setValue(2, bb.str());
			break;
		case 3:
			valid = true;
			if (valid)
			{
				newBeverage.type = "Beverage";
				foodPage.setValue(3, newBeverage.type);
				newBeverage.createBeverageID();
				foodPage.setValue(4, newBeverage.foodID);
				newBeverage.addFood();
				cout << "Added";
				_getch();
			}
			else
			{
				cout << "Please check you information";
				_getch();
			}
			break;
		}
	}
}
//Update food
void foodUpdate(StaffAcc staff)
{
	string type;
	Page FU; //food update
	stringstream ss; //header
	ss << setw(44) << "" << " Update Food " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	FU.header = ss.str();
	FU.addOption("Noodle");
	FU.addOption("Rice");
	FU.addOption("Beverage");

	while (1)
	{
		switch (FU.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			type = "Noodle";
			displayFood(staff,type);
			break;
		case 1:
			type = "Rice";
			displayFood(staff,type);
			break;
		case 2:
			type = "Beverage";
			displayFood(staff,type);
			break;
		}
	}
}
void displayFood(StaffAcc staff, string type)
{
	vector <Food> food;
	string ff; //display food
	string sortColumn = "FOODID";
	bool ascending = true;
	string foodID;
	Page FL; //food list
	FL.addOption("Select");
	food = Food::searchFood(type, sortColumn, ascending);

	while (1)
	{
		stringstream ss; //header
		ss << setw(48) << "" << " Menu " << "" << setw(47) << endl;
		if (type == "Noodle")
		{
			ss << setw(47) << "" << " " << type << " " << "" << setw(46) << endl;
		}
		else if (type == "Beverage")
		{
			ss << setw(46) << "" << " " << type << " " << "" << setw(45) << endl;
		}
		else if (type == "Rice")
		{
			ss << setw(48) << "" << " " << type << " " << "" << setw(47) << endl;
		}
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << "Staff Name : " << staff.name << endl;
		ss << "Staff ID   : " << staff.staffID <<endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << fixed << setprecision(2) << setw(15) << "FOOD ID" << "|" << setw(65) << "NAME"
			<< "|" << setw(20) << "PRICE" << "|" << endl;

		for (int i = 0; i < food.size(); i++)
		{
			ss << setw(15) << food[i].foodID << "|" << setw(65) << food[i].name
				<< "|" << fixed << setprecision(2) << setw(20) << food[i].price << "|" << endl;
		}
		FL.header = ss.str();
		switch (FL.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Insert Food ID to select: ";
			cin >> foodID;
			foodInfo(staff, foodID,type);
			break;
		}
	}
}
void foodInfo(StaffAcc staff, string foodID,string type)
{
	Food food = Food::searchFood(foodID);
	if (food.foodID.empty())
	{
		cout << "Food not found";
		_getch();
		return;
	}
	food.type = type;
	food.foodID = foodID;
	Food temp = food;
	Page FU;
	string price="";
	stringstream ss;
	ss << setw(44) << "" << " Update Food " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	FU.header = ss.str();

	FU.addOption("Name");
	FU.addOption("Description");
	FU.addOption("Price");
	FU.addOption("Reset");
	FU.addOption("Save");
	FU.addOption("Delete");


	string tmpInput;
	while (1)
	{
		FU.setValue(0, temp.name);
		FU.setValue(1, temp.description);

		stringstream pp;
		pp << fixed << setprecision(2) << temp.price;
		price = pp.str();

		FU.setValue(2, price);

		switch (FU.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "New name: ";
			getline(cin,temp.name);
			break;
		case 1:
			cout << "New description: ";
			getline(cin, temp.description);
			break;
		case 2:
			cout << "New price: ";
			cin >> temp.price;
			break;
		case 3:
			temp = food;
			break;
		case 4:
			food = temp;
			food.updateFood();
			cout <<endl<< "Updated";
			_getch();
			return;
			break;
		case 5:
			cout << "Delete the current food(y/n)";
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y')
			{
				food.foodRemove();
				food.updateLastRow();
				cout << endl << food.name << " is deleted";
				_getch();
				return;
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				foodUpdate(staff);
			}
			break;
		}
	}
}


//Staff manage order
void OrderMenu(StaffAcc staff)
{
	Page OM; //order manage
	stringstream ss;
	ss << setw(44) << "" << " Order Action " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	OM.header = ss.str();
	Order order;
	OM.addOption("Order ID");

	while (1)
	{
		switch (OM.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Order ID: ";
			cin >> order.orderID;
			if (order.notPaid())
			{
				orderManage(staff,order);
				_getch();
			}
			else
			{
				cout << "Please insert a valid Order ID";
				_getch();
			}
			break;
		}
	}
}
void orderManage(StaffAcc staff, Order order)
{
	string foodID;
	Page OM; //order manage
	vector<Receipt>result;
	result = Receipt::showReceipt(order.orderID);
	stringstream ss;
	ss << setw(44) << "" << " Order Action " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Order ID   : " << order.orderID << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << fixed << setw(15) << "FOOD ID" << "|" << setw(60) << "NAME" << "|" << setw(10) << "QUANTITY" << "|" << setw(10) << "AMT" << "|" << endl;

	double totalPrice = 0;
	for (int i = 0; i < result.size(); i++)
	{
		ss << fixed << setw(15) << result[i].foodID << "|" << setw(60) << result[i].name << "|" << setw(10) << result[i].quantity << "|" << setw(10) << setprecision(2) << result[i].total << "|" << endl;
		totalPrice += result[i].total;
	}
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Total: " << fixed << setprecision(2) << totalPrice;

	OM.header = ss.str();
	OM.addOption("Add");
	OM.addOption("Select");
	OM.addOption("Delete");
	while (1)
	{

		switch (OM.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			staffOrder(staff, order);
		case 1:
			cout << "Insert Food ID to select: ";
			cin >> foodID;
			if (order.foodList(foodID))
			{
				orderInfo(staff, foodID, order);
			}
			else
			{
				cout << "Food not found";
				_getch();
			}
			break;
		case 2:
			cout << "Are you confirm to cancel the order? (y/n)";
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y')
			{
				order.cancelOrder();
				order.updateLastRow();
				return;
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				return;
			}
			break;
		}
	}
}
void staffOrder(StaffAcc staff, Order order)
{
	Food food;
	int quantity = 0;
	stringstream ss;
	ss << setw(44) << "" << " Order Action " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Order ID   : " << order.orderID;
	Page OM;
	OM.header = ss.str();
	OM.addOption("Food ID");
	OM.addOption("Quantity");
	OM.addOption("Confirm");

	while (1)
	{
		switch (OM.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Food ID: ";
			cin >> food.foodID;
			if (food.validFood())
			{
				OM.setValue(0, food.foodID);
			}
			else
			{
				cout << "Please insert a valid Food ID";
				_getch();
			}
			break;
		case 1:
			cout << "Quantity: ";
			cin >> quantity;
			OM.setValue(1, to_string(quantity));
			break;
		case 2:
			cout << "Are you confirm to add the food? (y/n)";
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y')
			{
				if (food.foodID.empty() || quantity == 0)
				{
					cout << "Please check the information";
					_getch();
				}
				else
				{
					order.staffInsert(food.foodID, quantity);
					cout << "Added";
					StaffMenu(staff);
				}
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				return;
			}
			break;
		}
	}
}
void orderInfo(StaffAcc staff, string foodID,Order order)
{
	Order temp = order;
	Page OU;//order update
	string price = "";
	int amt =0;
	stringstream ss;
	ss << setw(44) << "" << " Order Update " << "" << setw(44) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Order ID   : " << order.orderID;
	OU.header = ss.str();
	OU.addOption("Quantity");
	OU.addOption("Save");
	OU.addOption("Delete");

	string tmpInput;
	while (1)
	{
		switch (OU.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "New quantity: ";
			cin >> amt;
			OU.setValue(0, to_string(amt));
			break;
		case 1:
			order = temp;
			order.update(foodID,amt);
			cout << endl << "Updated";
			_getch();
			orderManage(staff,order);
			break;
		case 2:
			cout << "Delete the current food(y/n)";
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y')
			{
				order.remove(foodID);
				cout << "\nDeleted";
				_getch();
				orderManage(staff, order);
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				return;
			}
			break;
		}
	}
}


//Payment
void PaymentMenu(StaffAcc staff)
{
	Payment newPay;
	Page PM; //paymentMenu
	stringstream ss;
	ss << setw(46) << "" << " Payment " << "" << setw(46) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	PM.header = ss.str();
	PM.addOption("Order ID");

	while (1)
	{
		switch (PM.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Order ID: ";
			cin >> newPay.orderID;
			if (newPay.notPaid())
			{
				viewReceipt(staff, newPay);
			}
			else
			{
				cout << "Please insert a valid Order ID";
				_getch();
			}
			break;
		}
	}
}
void viewReceipt(StaffAcc staff, Payment payment)
{
	Page PM;
	Page checkout;
	Order order;
	vector<Receipt>result;
	payment.getTable();
	order.orderID = payment.orderID;
	payment.staffID = staff.staffID;
	PM.addOption("Payment method");
	PM.addOption("Check out");

	Page MM; //METHOD MENU;
	stringstream pp; //payment method
	pp << setw(43) << "" << " Payment Method " << "" << setw(43) << endl;
	pp << "---------------------------------------------------------------------------------------------------------\n";
	pp << "Staff Name : " << staff.name << endl;
	pp << "Staff ID   : " << staff.staffID << endl;
	pp << "---------------------------------------------------------------------------------------------------------\n";
	pp << "Order ID   : " << payment.orderID;
	MM.header = pp.str();
	MM.addOption("Visa");
	MM.addOption("MasterCard");
	MM.addOption("TNG");
	MM.addOption("ShopeePay");
	MM.addOption("GrabPay");

	char confirm;

	result = Receipt::showReceipt(payment.orderID);

	while (1)
	{
		stringstream ss;
		ss << setw(46) << "" << " Payment " << "" << setw(46) << endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << "Staff Name : " << staff.name << endl;
		ss << "Staff ID   : " << staff.staffID << endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << "Order ID   : " << payment.orderID <<endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << setw(10)<< "FOOD ID" << "|" << setw(55) << "NAME" << "|" << setw(15) << "QUANTITY" << "|" << setw(15) << "AMT" << "|" << endl;

		double totalPrice = 0;
		for (int i = 0; i < result.size(); i++)
		{
			ss << setw(10) << result[i].foodID <<"|" << setw(55) << result[i].name << "|" << setw(15) << result[i].quantity << "|" << setw(15) <<fixed<< setprecision(2) << result[i].total << "|" << endl;
			totalPrice += result[i].total;
			payment.totalPrice = totalPrice;
		}
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << "TOTAL      : " << fixed << setprecision(2) <<totalPrice;
		PM.header = ss.str();

		switch (PM.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			switch (MM.prompt())
			{
			case -1:
				return;
				break;
			case 0:
				payment.method = "Visa";
				break;
			case 1:
				payment.method = "MasterCard";
				break;
			case 2:
				payment.method = "TNG";
				break;
			case 3:
				payment.method = "ShopeePay";
				break;
			case 4:
				payment.method = "GrabPay";
				break;
			}
			PM.setValue(0, payment.method);
			break;
		case 1:
			cout << "Check out? (y/n)";
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y') {
				payment.createPaymentID();
				payment.insert();
				order.newStatus();
				cout << "\n\nTHANK YOU! PLS COME AGAIN ^V^";
				_getch();
				main(); // go back to shop with empty cart
			}
			break;
		}
	}


}


//CustomerInfo
void CustomerInfo(StaffAcc staff)
{
	vector<Customer>list;
	Customer customer;
	Page CI; //Customer info
	CI.addOption("Customer ID");
	list = Customer::listCustomer();

	while (1)
	{
		stringstream ss;
		ss << setw(43) << "" << " Customer Update " << "" << setw(42) << endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << "Staff Name : " << staff.name << endl;
		ss << "Staff ID   : " << staff.staffID<<endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << setw(15) << "Customer ID" << "|" << setw(40) << "Name" << "|" << endl;
		for (int i = 0; i < list.size(); i++)
		{
			ss << setw(15) << list[i].customerID << "|" << setw(40) << list[i].nickname << "|" << endl;
		}
		CI.header = ss.str();

		switch (CI.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Customer ID: ";
			cin >> customer.customerID;
			if (customer.searchAcc())
			{
				UpdateCustomer(staff, customer);
				_getch();
			}
			else
			{
				cout << "Please insert a valid Customer ID";
				_getch();
			}
		}
	}

}
void UpdateCustomer(StaffAcc staff, Customer customer)
{
	Page CU; //Customer update;
	Customer temp = customer;
	Table table;
	stringstream ss;
	ss << setw(43) << "" << " Customer Update " << "" << setw(42) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	CU.header = ss.str();
	CU.addOption("Nickname  ");
	CU.addOption("Phone No  ");
	CU.addOption("Email     ");
	CU.addOption("Pax no    ");
	CU.addOption("Table no  ");
	CU.addOption("Save");
	CU.addOption("Reset");

	string tmpinput;
	int option = 0;
	while (1)
	{
		CU.setValue(0, temp.nickname);
		CU.setValue(1, temp.cusphoneNo);
		CU.setValue(2, temp.cusEmail);
		CU.setValue(3, to_string(temp.paxNo));
		CU.setValue(4, temp.tableID);
		CU.footer = "Select Option";

		option = CU.prompt(option);
		switch (option)
		{
		case -1:
			return;
			break;
		case 0:
			cout << "New name: ";
			cin >> temp.nickname;
			break;
		case 1:
			cout << "New Phone No: ";
			cin >> temp.cusphoneNo;
			break;
		case 2:
			cout << "New Email: ";
			cin >> temp.cusEmail;
			break;
		case 3:
			cout << "New Pax No: ";
			cin >> temp.paxNo;
			break;
		case 4:
			cout << "New Table No: ";
			cin >> temp.tableID;
			table.tableID = temp.tableID;
			if (!table.searchSeat())
			{
				cout << "Please insert a valid Table no";
				temp.tableID = customer.tableID;
				_getch();
			}
			break;
		case 5:
			customer = temp;
			customer.update();
			cout << "Your information is updated";
			_getch();
			return;
			break;
		case 6:
			temp = customer;
			break;
		}
	}

	

}


//Update staff info
StaffAcc StaffInfo(StaffAcc staff)
{
	StaffAcc temp = staff;

	Page infoPage;
	stringstream ss;
	ss << setw(42) << "" << " Staff Information " << "" << setw(41) << endl;
	ss << "---------------------------------------------------------------------------------------------------------\n";
	ss << "Staff Name : " << staff.name << endl;
	ss << "Staff ID   : " << staff.staffID;
	infoPage.header = ss.str();
	infoPage.addOption("Name       ");
	infoPage.addOption("Password   ");
	infoPage.addOption("Phone No   ");
	infoPage.addOption("Email      ");
	infoPage.addOption("Address    ");
	infoPage.addOption("Position   ");
	infoPage.addOption("Save");
	infoPage.addOption("Remove Account");

	string tmpInput;
	int option = 0;
	while (1)
	{
		infoPage.setValue(0, temp.name);
		infoPage.setValue(1, hideString(temp.getPassword()));
		infoPage.setValue(2, temp.staffphoneNo);
		infoPage.setValue(3, temp.staffemail);
		infoPage.setValue(4, temp.address);
		infoPage.setValue(5, temp.position);
		infoPage.footer = "Select Option";

		option = infoPage.prompt(option);
		switch (option)
		{
		case -1:
			return staff;
			break;
		case 0:
			cout << "New name: ";
			cin >> temp.name;
			break;
		case 1:
			cout << "Old Password: ";
			tmpInput = hiddenInput();
			if (staff.MatcPasswordWith(tmpInput)) {
				cout << "\nInsert new Password: ";
				tmpInput = hiddenInput();
				if (tmpInput.length() < 6) {
					cout << "Password must be at least 6 character long";
				}
				else {
					temp.setPassword(tmpInput);
				}
			}
			else {
				cout << "Invalid old password";
				_getch();
			}
			break;
		case 2:
			cout << "New Phone No: ";
			cin >> temp.staffphoneNo;
			break;
		case 3:
			cout << "New Email: ";
			cin >> temp.staffemail;
			break;
		case 4:
			cout << "New Address: ";
			cin >> temp.address;
			break;
		case 5:
			cout << "New Position: ";
			cin >> temp.position;
			break;
		case 6:
			staff = temp;
			staff.update();
			cout << "Your information is updated";
			_getch();
		case 7:
			cout << "Are you confirm to remove your staff account? (y/n)";
			char confirm;
			confirm = _getch();
			if (confirm == 'Y' || confirm == 'y')
			{
				staff = temp;
				staff.remove();
				staff.updateLastRow();
				main();
			}
			else if (confirm == 'N' || confirm == 'n')
			{
				return staff;
			}
			break;
		}
	}

}

void Exit()
{

}


void SaleReportMenu(StaffAcc staff) {

	string start, endDate;

	vector<string> typeName;

	bool sortByDate = true, ascending = true;

	Page saleM;
	saleM.addOption("Start");
	saleM.addOption("End");
	saleM.addOption("Generate");

	vector<Sales> result;

	while (1)
	{
		// report display
		stringstream ss;
		// construct our report header
		ss << setw(44) << "" << " Sales Report " << "" << setw(44) << endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << "Staff Name : " << staff.name << endl;
		ss << "Staff ID   : " << staff.staffID<<endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << setw(30) << "DATE" << "|";
		ss << setw(30) << "TYPE" << "|";
		ss << setw(30) << "SALE" << "|";

		double totalSale = 0;
		// repeat same structure for content of the report
		for (int i = 0; i < result.size(); i++) {
			ss << endl << setw(30) << result[i].date.substr(0, 7) << "|";
			ss << setw(30) << result[i].typeName << "|";
			ss << setw(30) << fixed << setprecision(2) << result[i].value << "|";
			totalSale += result[i].value;

		}
		ss << "\n---------------------------------------------------------------------------------------------------------\n";
		ss << "TOTAL SALE: " << fixed << setprecision(2) << totalSale << endl;
		ss << "---------------------------------------------------------------------------------------------------------\n";
		ss << setw(46) << "" << " End Report " << "" << setw(45);
		saleM.header = ss.str();


		switch (saleM.prompt())
		{
		case -1:
			return;
			break;
		case 0:
			cout << "Inser start date (yyyy/mm/dd): ";
			cin >> start;
			if (!validDate(start))
			{
				cout << "Please insert a valid date";
				_getch();
			}
			else {
				saleM.setValue(0, start);
			}
			break;
		case 1:
			cout << "Inser end date (yyyy/mm/dd): ";
			cin >> endDate;
			if (!validDate(endDate))
			{
				cout << "Please insert a valid date";
				_getch();
			}
			else {
				saleM.setValue(1, endDate);
			}
			break;
		case 2:
			result.clear();
			result = Sales::salesReport(start, endDate, typeName, sortByDate, ascending);

			ofstream outputFile("Sales_report.txt");
			if (outputFile.is_open()) {
				stringstream ff;
				// construct our report header
				ff << setw(44) << "" << " Sales Report " << "" << setw(44) << endl;
				ff << "---------------------------------------------------------------------------------------------------------\n";
				ff << "Staff Name : " << staff.name << endl;
				ff << "Staff ID   : " << staff.staffID << endl;
				ff << "---------------------------------------------------------------------------------------------------------\n";
				ff << setw(30) << "DATE" << "|";
				ff << setw(30) << "TYPE" << "|";
				ff << setw(30) << "SALE" << "|";

				double totalSale = 0;
				// repeat same structure for content of the report
				for (int i = 0; i < result.size(); i++) {
					ff <<endl << setw(30) << result[i].date.substr(0, 7) << "|";
					ff << setw(30) << result[i].typeName << "|";
					ff << setw(30) << fixed << setprecision(2) << result[i].value << "|";
					totalSale += result[i].value;

				}

				ff << "\n---------------------------------------------------------------------------------------------------------\n";
				ff << "Start Date : " << start << endl;
				ff << "End Date   : " << endDate <<endl<<endl;
				ff<< "TOTAL SALE  : " << fixed << setprecision(2) << totalSale<<endl;
				ff << "---------------------------------------------------------------------------------------------------------\n";
				ff << setw(46) << "" << " End Report " << "" << setw(45);
				outputFile << ff.str();

				outputFile.close();
			}
			else {
				cerr << "Unable to open the file for writing\n";
			}
			break;
		}

	}
}



string hiddenInput(string prevValue) {
	string input = "";
	char tmp = '\0';
	while (1) {
		tmp = _getch();
		switch (tmp)
		{
		case 13:// ASCII code for enter key
			return input;
			break;
		case 27://ASCII code for escape key
			return prevValue; //return the previous value insteadd to cancel
			break;
		case 8:
			if (input.length() > 0) {
				input.erase(input.size() - 1); // erase last index
				cout << "\b \b";
				//print  this which will move back caret and replace character with space and move back caret 1 more time to imitate backspace
			}
			break;
		default://for any other key press
			if (tmp >= 32 && tmp <= 126) {
				/// 32 until 126 is basically all the printable character in ASCII table,
				// modify this range if you want to limit input, 
				// for example 48~57 will only allow number input, 65~90 only uppercase, 97 ~122 only lowercase etc 
				//when key press is within our acceptable range append the character into string
				input += tmp;
				//display a * instead
				cout << "*";
			}
			//if the key press is outside of our allowed range simply skips it to ignore
			break;
		}
	}
}

// simple hiding string by replacing all character with a specific character *
string hideString(string tohide) {
	string hidden = "";
	for (int i = 0; i < tohide.size(); i++) {
		hidden += '*';
	}
	return hidden;
}

// simple hiding string by replacing all character with a specific character *
string hideString(int tohideLenght) {
	string hidden = "";
	for (int i = 0; i < tohideLenght; i++) {
		hidden += '*';
	}
	return hidden;
}

//check the format of date
bool validDate(const string& date) 
{
	regex datePattern("^\\d{4}[/-]\\d{2}[/-]\\d{2}$"); //Define date pattern

	// Check if the date matches the pattern
	if (!regex_match(date, datePattern)) {
		return false;
	}

	// Validate the calendar date
	tm tmDate = {};
	stringstream ss(date);
	ss >> get_time(&tmDate, "%Y/%m/%d");

	if (ss.fail()) {
		ss.clear();
		ss.str(date);
		ss >> get_time(&tmDate, "%Y-%m-%d");
		if (ss.fail()) {
			return false;
		}
	}

	// Check if the date is valid
	return mktime(&tmDate) != -1;
}