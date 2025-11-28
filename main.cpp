#include "utils/admin.hpp"
#include "utils/book.hpp"
#include "utils/hash.hpp"
#include "utils/shared.hpp"
#include "utils/transaction.hpp"
#include "utils/user.hpp"

AdminList adminList;
UserList userList;
BookList bookList;
TransactionList transactionList;

using namespace std;
void UserView();
void AdminView();


int main(){
    string inputID;
    string inputPassword;

    adminList.loadAdminFromFile("database/admin.csv");
    userList.loadUserFromFile("database/user.csv");
    bookList.loadBooksFromFile("database/books.csv");
    transactionList.loadTransactionFromFile("database/transactions.csv");

    while(true){
        clearScreen();
        cout << "----- Login Screen -----" << endl;
        cout << "Input ID: ";
        cin >> inputID;
        cout << "Input Password: ";
        cin >> inputPassword;

        for(size_t i = 0; i < inputID.length(); i++){
            inputID[i] = toupper(inputID[i]); // Normalize
        }

        if(inputID.substr(0,5) == "USER-"){
            if(userList.searchAndCompare(inputID, inputPassword)){
                cout << "User login success" << endl;
                UserView();
                break;
            } else {
                cout << "User login failed" << endl;
            }
        } else if(inputID.substr(0,3) == "AD-"){
            if(adminList.searchAndCompare(inputID, inputPassword)){
                cout << "Admin login success" << endl;
                AdminView();
                break;
            } else {
                cout << "Admin login failed" << endl;
            }
        } else {
            cout << "INVALID ID! Please try again" << endl;
        }

        cout << "Press Enter to continue..." << endl;
        cin.ignore();
        cin.get();

    }

    return 0;
}

void UserView(){
    int choice;
    while(1){
        clearScreen();
        cout << "----- USER VIEW -----";
        cout << "1. View all Books" << endl;
        cout << "2. Search book by title" << endl;
        cout << "3. Search book by author" << endl;
        cout << "4. Search book by ID" << endl;
        cout << "0. Exit" << endl;

        cout << "Enter Option: ";
        cin >> choice;

        switch(choice){
            case 1: { //View All Books

                break;
            }
            case 2: { //Search book by title
                string searchInput;
                cout << "Enter a title: ";
                cin.ignore();
                getline(cin, searchInput);
                
                break;
            }
        }
    }

};

void AdminView(){
    
};
