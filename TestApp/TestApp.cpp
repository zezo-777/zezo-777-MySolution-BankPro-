#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;
const string ClientsFileName = "ClientsData.txt";
const string UsersDataFile = "UsersData.txt";
void Start();
void RunTransactionsMenu();
void RunMangeUsersMenu();
struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	int AccountBalance = 0;
	bool MarkForDelete = false;
};
struct stUser
{
	string Username = "";
	string Password = "";
	short permissions = 0;
	bool MarkForDelete = false;
};
enum enMainMenuPermissions {
	eAll = -1, pClientList = 1, pAddNewClien = 2, pDeleteClient = 4, pUpdateClient = 8,
	pFindClient = 16, pTransactions = 32, pManageUsers = 64
};
stUser CurrentUser;
void ShowAccessDeniedMessage()
{
	cout << "\n-------------------------------------\n";
	cout << "Access Denied,\n";
	cout << "You don't have permission to do this,\n";
	cout << "Please contact your admin.\n";
	cout << "\n-------------------------------------\n";
}
vector <string> SplitString(string S1, string delim = "#//#")
{
	vector <string> Vstring;
	short pos = 0;
	string sWord;
	while ((pos = S1.find(delim)) != string::npos) {
		sWord = S1.substr(0, pos);
		if (!sWord.empty())
		{
			Vstring.push_back(sWord);

		}
		S1.erase(0, pos + delim.length());
	}

	if (S1 != "")
	{
		Vstring.push_back(S1);

	}
	return Vstring;
}
stClient CounvertLineToRecord(string Line)
{
	stClient Clinet;
	vector <string> vClinetData;
	vClinetData = SplitString(Line);
	Clinet.AccountNumber = vClinetData[0];
	Clinet.PinCode = vClinetData[1];
	Clinet.Name = vClinetData[2];
	Clinet.Phone = vClinetData[3];
	Clinet.AccountBalance = stod(vClinetData[4]);
	return Clinet;
}
stUser CounvertLineToRecordUsers(string Line)
{
	stUser User;
	vector<string> vUser;
	vUser = SplitString(Line);
	User.Username = vUser[0];
	User.Password = vUser[1];
	User.permissions = stoi(vUser[2]);
	return User;
}
vector <stClient> LoadClientsDataFromFile(string FileName)
{
	vector <stClient> vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stClient Clinet;
		while (getline(MyFile, Line))
		{
			Clinet = CounvertLineToRecord(Line);
			vClient.push_back(Clinet);
		}
		MyFile.close();
	}
	return vClient;
}
vector <stUser> LoadUsersDataFromFile(string FileName)
{
	vector <stUser> vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stUser User;
		while (getline(MyFile, Line))
		{
			User = CounvertLineToRecordUsers(Line);
			vUser.push_back(User);
		}
		MyFile.close();
	}
	return vUser;
}
bool IsAccountNumberExist(string AccountNumber, vector<stClient>& vClients) {
	for (stClient& Client : vClients) {
		if (Client.AccountNumber == AccountNumber) {
			return true;
		}
	}
	return false;
}
bool IsUsernameExist(string Username, vector<stUser>& vUsers) {
	for (stUser& U : vUsers) {
		if (U.Username == Username) {
			return true;
		}
	}
	return false;
}
stClient ReadNewClient()
{
	stClient Client;
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	do {
		cout << "Enter Account Number: ";
		getline(cin >> ws, Client.AccountNumber);
		if (IsAccountNumberExist(Client.AccountNumber, vClients)) {
			cout << "Account number ( " << Client.AccountNumber << " ) already exists.Please enter another account number.\n";
		}
	} while (IsAccountNumberExist(Client.AccountNumber, vClients));
	cout << "Enter PinCode ? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name ? ";
	getline(cin, Client.Name);
	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance ? ";
	cin >> Client.AccountBalance;
	return Client;
}
short ReadPermissionsToSet()
{
	short Permissions = 0;
	char Answer = 'Y';
	cout << "Do you want to give full access? (Y/N) : ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		return -1;
	}
	cout << "\nDo you want to give access to:\n";
	cout << "Show Client List ?(Y/N): ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuPermissions::pClientList;
	}
	cout << "Add New Client ?(Y/N): ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuPermissions::pAddNewClien;
	}
	cout << "Delete Client ?(Y/N): ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuPermissions::pDeleteClient;
	}
	cout << "Update Client ?(Y/N): ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuPermissions::pUpdateClient;
	}
	cout << "Find Client ?(Y/N): ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuPermissions::pFindClient;
	}
	cout << "Transactions ?(Y/N): ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuPermissions::pTransactions;
	}
	cout << "Manage Users ?(Y/N): ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuPermissions::pManageUsers;
	}
	return Permissions;
}
bool CheckAccessPermission(enMainMenuPermissions Permission)
{
	if (CurrentUser.permissions == enMainMenuPermissions::eAll)
		return true;
	if ((Permission & CurrentUser.permissions) == Permission)
		return true;
	else
		return false;
}
stUser ReadNewUser()
{
	stUser User;
	vector <stUser> vUsers = LoadUsersDataFromFile(UsersDataFile);
	do {
		cout << "Enter Username: ";
		getline(cin >> ws, User.Username);
		if (IsUsernameExist(User.Username, vUsers))
		{
			cout << "Username ( " << User.Username << " ) already exists.Please enter another Username.\n";
		}
	} while (IsUsernameExist(User.Username, vUsers));
	cout << "Enter Password ? ";
	getline(cin, User.Password);
	User.permissions = ReadPermissionsToSet();
	return User;
}
string CounvertRecordToLine(stClient Client, string Separator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}
string CounvertRecordToLineUsers(stUser User, string Separator = "#//#")
{
	string stUserRecord = "";
	stUserRecord += User.Username + Separator;
	stUserRecord += User.Password + Separator;
	stUserRecord += to_string(User.permissions);
	return stUserRecord;
}
void AddDataLineToFile(string FileName, string strDataLine)
{
	fstream MyFile;
	char Revision = 'y';
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << strDataLine << endl;
		MyFile.close();
	}
}
void AddNewClient()
{
	stClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, CounvertRecordToLine(Client));
}
void AddNewUser()
{
	stUser User;
	User = ReadNewUser();
	AddDataLineToFile(UsersDataFile, CounvertRecordToLineUsers(User));
}
void AddClients()
{
	system("cls");
	if (!CheckAccessPermission(enMainMenuPermissions::pAddNewClien))
	{
		ShowAccessDeniedMessage();
		system("pause>0");
		Start();
	}
	cout << "\n==================================================\n";
	cout << "\t\t Adding New Client Screen \n";
	cout << "==================================================\n";
	char AddMore = 'Y';
	do
	{
		AddNewClient();
		cout << "\nClient Added Successfully , do you want to add more clients ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}
void AddUsers()
{
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Adding New User Screen \n";
	cout << "==================================================\n";
	char AddMore = 'Y';
	do
	{
		AddNewUser();
		cout << "\nUser Added Successfully , do you want to add more Users ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}
void PrintClientRecord(stClient Client)
{
	cout << "\n\nThe following are the client Details : \n\n";
	cout << "==================================================\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "PinCode : " << Client.PinCode << endl;
	cout << "Name : " << Client.Name << endl;
	cout << "Phone : " << Client.Phone << endl;
	cout << "Account Balance : " << Client.AccountBalance << endl;
	cout << "==================================================\n";
}
void PrintUserRecord(stUser User)
{
	cout << "\n\nThe following are the User Details : \n\n";
	cout << "==================================================\n";
	cout << "Username : " << User.Username << endl;
	cout << "password : " << User.Password << endl;
	cout << "Permissions : " << User.permissions << endl;
	cout << "==================================================\n";
}
bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client)
{
	for (stClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool FindUserByUsername(string Username, vector <stUser> vUsers, stUser& User)
{
	for (stUser U : vUsers)
	{
		if (U.Username == Username)
		{
			User = U;
			return true;
		}
	}
	return false;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
bool MarkUserForDeleteByUsername(string Username, vector <stUser>& vUsers)
{
	for (stUser& U : vUsers)
	{
		if (U.Username == Username)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		for (stClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = CounvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}
vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser> vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		for (stUser U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				DataLine = CounvertRecordToLineUsers(U);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUsers;
}
bool DeleteClientByAccountNumber()
{
	system("cls");
	if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		system("pause>0");
		Start();
	}
	cout << "\n==================================================\n";
	cout << "\t\t Delete Client Screen \n";
	cout << "==================================================\n";
	stClient Client;
	string AccountNumber = "";
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter AccountNumber ? ";
		cin >> AccountNumber;
		if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found! \n";
		}

	} while (!FindClientByAccountNumber(AccountNumber, vClients, Client));

	PrintClientRecord(Client);
	cout << "\n\nAre you sure you want delete client ? n/y ?";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
		SaveClientsDataToFile(ClientsFileName, vClients);
		vClients = LoadClientsDataFromFile(ClientsFileName);
		cout << "\n\n Client Deleted Successfully. \n";
		return true;
	}
}
bool DeleteUserByUsername()
{
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Delete User Screen \n";
	cout << "==================================================\n";
	stUser User;
	string Username = "";
	vector <stUser> vUsers = LoadUsersDataFromFile(UsersDataFile);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter Username ? ";
		cin >> Username;
		if (Username == "Admin")
		{
			cout << "\n\t=================================\n";
			cout << "\t|| You Can't Delete This User. ||";
			cout << "\n\t=================================\n";
			return false;
		}
		if (!FindUserByUsername(Username, vUsers, User))
		{
			cout << "\n" << Username << " is NOT Found! \n";
		}

	} while (!FindUserByUsername(Username, vUsers, User));

	PrintUserRecord(User);
	cout << "\n\nAre you sure you want delete This User ? n/y ?";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		MarkUserForDeleteByUsername(Username, vUsers);
		SaveUsersDataToFile(UsersDataFile, vUsers);
		vUsers = LoadUsersDataFromFile(UsersDataFile);
		cout << "\n\n User Deleted Successfully. \n";
		return true;
	}

}
void AccountToDeposit(string AccountNumber, vector <stClient>& vClients, float DepositAmount)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.AccountBalance += DepositAmount;
		}
	}
}
void DepositMoney()
{
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Depoist Screen \n";
	cout << "==================================================\n";
	stClient Client;
	string AccountNumber = "";
	float DepositAmount = 0;
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter AccountNumber ? ";
		cin >> AccountNumber;
		if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found! \n";
		}

	} while (!FindClientByAccountNumber(AccountNumber, vClients, Client));

	PrintClientRecord(Client);
	cout << "\n\nAre you sure you want to make Deposit ? n/y ?";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		cout << "Enter Deposit Amount ?";
		cin >> DepositAmount;
		AccountToDeposit(AccountNumber, vClients, DepositAmount);
		SaveClientsDataToFile(ClientsFileName, vClients);
		vClients = LoadClientsDataFromFile(ClientsFileName);
		cout << "\n\n Deposit done Successfully. \n";
	}

}
void AccountToWithdraw(string AccountNumber, vector <stClient>& vClients)
{
	float WithdrawAmount = 0;

	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			do
			{
				cout << "Enter Withdraw Amount ?";
				cin >> WithdrawAmount;
				if (WithdrawAmount > C.AccountBalance)
				{
					cout << "Amount Exceeds The Balance You Can Withdraw up to : " << C.AccountBalance << endl;
				}
			} while (WithdrawAmount > C.AccountBalance);
			C.AccountBalance -= WithdrawAmount;
		}
	}
}
void WithdrawMoney()
{
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Withdraw Screen \n";
	cout << "==================================================\n";
	stClient Client;
	string AccountNumber = "";
	float DepositAmount = 0;
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter AccountNumber ? ";
		cin >> AccountNumber;
		if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found! \n";
		}

	} while (!FindClientByAccountNumber(AccountNumber, vClients, Client));
	PrintClientRecord(Client);
	cout << "\n\nAre you sure you want to make Withdraw ? n/y ?";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		AccountToWithdraw(AccountNumber, vClients);
		SaveClientsDataToFile(ClientsFileName, vClients);
		vClients = LoadClientsDataFromFile(ClientsFileName);
		cout << "\n\n Withdraw done Successfully. \n";
	}

}
stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "Enter PinCode ? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name ? ";
	getline(cin, Client.Name);
	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance ? ";
	cin >> Client.AccountBalance;
	return Client;
}
stUser ChangeUserRecord(string Username)
{
	stUser User;
	User.Username = Username;
	cout << "Enter Password ? ";
	getline(cin >> ws, User.Password);
	User.permissions = ReadPermissionsToSet();
	return User;
}
float CalculateTotalBalances(vector <stClient> vClients)
{
	float TotalBalances = 0;
	for (stClient& C : vClients)
	{
		TotalBalances += C.AccountBalance;
	}
	return TotalBalances;
}
void PrintClientRecord3(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}
void ShowTotalBalances()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\t Client List (" << vClients.size() << ") Client(s). ";
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(12) << "Balance ";
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
	for (stClient Client : vClients)
	{
		PrintClientRecord3(Client);
		cout << endl;
	}
	cout << "\t\t\t  Total Balances = " << CalculateTotalBalances(vClients);
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
}
bool UpdateClientByAccountNumber()
{
	system("cls");
	if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClient))
	{
		ShowAccessDeniedMessage();
		system("pause>0");
		Start();
	}
	cout << "\n==================================================\n";
	cout << "\t\t Update Clients Info Screen \n";
	cout << "==================================================\n";
	stClient Client;
	string AccountNumber = "";
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter AccountNumber ? ";
		cin >> AccountNumber;
		if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found! \n";
		}

	} while (!FindClientByAccountNumber(AccountNumber, vClients, Client));
	PrintClientRecord(Client);
	cout << "\n\nAre you sure you want Update client ? n/y ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C = ChangeClientRecord(AccountNumber);
				break;
			}
		}
		SaveClientsDataToFile(ClientsFileName, vClients);
		cout << "\n\n Client Updated Successfully. \n";
		return true;
	}
}
bool UpdateUserByUsername()
{
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Update Users Info Screen \n";
	cout << "==================================================\n";
	stUser User;
	string Username = "";
	vector <stUser> vUsers = LoadUsersDataFromFile(UsersDataFile);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter Username ? ";
		cin >> Username;
		if (!FindUserByUsername(Username, vUsers, User))
		{
			cout << "\n User with Username (" << Username << ") is NOT Found! \n";
		}

	} while (!FindUserByUsername(Username, vUsers, User));
	PrintUserRecord(User);
	cout << "\n\nAre you sure you want Update this user ? n/y ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (stUser& U : vUsers)
		{
			if (U.Username == Username)
			{
				U = ChangeUserRecord(Username);
				break;
			}
		}
		SaveUsersDataToFile(UsersDataFile, vUsers);
		cout << "\n\n User Updated Successfully. \n";
		return true;
	}
}
void SearchForClient()
{
	system("cls");
	if (!CheckAccessPermission(enMainMenuPermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		system("pause>0");
		Start();
	}
	cout << "\n==================================================\n";
	cout << "\t\t Search For Client Screen \n";
	cout << "==================================================\n";
	stClient Client;
	string AccountNumber = "";
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter AccountNumber ? ";
		cin >> AccountNumber;
		if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found! \n";
		}

	} while (!FindClientByAccountNumber(AccountNumber, vClients, Client));
	PrintClientRecord(Client);
}
void SearchUser()
{
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Search For User Screen \n";
	cout << "==================================================\n";
	stUser User;
	string Username = "";
	vector <stUser> vUsers = LoadUsersDataFromFile(UsersDataFile);
	char Answer = 'n';
	do
	{
		cout << "\nPlease enter Username ? ";
		cin >> Username;
		if (!FindUserByUsername(Username, vUsers, User))
		{
			cout << "\n Username (" << Username << ") is NOT Found! \n";
		}

	} while (!FindUserByUsername(Username, vUsers, User));
	PrintUserRecord(User);
}
void PrintClientRecord2(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}
void PrintUsersRecord2(stUser User)
{
	cout << "| " << left << setw(15) << User.Username;
	cout << "| " << left << setw(10) << User.Password;
	cout << "| " << left << setw(30) << User.permissions;
}
void PrintAllClientsData()
{
	system("cls");
	if (!CheckAccessPermission(enMainMenuPermissions::pClientList))
	{
		ShowAccessDeniedMessage();
		system("pause>0");
		Start();
	}
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\t Client List (" << vClients.size() << ") Client(s). ";
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code ";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(12) << "Phone ";
	cout << "| " << left << setw(12) << "Balance ";
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
	for (stClient Client : vClients)
	{
		PrintClientRecord2(Client);
		cout << endl;
	}
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
}
void PrintAllUsersData()
{
	system("cls");
	vector <stUser> vUsers = LoadUsersDataFromFile(UsersDataFile);
	cout << "\n\t\t\t\t\t User List (" << vUsers.size() << ") User(s). ";
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
	cout << "| " << left << setw(15) << "Username";
	cout << "| " << left << setw(10) << "Password ";
	cout << "| " << left << setw(30) << "permissions";
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
	for (stUser User : vUsers)
	{
		PrintUsersRecord2(User);
		cout << endl;
	}
	cout << "\n---------------------------------------------------------";
	cout << "-------------------------------------------\n" << endl;
}
void ShowMainMenue()
{
	system("cls");
	cout << "==================================================\n";
	cout << "\t\t Main Menue Screen \n";
	cout << "==================================================\n";
	cout << "\t [1] Show Client List .\n";
	cout << "\t [2] Add New Client .\n";
	cout << "\t [3] Delete Client .\n";
	cout << "\t [4] Update Client Info .\n";
	cout << "\t [5] Find Client .\n";
	cout << "\t [6] Transactions .\n";
	cout << "\t [7] Manage Users .\n";
	cout << "\t [8] Logout .\n";
	cout << "==================================================\n";
}
void ManageUsersMenuScreen()
{
	system("cls");
	cout << "==================================================\n";
	cout << "\t\t Manage Users Menu Screen \n";
	cout << "==================================================\n";
	cout << "\t [1] Show Users List .\n";
	cout << "\t [2] Add New User .\n";
	cout << "\t [3] Delete  User .\n";
	cout << "\t [4] Update User Info .\n";
	cout << "\t [5] Find User .\n";
	cout << "\t [6] Main Menu .\n";
	cout << "==================================================\n";
}
void ShowTransactionsMenu()
{
	system("cls");
	cout << "==================================================\n";
	cout << "\t\t Transactions Menu Screen \n";
	cout << "==================================================\n";
	cout << "\t [1] Deposit .\n";
	cout << "\t [2] WithDraw .\n";
	cout << "\t [3] Total Balances .\n";
	cout << "\t [4] Main Menu .\n";
	cout << "==================================================\n";
}
void GoBackToMainMenu()
{
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	Start();
}
void GoBackToTransactionsMenu()
{
	cout << "\n\nPress any key to go back to previous Menu...";
	system("pause>0");
	RunTransactionsMenu();
}
void GoBackToMangeUsersMenu()
{
	cout << "\n\nPress any key to go back to previous Menu...";
	system("pause>0");
	RunMangeUsersMenu();
}
void RunTransactionsMenu()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pTransactions))
	{
		ShowAccessDeniedMessage();
		system("pause>0");
		Start();
	}
	short UserChoice = 0;
	ShowTransactionsMenu();
	cout << "Chose What Do You Want To Do [1 to 4 ] ? ";
	cin >> UserChoice;
	switch (UserChoice) {
	case 1:

		DepositMoney();
		GoBackToTransactionsMenu();
		break;
	case 2:

		WithdrawMoney();
		GoBackToTransactionsMenu();
		break;
	case 3:
		system("cls");
		ShowTotalBalances();
		GoBackToTransactionsMenu();
		break;
	case 4:
		system("pause>0");
		Start();
		break;
	}
}
void RunMangeUsersMenu()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		system("pause>0");
		Start();
	}
	short UserChoice = 0;
	ManageUsersMenuScreen();
	cout << "Chose What Do You Want To Do [1 to 6 ] ? ";
	cin >> UserChoice;
	switch (UserChoice)
	{
	case 1:
		PrintAllUsersData();
		GoBackToMangeUsersMenu();
		break;
	case 2:
		AddUsers();
		GoBackToMangeUsersMenu();
		break;
	case 3:
		system("cls");
		DeleteUserByUsername();
		GoBackToMangeUsersMenu();
		break;
	case 4:
		system("cls");
		UpdateUserByUsername();
		GoBackToMangeUsersMenu();
		break;
	case 5:
		system("cls");
		SearchUser();
		GoBackToMangeUsersMenu();
		break;
	case 6:
		system("pause>0");
		Start();
		break;
	}
}
bool FindUserByUserNameAndPassword(string Username, string Password, stUser& User)
{
	vector<stUser>vUser;
	vUser = LoadUsersDataFromFile(UsersDataFile);
	for (stUser U : vUser)
	{
		if (U.Username == Username && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}
string ReadUsername()
{
	string Username = "";
	cout << "\nPlease Enter Username: ";
	cin >> Username;
	return Username;
}
string ReadPassword()
{
	string Password = "";
	cout << "Please Enter Password: ";
	cin >> Password;
	return Password;
}
bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByUserNameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}
void ShowLoginScreen()
{
	cout << "\n==================================================\n";
	cout << "\t\t Login Screen (-_-)  \n";
	cout << "==================================================\n";
}
void LoginSrceen()
{
	bool LoginFailed = false;
	do
	{
		ShowLoginScreen();
		if (LoginFailed)
		{
			cout << "Invalid Username/Password!\n";
		}
		string Username = ReadUsername();
		string Password = ReadPassword();
		LoginFailed = !LoadUserInfo(Username, Password);
	} while (LoginFailed);
	Start();
}
void Start()
{
	short UserChoice = 0;
	ShowMainMenue();
	cout << "Chose What Do You Want To Do [1 to 7 ] ? ";
	cin >> UserChoice;
	switch (UserChoice) {
	case 1:
		PrintAllClientsData();
		GoBackToMainMenu();
		break;
	case 2:
		AddClients();
		GoBackToMainMenu();
		break;
	case 3:

		DeleteClientByAccountNumber();
		GoBackToMainMenu();
		break;
	case 4:

		UpdateClientByAccountNumber();
		GoBackToMainMenu();
		break;
	case 5:

		SearchForClient();
		GoBackToMainMenu();
		break;
	case 6:
		system("cls");
		RunTransactionsMenu();
		break;
	case 7:
		system("cls");
		RunMangeUsersMenu();
		break;
	case 8:
		system("cls");
		LoginSrceen();
		break;
		break;
	}
}
int main()
{
	system("color F0");
	LoginSrceen();
	system("pause>0");
}