#ifndef transaction_hpp
#define transaction_hpp
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <iomanip>
using namespace std;

enum TransactionType{
    BORROW,
    RETURN
};

string transactionTypeToString(TransactionType value){
    static const map<TransactionType, string> enumMap = {
        {TransactionType::BORROW, "BORROW"},
        {TransactionType::RETURN, "RETURN"},
    };

    auto itr = enumMap.find(value);
    if(itr != enumMap.end()){
        return itr->second;
    }
    return "UNKNOWN";
}

TransactionType stringToTransactionType(string transactionType){
    static unordered_map<string, TransactionType> const table = {{"BORROW", TransactionType::BORROW}, {"RETURN", TransactionType::RETURN}};
    auto itr = table.find(transactionType);
    if(itr != table.end()){
        return itr->second;
    }
}

struct TransactionData{
    string transactionID;
    string userID;

    string itemID;
    TransactionType type;
    string transactionTime;
    string relatedTransaction;
};

//DLL
class TransactionNode{
    public:
        TransactionData transactionData;

        TransactionNode *prev;
        TransactionNode *next;

        TransactionNode(TransactionData transactionData){
            this->transactionData.transactionID = transactionData.transactionID;
            this->transactionData.userID = transactionData.userID;
            this->transactionData.itemID = transactionData.itemID;
            this->transactionData.type = transactionData.type;

            prev = next = nullptr;
        }

    friend class TransactionList;
};

//Stack
class TransactionList{
    private:
        TransactionNode* top;
        TransactionNode* bottom;
    
    public:
        TransactionList(){
            bottom = top = nullptr;
        }
    
    void push(TransactionData transactionData){
        TransactionNode* newNode = new TransactionNode(transactionData);

        if(isEmpty()){
            bottom = top = newNode;
        } else {
            top->next = newNode;
            newNode->next = nullptr;
            newNode->prev = top;
            top = newNode;
        }
    }

    void pop(){
        TransactionNode* node = top;
        if(isEmpty()){
            cout << "Stack is empty";
        } else if (top == bottom){
            top = bottom = nullptr;
            free(node);
        } else {
            top->prev->next = nullptr;
            top = node->prev;
            free(node);
        }
    }

    bool isEmpty(){
        if(bottom==nullptr) return true;
        return false;
    }

    int getSize(){
        int c = 0;
        if (isEmpty()){
            return c;
        } else {
            TransactionNode* ptr = bottom;
            while(bottom != nullptr){
                c++;
                bottom = bottom->next;
            }
            return c;
        }
    }
    
    TransactionNode* peek(){
        return top;
    }

    void addTransaction(TransactionData transactionData){
        push(transactionData);
        cout << "User ID: "<< transactionData.userID << " "<< transactionData.type << " Item ID: " << transactionData.itemID << " Transaction ID: "<<transactionData.transactionID << endl;
    }

    void saveTransactionToFile(string filename){
        ofstream file(filename);
        if(!file.is_open()){
            cerr << "Error opening file " << filename << endl;
            return;
        }

        file << "index,transaction_id,user_id,book_id,transaction_type,transaction_time,related_transaction" << 'endl';

        int index = 1;
        TransactionNode* curr = top;
        while(top != nullptr){
            file << index << ','
                << top->transactionData.transactionID << ','
                << top->transactionData.userID << ','
                << top->transactionData.itemID << ','
                << transactionTypeToString(top->transactionData.type) << ','
                << top->transactionData.transactionTime << ','
                << top->transactionData.relatedTransaction << endl;
            index++;
            top = top->prev;
        }
    }

