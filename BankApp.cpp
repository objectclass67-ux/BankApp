#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <iomanip>
using namespace std;

const string FileName = "/storage/emulated/0/BankApp/Client.text";
const string FileNameUsers = "/storage/emulated/0/BankApp/Users.text";
enum enPermissions {eAll=-1, eShow=1, eAdd=2, eDelete=4, eUpdate=8, eFind=16, eTransactions=32, eManage=64};
struct stClient
{
    string AccountNumber = "";
    string Name = "";
    string PinCode = "";
    string Phone = "";
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
    bool IsFound = false;
};
struct stUsers
{
    string UserName = "";
    string PassWord = "";
    int Permissions = -1;
    bool MarkUserForDelete = false;
    bool IsFoundUser = false;
};
stUsers CurrentUser;
void Login(vector<stUsers> &vUsers, vector<stClient> &vClients);
string ReadString(string Message)
{
    string S;
    cout << Message;
    getline(cin >> ws, S);
    return S;
}
short ReadNumberInRange(string Message, short from, short to)
{
    short Number;
    cout << Message;
    cin >> Number;
    while (cin.fail() || Number > to || Number < from)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nWrong Input , " << Message;
        cin >> Number;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return Number;
}
stClient ChangeClientRecord(string AccountNum)
{
    stClient Client;
    Client.AccountNumber = AccountNum;
    cout << "\nEnter PinCode ? ";
    getline(cin >> ws, Client.PinCode);
    cout << "\nEnter Name ? ";
    getline(cin, Client.Name);
    cout << "\nEnter Phone ? ";
    getline(cin, Client.Phone);
    cout << "\nEnter AccountBalance ? ";
    cin >> Client.AccountBalance;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return Client;
}
vector<string> SplitString(string S, string Delim = "#//#")
{
    vector<string> vString;
    S += Delim;
    string sWord = "";
    short pos = 0;
    while ((pos = S.find(Delim)) != std::string::npos)
    {
        sWord = S.substr(0, pos);
        if (sWord != " ")
        {
            vString.push_back(sWord);
        }
        S.erase(0, pos + Delim.length());
    }
    return vString;
}
void AccessDeniedMessage()
{
    cout << "\n-----------------------------------\n";
    cout << "Access Denied , ";
    cout << "\nYou don't have permission to do this , ";
    cout << "\nplease contact your admin. "; 
    cout << "\n-----------------------------------\n";
}
bool UserPermissionsVerify(int Permissions)
{
    if(CurrentUser.Permissions == enPermissions::eAll)
    {
        return true;
    }
    if((Permissions & CurrentUser.Permissions) == Permissions)
    {
        return true;
    }
    return false;
}
int ReadPermissions()
{
    int Permissions = 0;
    char Answer = 'n';
    
    cout << "\nDo you want to give full access ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        return enPermissions::eAll;
    }
    
    cout << "\nShow Client List ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        Permissions |= enPermissions::eShow;
    }
    
    cout << "\nAdd New Client ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        Permissions |= enPermissions::eAdd;
    }
    
    cout << "\nDelete Client ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        Permissions |= enPermissions::eDelete;
    }
    
    cout << "\nUpdate Client ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        Permissions |= enPermissions::eUpdate;
    }
    
    cout << "\nFind Client ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        Permissions |= enPermissions::eFind;
    }
    
    cout << "\nTransactions Menu ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        Permissions |= enPermissions::eTransactions;
    }
    
    cout << "\nManage Users ? (y/n) ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(tolower(Answer)=='y')
    {
        Permissions |= enPermissions::eManage;
    }
    
    return Permissions;
}
stClient ConvertLineToRecord(string S, string Separator = "#//#")
{
    stClient Client;
    vector<string> vString;
    vString = SplitString(S, Separator);
    Client.AccountNumber = vString.at(0);
    Client.PinCode = vString.at(1);
    Client.Name = vString.at(2);
    Client.Phone = vString.at(3);
    Client.AccountBalance = stod(vString.at(4));
    return Client;
}
string ConvertRecordToLine(stClient Client, string Separator = "#//#")
{
    string sClient;
    sClient += Client.AccountNumber + Separator;
    sClient += Client.PinCode + Separator;
    sClient += Client.Name + Separator;
    sClient += Client.Phone + Separator;
    sClient += to_string(Client.AccountBalance);
    return sClient;
}
vector<stClient> LoadClientsFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream File;
    File.open(FileName, ios::in);
    if (!File.is_open())
    {
        File.open(FileName, ios::out);
        if (File.is_open())
        {
            File.close();
            File.open(FileName, ios::in);
        }
    }
    if (File.is_open())
    {
        stClient Client;
        string Line = "";
        while (getline(File, Line))
        {
            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        File.close();
    }
    return vClients;
}
void PrintDataClient(stClient Client)
{
    cout << "|" << left << setw(14) << Client.AccountNumber;
    cout << "|" << left << setw(8) << Client.PinCode;
    cout << "|" << left << setw(17) << Client.Name;
    cout << "|" << left << setw(11) << Client.Phone;
    cout << "|" << left << setw(9) << Client.AccountBalance << "|\n";
]}
void PrintHeaderTable(short NumOfClients)
{
    printf("\t\tClient List (%d) Client(s).\n", NumOfClients);
    cout << "\n_________________________________________________________________";
    cout << "\n|Account Number|";
    cout << "Pin Code|"
         << "  Account Name   |";
    cout << "   Phone   |"
         << " Balance |";
    cout << "\n|______________|________|_________________|___________|_________|\n";
}
void BackToTheMenu()
{
    cout << "\nPress Enter to return to the menu.....\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    system("clear");
}
void PrintClientsList(vector<stClient> &vClients)
{
    if(!UserPermissionsVerify(enPermissions::eShow))
    {
        AccessDeniedMessage();
        return;
    }
    vClients = LoadClientsFromFile(FileName);
    PrintHeaderTable(vClients.size());
    for (stClient &Client : vClients)
    {
        PrintDataClient(Client);
    }
    cout << "|______________|________|_________________|___________|_________|\n";
}
stClient FindClientInFile(string FileName, string AccountNum)
{
    stClient Client;
    vector<stClient> vClients = LoadClientsFromFile(FileName);
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNum)
        {
            C.IsFound = true;
            Client = C;
            break;
        }
    }
    return Client;
}
stClient ReadNewClient()
{
    stClient Client;
    string AccountNum;
    AccountNum = ReadString("\nEnter Account Number : ");
    Client = FindClientInFile(FileName, AccountNum);
    while (Client.IsFound)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nError,Account is found.\n";
        AccountNum = ReadString("\nEnter Account Number : ");
        Client = FindClientInFile(FileName, AccountNum);
    }
    Client = ChangeClientRecord(AccountNum);
    return Client;
}
void SaveDataClientInFile(string FileName, string stDataLine)
{
    fstream File;
    File.open(FileName, ios::out | ios::app);
    if (File.is_open())
    {
        File << stDataLine << endl;
        File.close();
    }
    else
        cout << "something went wrong :(" << endl;
}
void AddNewClient()
{
    stClient Client;
    Client = ReadNewClient();
    SaveDataClientInFile(FileName, ConvertRecordToLine(Client));
}
void AddClients()
{
    if(!UserPermissionsVerify(enPermissions::eAdd))
    {
        AccessDeniedMessage();
        return;
    }
    
    char AddMore = 'Y';
    do
    {
        system("clear");
        cout << "Please Enter Client Data : \n";
        AddNewClient();
        cout << "\n\nClient Added Successfully :) , Do you want add more (Y/N) ? ";
        cin >> AddMore;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n";
    } while (toupper(AddMore) == 'Y');
}
bool MarkClientForDeleteByAccountNum(vector<stClient> &vClients, string AccountNum)
{
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNum)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
vector<stClient> SaveClientsDataToFile(string FileName, vector<stClient> &vClients)
{
    fstream File;
    File.open(FileName, ios::out);
    if (File.is_open())
    {
        string DataLine;
        for (stClient C : vClients)
        {
            if (!C.MarkForDelete)
            {
                DataLine = ConvertRecordToLine(C);
                File << DataLine << endl;
            }
        }
        File.close();
    }
    else
        cout << "\nFile is not found :( : " << endl;
    return vClients;
}
void PrintLineAsClient(stClient Client)
{
    cout << "\n\nThe following is the extracted client record : \n\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "Pin Code : " << Client.PinCode << endl;
    cout << "Name : " << Client.Name << endl;
    cout << "Phone : " << Client.Phone << endl;
    cout << "Account Balance : " << Client.AccountBalance << endl;
}
void SearchAndPrintClient(stClient &Client, string &AccountNum)
{
    AccountNum = ReadString("Enter Account Number : ");
    Client = FindClientInFile(FileName, AccountNum);
    
    if(!UserPermissionsVerify(enPermissions::eFind))
    {
        AccessDeniedMessage();
        return;
    }
    
    if (Client.IsFound)
    {
        cout << "\nThe Account is found :)\n";
        PrintLineAsClient(Client);
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNum << ") is not found :( \n";
    }
}
bool DeleteClientByAccountNumber(vector<stClient> &vClients)
{
    if(!UserPermissionsVerify(enPermissions::eDelete))
    {
        AccessDeniedMessage();
        return false;
    }
    
    stClient Client;
    string AccountNum;
    SearchAndPrintClient(Client, AccountNum);
    
    if (Client.IsFound)
    {
        char Answer = 'N';
        cout << "\nAre you sure you want to delete this client ? (Y/N) ? ";
        cin >> Answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNum(vClients, AccountNum);
            SaveClientsDataToFile(FileName, vClients);
            vClients = LoadClientsFromFile(FileName);
            cout << "\n\nDelete Client Data is successful :).\n";
            return true;
        }
    }
    return false; 
}
bool UpdateClientByAccountNumber(vector<stClient> &vClients)
{
    if(!UserPermissionsVerify(enPermissions::eUpdate))
    {
        AccessDeniedMessage();
        return false;
    }
    
    stClient Client;
    string AccountNum;
    SearchAndPrintClient(Client, AccountNum);
    
    if (Client.IsFound)
    {
        char Answer = 'N';
        cout << "\nAre you sure you want to Update this client ? (Y/N) ? ";
        cin >> Answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (toupper(Answer) == 'Y')
        {
            for (stClient &C : vClients)
            {
                if (C.AccountNumber == AccountNum)
                {
                    C = ChangeClientRecord(AccountNum);
                }
            }
            SaveClientsDataToFile(FileName, vClients);
            vClients = LoadClientsFromFile(FileName);
            cout << "\n\nUpdate Client Data is successful :).\n";
            return true;
        }
    }
    return false;
}
double ReadAmount(string Message)
{
    double Amount = 0.0;
    cout << Message;
    cin >> Amount;
    while (cin.fail() || Amount < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nWrong Input , ";
        cout << Message;
        cin >> Amount;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return Amount;
}
void DepositAmountInAccount(vector<stClient> &vClients, string AccountNum, double Amount, string KeyWord)
{
    char DepositConfirm = 'N';
    double Amount1 = Amount;
    if (Amount < 0)
        Amount1 = -1 * Amount;

    cout << "\nAre you sure you want to " + KeyWord + " the amount " << Amount1 << " to the account with the number " << AccountNum << " ? (Y/N) ? ";
    cin >> DepositConfirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (toupper(DepositConfirm) == 'Y')
    {
        for (stClient &C : vClients)
        {
            if (C.AccountNumber == AccountNum)
            {
                C.AccountBalance += Amount;
                break;
            }
        }
        SaveClientsDataToFile(FileName, vClients);
        vClients = LoadClientsFromFile(FileName);
        cout << "\n" + KeyWord + " successfully :)\n";
    }
    else
        cout << "\n" << KeyWord << " operation cancelled.\n";
}
void WithdrawAmount(vector<stClient> &vClients, stClient Client, string AccountNum, string KeyWord)
{
    double Amount = ReadAmount("Enter the amount you want to withdraw to the account ? ");
    static int count = 0;
    if (Client.AccountBalance >= Amount)
    {
        count = 0;
        DepositAmountInAccount(vClients, AccountNum, -1 * Amount, KeyWord);
    }
    else
    {
        count++;
        cout << "\nInsufficient balance! Available balance: " << Client.AccountBalance << endl;
        if (count >= 3)
        {
            cout << "\n🔒 Your card is blocked due to too many failed attempts! 🚫\n";
            cout << "   Please contact customer service.\n";
            count = 0;
            return;
        }
        WithdrawAmount(vClients, Client, AccountNum, "Withdraw");
    }
}
void PrintBalancesClient(stClient Client)
{
    cout << "|" << left << setw(16) << Client.AccountNumber;
    cout << "|" << left << setw(19) << Client.Name;
    cout << "|" << left << setw(10) << Client.AccountBalance << "|\n";
}
double CalcTotalBalance(vector<stClient> &vClients)
{
    double TotalBalance = 0.0;
    for (stClient &C : vClients)
    {
        TotalBalance += C.AccountBalance;
    }
    return TotalBalance;
}
void PrintTotalBalance(vector<stClient> &vClients)
{
    cout << "\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "\n_________________________________________________";
    cout << "\n| Account Number |";
    cout << "    Account Name   |";
    cout << "  Balance |";
    cout << "\n|________________|___________________|__________|\n";
    for (stClient &Client : vClients)
    {
        PrintBalancesClient(Client);
    }
    cout << "|________________|___________________|__________|\n";
    cout << "\nTotal Balance = " << CalcTotalBalance(vClients) << endl;
}
void TranscationsProcessing(vector<stClient> &vClients, short Choose)
{
    switch (Choose)
    {
    case 1:
    {
        stClient Client;
        string AccountNum;
        SearchAndPrintClient(Client, AccountNum);
        if (Client.IsFound)
        {
            DepositAmountInAccount(vClients, AccountNum, ReadAmount("Enter the amount you want to add to the account ? "), "Add");
        }
        BackToTheMenu();
    }
    break;
    case 2:
    {
        stClient Client;
        string AccountNum;
        SearchAndPrintClient(Client, AccountNum);
        if (Client.IsFound)
        {
            WithdrawAmount(vClients, Client, AccountNum, "Withdraw");
        }
        BackToTheMenu();
    }
    break;
    case 3:
    {
        PrintTotalBalance(vClients);
        BackToTheMenu();
    }
    break;
    default:
    {
        BackToTheMenu();
    }
    }
}
void ViewTranscationsMinu(vector<stClient> &vClients)
{
    if(!UserPermissionsVerify(enPermissions::eTransactions))
    {
        AccessDeniedMessage();
        BackToTheMenu();
        return;
    }
    
    short Choose = 0;
    while (Choose > 4 || Choose < 1)
    {
        cout << "===================================\n";
        cout << "\tTranscations Menu Screen\n";
        cout << "===================================\n";
        cout << "\t[1] Desposit.\n";
        cout << "\t[2] Withdraw.\n";
        cout << "\t[3] Total Balances\n";
        cout << "\t[4] Main Menu.\n";
        cout << "===================================\n";
        Choose = ReadNumberInRange("Choose what do you want to do ? [1 to 4] ? ", 1, 4);
        system("clear");
        TranscationsProcessing(vClients, Choose);
        if (Choose != 4)
        {
            Choose = 0;
        }
    }
}
stUsers ConvertLineToUserRecord(string S, string Separator = "#//#")
{
    stUsers User;
    vector<string> vString;
    vString = SplitString(S, Separator);
    if (vString.size() >= 3)
    {
        User.UserName = vString.at(0);
        User.PassWord = vString.at(1);
        User.Permissions = stoi(vString.at(2));
    }
    return User;
}
string ConvertUserRecordToLine(stUsers User, string Separator = "#//#")
{
    string sUser = "";
    sUser += User.UserName + Separator;
    sUser += User.PassWord + Separator;
    sUser += to_string(User.Permissions);
    return sUser;
}
vector<stUsers> LoadUsersFromFile(string FileNameUsers)
{
    vector<stUsers> vUsers;
    fstream File;
    File.open(FileNameUsers, ios::in);
    if (!File.is_open())
    {
        File.open(FileNameUsers, ios::out);
        if (File.is_open())
        {
            File.close();
            File.open(FileNameUsers, ios::in);
        }
    }
    if (File.is_open())
    {
        stUsers User;
        string Line = "";
        while (getline(File, Line))
        {
            User = ConvertLineToUserRecord(Line);
            vUsers.push_back(User);
        }
        File.close();
    }
    return vUsers;
}
stUsers FindUserInFile(string FileNameUsers, string UserName)
{
    stUsers User;
    vector<stUsers> vUsers = LoadUsersFromFile(FileNameUsers);
    for (stUsers &U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.IsFoundUser = true;
            User = U;
            break;
        }
    }
    return User;
}
void PrintDataUser(stUsers User)
{
    cout << "|" << left << setw(11) << User.UserName;
    cout << "|" << left << setw(10) << User.PassWord;
    cout << "|" << left << setw(13) << User.Permissions << "|\n";
}
void PrintHeaderTableForUsers(short NumOfUsers)
{
    printf("\t\tUser List (%d) Users(s).\n", NumOfUsers);
    cout << "\n______________________________________";
    cout << "\n| User Name |";
    cout << " PassWord |";
    cout << " Permissions |";
    cout << "\n|___________|__________|_____________|\n";
}
void PrintUsersList(vector<stUsers> &vUsers)
{
    vUsers = LoadUsersFromFile(FileNameUsers);
    PrintHeaderTableForUsers(vUsers.size());
    for (stUsers &User : vUsers)
    {
        PrintDataUser(User);
    }
    cout << "|___________|__________|_____________|\n";
}
stUsers ReadNewUser()
{
    stUsers User,User1;
    User.UserName = ReadString("\nEnter user name : ");
    User.PassWord = ReadString("\nEnter password : ");
    User1 = FindUserInFile(FileNameUsers, User.UserName);
    while (User.IsFoundUser)
    {
        cin.clear();       
        cout << "\nUser with Name (" << User.UserName << ") , User is found :(.\n";
        User.UserName = ReadString("\nEnter user name : ");
        User.PassWord = ReadString("\nEnter password :  ");
        User1 = FindUserInFile(FileNameUsers, User.UserName);
    }
    User.Permissions = ReadPermissions();
    return User;
}
void SaveDataUserInFile(string FileNameUsers, string stDataLine)
{
    fstream File;
    File.open(FileNameUsers, ios::out | ios::app);
    if (File.is_open())
    {
        if (stDataLine != "")
        {
            File << stDataLine << endl;
        }
        File.close();
    }
    else
        cout << "something went wrong :(" << endl;
}
void AddNewUser()
{
    stUsers User;
    User = ReadNewUser(); SaveDataUserInFile(FileNameUsers, ConvertUserRecordToLine(User));
}
void AddUsers()
{
    if(!UserPermissionsVerify(enPermissions::eAdd))
    {
        AccessDeniedMessage();
        BackToTheMenu();
        return;
    }
    
    char AddMore = 'Y';
    do
    {
        system("clear");
        cout << "===================================\n";
        cout << "\tAdd New User Screen\n";
        cout << "===================================\n";
        cout << "Please Enter User Data : \n";
        AddNewUser();
        cout << "\n\nUser Added Successfully :) , Do you want add more (Y/N) ? ";
        cin >> AddMore;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n";
    } while (toupper(AddMore) == 'Y');
}
bool MarkUserForDeleteByUserName(vector<stUsers> &vUsers, string UserName)
{
    for (stUsers &U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.MarkUserForDelete = true;
            return true;
        }
    }
    return false;
}
vector<stUsers> SaveUsersDataToFile(string FileNameUsers, vector<stUsers> &vUsers)
{
    fstream File;
    File.open(FileNameUsers, ios::out);
    if (File.is_open())
    {
        string DataLine;
        for (stUsers U : vUsers)
        {
            if (!U.MarkUserForDelete)
            {
                DataLine = ConvertUserRecordToLine(U);
                File << DataLine << endl;
            }
        }
        File.close();
    }
    else
        cout << "\nFile is not found :( : " << endl;
    return vUsers;
}
void PrintOrderUserInfo(stUsers User)
{
    cout << "\n\nThe following is the extracted user info : \n\n";
    cout << "User Name : " << User.UserName << endl;
    cout << "PassWord : " << User.PassWord << endl;
    cout << "Permissions : " << User.Permissions << endl;
}
void PrintUserInfo(stUsers User)
{   
    if (User.IsFoundUser)
    {
        cout << "\nThe user is found :)\n";
        PrintOrderUserInfo(User);
    }
    else
    {
        cout << "\nthe user is not found :(\n";
    }
}
bool DeleteUserByUserName(vector<stUsers> &vUsers)
{
    if(!UserPermissionsVerify(enPermissions::eDelete))
    {
        AccessDeniedMessage();
        return false;
    }
    
    cout << "===================================\n";
    cout << "\tDelete User\n";
    cout << "===================================\n";
    string UserName;
        UserName = ReadString("Enter the name of user ? ");
    stUsers User = FindUserInFile(FileNameUsers, UserName);
    PrintUserInfo(User);
    if (User.IsFoundUser)
    {
        char Answer = 'N';
        cout << "\nAre you sure you want to delete this User ? (Y/N) ? ";
        cin >> Answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (toupper(Answer) == 'Y')
        {
            MarkUserForDeleteByUserName(vUsers, User.UserName);
            SaveUsersDataToFile(FileNameUsers, vUsers);
            vUsers = LoadUsersFromFile(FileNameUsers);
            cout << "\n\nDelete User Data is successful :).\n";
            return true;
        }
    }
    return false;
}
bool UpdateUserByUserName(vector<stUsers> &vUsers)
{
    if(!UserPermissionsVerify(enPermissions::eUpdate))
    {
        AccessDeniedMessage();
        return false;
    }
    
    cout << "===================================\n";
    cout << "\tUpdate User\n";
    cout << "===================================\n";
    string UserName;
        UserName = ReadString("Enter the name of user ? ");   
    stUsers User = FindUserInFile(FileNameUsers, UserName);
    PrintUserInfo(User);
    if (User.IsFoundUser)
    {
        char Answer = 'N';
        cout << "\nAre you sure you want to Update this User ? (Y/N) ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (stUsers &U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U.PassWord = ReadString("\nEnter new password : ");
                    U.Permissions = ReadPermissions();
                    
                }
            }            SaveUsersDataToFile(FileNameUsers, vUsers);
            vUsers = LoadUsersFromFile(FileNameUsers);
            cout << "\n\nUpdate User Data is successful :).\n";
            return true;
        }
    }
    return false;
}
void ManageUsersProcessing(vector<stUsers> &vUsers, short Choose)
{
    switch (Choose)
    {
    case 1:
    {
        PrintUsersList(vUsers);
        BackToTheMenu();
    }
    break;
    case 2:
    {
        AddUsers();
        BackToTheMenu();
    }
    break;
    case 3:
    {        DeleteUserByUserName(vUsers);
        BackToTheMenu();
    }
    break;
    case 4:
    {
        UpdateUserByUserName(vUsers);
        BackToTheMenu();
    }
    break;
    case 5:
    {
        cout << "\n-----------------------------------\n";
    cout << "\tFind User";
    cout << "\n-----------------------------------\n";
    stUsers User;
    User.UserName = ReadString("Enter User Name : ");
        User = FindUserInFile(FileNameUsers, User.UserName);
        PrintUserInfo(User);
        BackToTheMenu();
    }
    break;
    default:
    {
        BackToTheMenu();
    }
    }
}
void ViewManageUsersMenu(vector<stUsers> &vUsers)
{
    if(!UserPermissionsVerify(enPermissions::eManage))
    {
        AccessDeniedMessage();
        BackToTheMenu();
        return;
    }
    
    short Choose = 0;
    while (Choose > 6 || Choose < 1)
    {
        cout << "===================================\n";
        cout << "\tManage Users Menu\n";
        cout << "===================================\n";
        cout << "\t[1] List Users.\n";
        cout << "\t[2] Add New User.\n";
        cout << "\t[3] Delete User\n";
        cout << "\t[4] Update User Info.\n";
        cout << "\t[5] Find User.\n";
        cout << "\t[6] Main Menu.\n";
        cout << "===================================\n";
        Choose = ReadNumberInRange("Choose what do you want to do ? [1 to 6] ? ", 1, 6);
        system("clear");
        ManageUsersProcessing(vUsers, Choose);
        if (Choose != 6)
        {
            Choose = 0;
        }
    }
}
void ChooseProcessing(vector<stClient> &vClients, vector<stUsers> &vUsers, short Choose)
{
    switch (Choose)
    {
    case 1:
    {
        PrintClientsList(vClients);
        BackToTheMenu();
    }
    break;
    case 2:
    {
        AddClients();
        BackToTheMenu();
    }
    break;
    case 3:
    {
        DeleteClientByAccountNumber(vClients);
        BackToTheMenu();
    }
    break;
    case 4:
    {
        UpdateClientByAccountNumber(vClients);
        BackToTheMenu();
    }
    break;
    case 5:
    {
        stClient Client;
        string AccountNum;
        SearchAndPrintClient(Client, AccountNum);
        BackToTheMenu();
    }
    break;
    case 6:
    {
        ViewTranscationsMinu(vClients);
    }
    break;
    case 7:
    {
        ViewManageUsersMenu(vUsers);
    }
    break;
    default:
    {
        system("clear");
        Login(vUsers, vClients);
    }
    }
}
void ViewMainMenu(vector<stClient> &vClients, vector<stUsers> &vUsers)
{
    short Choose = 0;
    while (Choose > 8 || Choose < 1)
    {
        cout << "===================================\n";
        cout << "\tMain Menu Screen\n";
        cout << "===================================\n";
        cout << "\t[1] Show Client List.\n";
        cout << "\t[2] Add New Client.\n";
        cout << "\t[3] Delete Client\n";
        cout << "\t[4] Update Client Info.\n";
        cout << "\t[5] Find Client.\n";
        cout << "\t[6] Transactions.\n";
        cout << "\t[7] Manage Users.\n";
        cout << "\t[8] Logout.\n";
        cout << "===================================\n";
        Choose = ReadNumberInRange("Choose what do you want to do ? [1 to 8] ? ", 1, 8);
        system("clear");
        ChooseProcessing(vClients, vUsers, Choose);
        if (Choose != 8)
        {
            Choose = 0;
        }
    }
}
bool LoginScreen(vector<stUsers> &vUsers)
{
    stUsers User;
    string PassWord;
    cout << "===================================\n";
    cout << "\tLogin Screen\n";
    cout << "===================================\n";
    cout << "\nEnter a user name : ";
    getline(cin >> ws, User.UserName);
    cout << "\nEnter a password : ";
    getline(cin, PassWord);
    User = FindUserInFile(FileNameUsers, User.UserName);
    while (!User.IsFoundUser || PassWord != User.PassWord)
    {
        cin.clear();
        cout << "\nUser is not found , Invalid UserName/Password" << endl;
        cout << "\nEnter a user name : ";
        getline(cin >> ws, User.UserName);
        cout << "\nEnter a password : ";
        getline(cin, PassWord);
        User = FindUserInFile(FileNameUsers, User.UserName);
    }
    CurrentUser = User;
    return true;
}
void Login(vector<stUsers> &vUsers, vector<stClient> &vClients)
{
    LoginScreen(vUsers);
    system("clear");
    ViewMainMenu(vClients, vUsers);
}
int main()
{
    vector<stClient> vClients = LoadClientsFromFile(FileName);
    vector<stUsers> vUsers = LoadUsersFromFile(FileNameUsers);
    Login(vUsers, vClients);
    return 0;
}