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
                bookList.transformList();
                break;
            }
            case 2: { //Search book by title
                string searchInput;
                cout << "Enter a title: ";
                cin.ignore();
                getline(cin, searchInput);
                bookList.searchByTitle(searchInput);
                break;
            }
            case 3: {
                string searchInput;
                cout << "Enter author name: ";
                cin.ignore();
                getline(cin, searchInput);
                bookList.searchByAuthor(searchInput);
                break;
            }
            case 0: {
                cout << "Exiting..." << endl;
                return;
                break;
            }
            default : {
                cout << "INVALID CHOICE! PLease try again..." << endl;
                break;
            }
        }
    }
};

void AdminView(){
    int choice;
    while(1){
        clearScreen();
        cout << "----- ADMIN PORTAL -----" << endl;
        cout << "1. Permit a borrow" << endl;
        cout << "2. Accept return" << endl;
        cout << "3. View users" << endl;
        cout << "4. Add users" << endl;
        cout << "5. Edit users" << endl;
        cout << "6. Remove users" << endl;
        cout << "7. View books" << endl;
        cout << "8. Add books" << endl;
        cout << "9. Edit books" << endl;
        cout << "10. Remove books" << endl;
        cout << "11. View transacions" << endl;
        cout << "0. Exit" << endl;

        cout << "Enter option: ";
        cin >> choice;
        
        switch(choice){
            case 1:{ // Borrow book
                string transactionID, inputItemID, inputUserID, relatedTransactionIDField, relatedTransactionID;
                TransactionData transactionData;
                TransactionData topTransactionData;

                transactionData.type = TransactionType::BORROW;

                cout << "Enter user ID: ";
                if(inputUserID.substr(0,5) == "USER-"){
                    cin >> inputUserID;
                    transactionData.userID = inputUserID;
                } else {
                    cout << "Invalid User ID";
                    return;
                    break;
                }

                cout << "Enter item ID: ";
                if(inputItemID.substr(0,4) == "BID-"){
                    cin >> inputItemID;
                    transactionData.itemID = inputinputItemIDUserID;
                } else {
                    cout << "Invalid Item ID";
                    return;
                    break;
                }

                cout << "Enter Related Transaction ID";
                cin >> relatedTransactionIDField;

                if(relatedTransactionIDField.substr(0,3) == "TR-" && transactionList.searchAndCompare(relatedTransactionIDField)){
                    relatedTransactionID = relatedTransactionIDField;
                    transactionData.relatedTransaction = relatedTransactionID;
                } else {
                    cout << "Invalid Related Transaction";
                    relatedTransactionID = "NULL";
                }

                topTransactionData = transactionList.peek();
                string currentTransactionID = topTransactionData.transactionID;
                string numberPart = currentTransactionID.substr(3);
                int number = stoi(numberPart);
                number++;
                string newNumber = to_string(number);
                while (newNumber.length() < 3) {
                    newNumber = "0" + newNumber;
                }
                transactionID = "TR-" + newNumber;
                transactionData.transactionID = transactionID;
                transactionData.transactionTime = to_string(time(NULL));        

                if(bookList.updateBorrow(inputItemID)){
                    bookList.saveBooksToFile("database/books.csv");
                    transactionList.addTransaction(transactionData);
                    transactionList.saveTransactionToFile("database/transactions.hpp");
                } else {
                    cout << "Borrow failed" << endl;
                }

                cin.ignore();
                cin.get();
                break;
            }
            case 2: { //Accept return
                string inputItemID, inputUserID, relatedTransactionID;
                TransactionData transactionData;
                transactionData.type = TransactionType::RETURN;

                cout << "Enter user ID: ";
                if(inputUserID.substr(0,5) == "USER-"){
                    cin >> inputUserID;
                    transactionData.userID = inputUserID;
                } else {
                    cout << "Invalid User ID";
                    return;
                    break;
                }

                cout << "Enter item ID: ";
                if(inputItemID.substr(0,4) == "BID-"){
                    cin >> inputItemID;
                    transactionData.itemID = inputinputItemIDUserID;
                } else {
                    cout << "Invalid Item ID";
                    return;
                    break;
                }

                bool foundBorrowTransaction = false;
                TransactionNode* curr = top;

                while (curr != nullptr) {
                    if (curr->transactionData.type == TransactionType::BORROW &&
                        curr->transactionData.itemID == inputItemID &&
                        curr->transactionData.userID == inputUserID) {
                        
                        if (curr->transactionData.relatedTransaction == "" || 
                            curr->transactionData.relatedTransaction == "NULL") {
                            
                            foundBorrowTransaction = true;
                            relatedTransactionID = curr->transactionData.transactionID;
                            break;
                        }
                    }
                    current = curr->prev; // Move down the stack (older transactions)
                }

                if (!foundBorrowTransaction) {
                    cout << "No active borrow transaction found for this user and item." << endl;
                    cin.ignore();
                    cin.get();
                    break;
                }

                string currentTransactionID = "";
                if (!isEmpty()) {
                    currentTransactionID = top->transactionData.transactionID;
                }

                string transactionID;
                string numberStr = currentTransactionID.substr(3);
                int number = stoi(numberStr) + 1;
                stringstream ss;
                ss << "TR-" << setw(3) << setfill('0') << number;
                transactionID = ss.str();

                transactionData.transactionID = transactionID;
                transactionData.relatedTransaction = relatedTransactionID;

                transactionData.transactionTime = to_string(time(NULL));        

                if(bookList.updateReturn(inputItemID)){
                    bookList.saveBooksToFile("database/books.csv");
                    transactionList.addTransaction(transactionData);
                    transactionList.saveTransactionToFile("database/transactions.hpp");
                } else {
                    cout << "Return failed" << endl;
                }
            }
            case 3: { //
                string
            }
        }

    }
};