    void loadTransactionFromFile(string filename){
        bool firstLine = true;
        ifstream file(filename);
        if(!file.is_open()){
            cerr << "Unable to open file: " << filename << endl;
            return;
        }

        string line;
        while(getline(file, line)){
            // Skip header and blank/whitespace lines
            if(firstLine){
                firstLine = false;
                continue;
            }
            if(line.empty() || all_of(line.begin(), line.end(), ::isspace)){
                continue;
            }

            stringstream ss(line);
            string field;
            int index;
            TransactionData transactionData;
            string transactionDataType;

            // Parse index field
            getline(ss, field, ',');
            try{
                index = stoi(field);
            } catch(const exception& e){
                cerr << "Error converting index: " << e.what() << endl;
                continue;
            }

            if(getline(ss, transactionData.transactionID, ',') &&
               getline(ss, transactionData.userID, ',') &&
               getline(ss, transactionData.itemID, ',') &&
               getline(ss, transactionDataType, ',') &&
               getline(ss, transactionData.transactionTime, ',') &&
               getline(ss, transactionData.relatedTransaction, ',')){
                transactionData.type = stringToTransactionType(transactionDataType);
                push(transactionData);
            }
        }
        file.close();
    }

    bool searchAndCompare(string transactionID){
        TransactionNode* curr = top;
        while(curr != nullptr){
            if(curr->transactionData.transactionID == transactionID){
                return true;
            }
            curr = curr->prev;
        }
        return false;
    }

    void display() {
        if (isEmpty()) {
            cout << "No transactions found." << endl;
            return;
        }

        vector<TransactionData> transactions = toVector();
        
        int itemPerPage = 10;
        cout << "Enter number of items per page (default 10): ";
        string input;
        getline(cin, input);
        if (!input.empty()) {
            try {
                int val = stoi(input);
                if (val > 0) itemPerPage = val;
                else cout << "Invalid input, using default 10." << endl;
            } catch (...) {
                cout << "Invalid input, using default 10." << endl;
            }
        }

        int totalItem = transactions.size();
        int totalPage = (totalItem + itemPerPage - 1) / itemPerPage;
        int currentPage = 1;
        string navKey;

        while (true) {
            clearScreen();
            cout << "===== TRANSACTION HISTORY =====" << endl;
            cout << "Page " << currentPage << " of " << totalPage << endl;
            cout << "Total transactions: " << totalItem << endl;
            
            // Count borrow vs return
            int borrowCount = 0, returnCount = 0;
            for (const auto& trans : transactions) {
                if (trans.type == TransactionType::BORROW) borrowCount++;
                else if (trans.type == TransactionType::RETURN) returnCount++;
            }
            cout << "Borrow operations: " << borrowCount << endl;
            cout << "Return operations: " << returnCount << endl;
            cout << endl;

            // Display header
            cout << setw(5) << left << "No."
                 << setw(15) << left << "Transaction ID"
                 << setw(10) << left << "User ID"
                 << setw(10) << left << "Book ID"
                 << setw(10) << left << "Type"
                 << setw(20) << left << "Time" << endl;
            cout << string(80, '-') << endl;

            // Display current page
            int start = (currentPage - 1) * itemPerPage;
            int end = min(start + itemPerPage, totalItem);
            
            for (int i = start; i < end; i++) {
                const auto& trans = transactions[i];
                cout << setw(5) << left << (i + 1)
                     << setw(15) << left << trans.transactionID
                     << setw(10) << left << trans.userID
                     << setw(10) << left << trans.itemID
                     << setw(10) << left << transactionTypeToString(trans.type)
                     << setw(20) << left << trans.transactionTime << endl;
            }

            cout << endl;
            cout << "Navigation: previous page '<-', next page '->', exit 'q'" << endl;
            
            navKey = readNav();  // Assuming you have this function
            if (navKey == "left" && currentPage > 1) currentPage--;
            else if (navKey == "right" && currentPage < totalPage) currentPage++;
            else if (navKey == "exit") break;
        }
    }

    // Helper to convert stack to vector (most recent first)
    vector<TransactionData> toVector() {
        vector<TransactionData> result;
        TransactionNode* curr = top;
        while (curr != nullptr) {
            result.push_back(curr->transactionData);
            curr = curr->prev;
        }
        return result;
    }
};


#endif