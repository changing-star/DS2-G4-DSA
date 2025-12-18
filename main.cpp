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

    cout << "cuh" << endl;

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
        cout << "----- USER VIEW -----" << endl;
        cout << "1. View all Books" << endl;
        cout << "2. Search book by title" << endl;
        cout << "3. Search book by author" << endl;
        // cout << "4. Search book by ID" << endl;
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
            case 4: {
                string searchInput;
                cout << "Enter input ID: ";
                cin.ignore();
                getline(cin, searchInput);
                bookList.searchByID(searchInput);
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
        cout << "4. Add user" << endl;
        cout << "5. Edit user" << endl;
        cout << "6. Remove user" << endl;
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

                transactionData.type = TransactionType::BORROW;

                cout << "Enter user ID: ";
                cin >> inputUserID;
                if(normalizeID(inputUserID).substr(0,5) == "USER-"){
                    transactionData.userID = inputUserID;
                } else {
                    cout << "Invalid User ID";
                    cin.ignore();
                    cin.get();
                    break;
                }

                cout << "Enter item ID: ";
                cin >> inputItemID;
                if(normalizeID(inputItemID).substr(0,4) == "BID-"){
                    transactionData.itemID = inputItemID;
                } else {
                    cout << "Invalid Item ID";
                    cin.ignore();
                    cin.get();
                    break;
                }

                // Check if transaction list is empty to avoid null pointer dereference
                if (!transactionList.isEmpty()) {
                    TransactionNode* topNode = transactionList.peek();
                    string currentTransactionID = topNode->transactionData.transactionID;
                    string numberPart = currentTransactionID.substr(3);
                    int number = stoi(numberPart);
                    number++;
                    stringstream ss;
                    ss << "TR-" << setw(3) << setfill('0') << number;
                    transactionID = ss.str();
                } else {
                    // First transaction
                    transactionID = "TR-001";
                }
                
                transactionData.transactionID = transactionID;
                transactionData.transactionTime = to_string(time(NULL));
                transactionData.relatedTransaction = "";

                if(bookList.updateBorrow(inputItemID)){
                    bookList.saveBooksToFile("database/books.csv");
                    transactionList.addTransaction(transactionData);
                    transactionList.saveTransactionToFile("database/transactions.csv");
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
                TransactionNode* top = transactionList.peek();

                transactionData.type = TransactionType::RETURN;

                cout << "Enter user ID: ";
                cin >> inputUserID;
                if(normalizeID(inputUserID).substr(0,5) == "USER-"){
                    transactionData.userID = inputUserID;
                } else {
                    cout << "Invalid User ID";
                    return;
                    break;
                }

                cout << "Enter item ID: ";
                cin >> inputItemID;
                if(normalizeID(inputItemID).substr(0,4) == "BID-"){
                    transactionData.itemID = inputItemID;
                } else {
                    cout << "Invalid Item ID";
                    return;
                    break;
                }

                bool foundBorrowTransaction = false;
                TransactionNode* curr = transactionList.peek();

                while (curr != nullptr) {
                    if (curr->transactionData.type == TransactionType::BORROW &&
                        normalizeID(curr->transactionData.itemID) == normalizeID(inputItemID) &&
                        normalizeID(curr->transactionData.userID) == normalizeID(inputUserID)) {
                        
                        if (curr->transactionData.relatedTransaction == "" || 
                            curr->transactionData.relatedTransaction == "NULL") {
                            
                            foundBorrowTransaction = true;
                            relatedTransactionID = curr->transactionData.transactionID;
                            break;
                        }
                    }
                    curr = curr->prev; // Move down the stack (older transactions)
                }

                if (!foundBorrowTransaction) {
                    cout << "No active borrow transaction found for this user and item." << endl;
                    cin.ignore();
                    cin.get();
                    break;
                }

                string currentTransactionID = "";
                if (!transactionList.isEmpty()) {
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
                    transactionList.saveTransactionToFile("database/transactions.csv");
                } else {
                    cout << "Return failed" << endl;
                }
                break;
            }
            case 3: { //View users
                userList.displayUsers();
                break;
            }
            case 4: { //Add users
                string inputPassword;
                UserData userData;
                cout << "Enter a new user name: ";
                cin.ignore();
                getline(cin, userData.username);
                cout << "Enter a new user password: ";
                getline(cin, inputPassword);

                userData.userPassword = md5Hash(inputPassword);
                inputPassword = "";

                userData.createdAt = to_string(time(NULL));        

                UserNode* tailNode = userList.getTail();

                string currentUserID = tailNode->userData.userID;

                string userID;
                string numberStr = currentUserID.substr(5);
                int number = stoi(numberStr) + 1;
                stringstream ss;
                ss << "USER-" << setw(5) << setfill('0') << number;
                userID = ss.str();

                userData.userID = userID;
                
                userList.insertBack(userData);
                userList.saveUserToFile("database/user.csv");
                
                cout << "User " << userData.userID << " added successfully!" << endl;
                cin.get();
                break;
            }
            case 5:{ // Edit User
                string inputID;
                cout << "Enter a user ID to edit: ";
                cin >> inputID;
                userList.edit(inputID);
                userList.saveUserToFile("database/user.csv");
                break;
            }
            case 6:{ //Remove User
                string inputID;
                cout << "Enter a user ID to remove: ";
                cin >> inputID;
                userList.removeByID(inputID);
                userList.saveUserToFile("database/user.csv");
                break;
            }
            case 7: { //View Books
                bookList.transformList();
                break;
            }
            case 8: { //Add Book
                BookData bookData;
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, bookData.bookTitle);
                cout << "Enter book author name: ";
                getline(cin, bookData.bookAuthor);
                cout << "Enter book ISBN: ";
                getline(cin, bookData.isbn);

                cout << "Enter total copies: ";
                string totalCopiesField;
                getline(cin, totalCopiesField);

                while(stoi(totalCopiesField) <= 0){
                    cout << "Total copies must be at least 1. Enter again:" ;
                    cin >> totalCopiesField;
                }

                bookData.totalCopies = stoi(totalCopiesField);

                bookData.availableCopies = bookData.totalCopies;

                bookList.add(&bookData);
                bookList.saveBooksToFile("database/books.csv");
                break;
            }
            case 9: { //Edit book
                string inputID;
                cout << "Enter book ID to edit: ";
                cin >> inputID;
                bookList.edit(inputID);
                bookList.saveBooksToFile("database/books.csv");
                break;
            }
            case 10: { //Remove book
                string inputID;
                cout << "Enter book ID to remove: ";
                cin >> inputID;
                bookList.removeByID(inputID);
                bookList.saveBooksToFile("database/books.csv");
                break;                
            }
            case 11: { //View Transaction List
                transactionList.display();
                break;
            }
            case 0: {
                cout << "Exiting..." << endl;
                return;
            }
            default: {
                cout << "Invalid Choice! Please try again..." << endl;
                break;
            }
        }
    }
};
