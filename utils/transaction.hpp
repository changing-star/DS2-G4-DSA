#ifndef transaction_hpp
#define transaction_hpp

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
    private:
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
    
    TransactionData peek(){
        TransactionData transactionData;
        if (isEmpty()) return transactionData;
        transactionData = top->transactionData;
        return transactionData;
    }

    void saveTransactionToFile(string filename){
        ofstream file(filename);
        if(!file.is_open()){
            cerr << "Error opening file " << filename << endl;
            return;
        }

        file << "index,transaction_id,user_id,book_id,transaction_type,transaction_time,related_transaction" << 'endl';

        int index = 1;

        while(!isEmpty()){
            file << index << ','
                << top->transactionData.transactionID << ','
                << top->transactionData.userID << ','
                << top->transactionData.itemID << ','
                << transactionTypeToString(top->transactionData.type) << ','
                << top->transactionData.transactionTime << ','
                << top->transactionData.relatedTransaction << endl;
            index++;
            pop();
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
        string transactionDataType;
        TransactionData transactionData;
        
        while(getline(file, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            stringstream ss(line);
            
            getline(ss, transactionData.transactionID, ',');
            getline(ss, transactionData.userID, ',');
            getline(ss, transactionData.itemID, ',');
            getline(ss, transactionDataType, ',');
            getline(ss, transactionData.transactionTime, ',');
            getline(ss, transactionData.relatedTransaction, ',');

            transactionData.type = stringToTransactionType(transactionDataType);
        }
        
    }
    
};

#endif