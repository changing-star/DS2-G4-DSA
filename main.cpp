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
void UserMode();
void AdminMode();


int main(){
    string inputID;
    string inputPassword;

    adminList.loadAdminFromFile("database/admin.csv");

    cout << adminList.getSize() << endl;

    return 0;
}

void UserMode(){

};

void AdminMode(){
    
};
