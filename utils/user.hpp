#ifndef user_hpp
#define user_hpp
#include "shared.hpp"
#include "hash.hpp"

struct UserData{
    string userID;
    string username;
    string userPassword;
    string createdAt;
};

class UserNode{
    public:
        UserData userData;
        UserNode* prev;
        UserNode* next;

        UserNode(UserData userData){
            this->userData.userID = userData.userID;
            this->userData.username = userData.username;
            this->userData.userPassword = userData.userPassword;
            this->userData.createdAt = userData.createdAt;
            prev = next = nullptr;
        }

    friend class UserList;
};

class UserList{
    private:
        UserNode* head;
        UserNode* tail;
        int size;
    public:
        UserList(){
            head = tail = nullptr;
            size = 0;
        }
        
        int getSize(){
            return size;
        }

        UserNode* getHead(){
            return head;
        }
        UserNode* getTail(){
            return tail;
        }

        bool isEmpty(){
            return head==nullptr;
        }

        void insertFront(UserData userData){
            UserNode* newNode = new UserNode(userData);

            if(head == nullptr){
                head = tail = newNode;
            } else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            size++;
        }

        void insertBack(UserData userData){
            UserNode* newNode = new UserNode(userData);
            if(head == nullptr){
                head = tail = newNode;
            } else {
                newNode->prev = tail;
                tail->next = newNode;
                tail = newNode;
            };
            size++;
        }

        void displayUsers(){
            if(head == nullptr){
                clearScreen();
                cout << "No users available." << endl;
                cout << "Press any key to return..." << endl;
                cin.ignore();
                cin.get();
                return;
            }
            
            // Collect all users into a vector for pagination
            vector<UserData> allUsers;
            UserNode* curr = head;
            while(curr != nullptr){
                allUsers.push_back(curr->userData);
                curr = curr->next;
            }
            
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
            
            int totalItem = allUsers.size();
            int totalPage = (totalItem + itemPerPage - 1) / itemPerPage;
            int currentPage = 1;
            string navKey;
            
            while (true) {
                clearScreen();
                cout << "===== View all users =====" << endl;
                cout << "Page " << currentPage << " of " << totalPage << endl;
                cout << "Total users: " << totalItem << endl;
                cout << endl;
                
                int start = (currentPage - 1) * itemPerPage;
                int end = min(start + itemPerPage, totalItem);
                
                // Display header
                cout << left << setw(6) << "No."
                     << setw(15) << "User ID"
                     << setw(20) << "Username"
                     << setw(40) << "Password (hashed)" << endl;
                cout << string(80, '-') << endl;
                
                // Display current page users
                for(int i = start; i < end; i++){
                    cout << left << setw(6) << (i + 1)
                         << setw(15) << allUsers[i].userID
                         << setw(20) << allUsers[i].username
                         << setw(40) << allUsers[i].userPassword
                         << endl;
                }
                
                cout << endl;
                cout << "*********************************************************" << endl;
                cout << "Navigation: previous page '<-', next page '->', exit 'q'" << endl;
                navKey = readNav();
                if (navKey == "left" && currentPage > 1) currentPage--;
                else if (navKey == "right" && currentPage < totalPage) currentPage++;
                else if (navKey == "exit") break;
            }
        }

        void edit(string inputID){
            if(isEmpty()){
                cout << "User list is empty!" << endl;
                return;
            }

            UserNode* curr = head;
            while(curr != nullptr){
                if(normalizeID(curr->userData.userID) == normalizeID(inputID)){
                    string passwordField;
                    cout << "User found. Enter new Info: " << endl;
                    cout << "Enter new Username: ";
                    cin.ignore();
                    getline(cin, curr->userData.username);
                    cout << "Enter new User Password: ";
                    getline(cin, passwordField);
                    curr->userData.userPassword = md5Hash(passwordField);

                    cout << "User info updated success" << endl;
                    return;
                }
                curr = curr->next;
            }
            cout << "User with ID " << inputID << " not found." << endl;
        }

        void removeFront(){
            if(isEmpty()){
                cout << "User list is empty!";
                return;
            }
            UserNode* t = head;
            if (head == tail) {
                head = tail = nullptr;
            } else {
                head = head->next;
                head->prev = nullptr;
            }
            delete t;
            size--;
        }
        void removeBack(){
            if(isEmpty()){
                cout << "User list is empty!";
                return;
            }
            UserNode* t = tail;
            if (head == tail) {
                head = tail = nullptr;
            } else {
                tail = tail->prev;
                tail->next = nullptr;
            }
            delete t;
            size--;
        }

        void removeByID(string inputID){
            if (isEmpty()){
                cout << "User list is empty!" << endl;
                return;
            }

            UserNode* curr = head;
            while(curr != nullptr && curr->next->userData.userID != inputID){
                curr = curr->next;
            }

            if (curr == nullptr) {
                cout << "User with ID " << inputID << " not found." << endl;
                return;
            }
            if(curr == head){
                removeFront();
            } else if (curr == tail){
                removeBack();
            } else {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete curr;
            }
        }

        void saveUserToFile(string filename){
            ofstream file(filename);
                if(!file.is_open()){
                    cout << "Error opening file " << filename << endl;
                    return;
                }

                UserNode* curr = head;
                file << "index,user_id,username,password_hash,created_at" << endl;

                int index = 1;

                while(curr != nullptr){
                    file << index << ','
                        << curr->userData.userID << ','
                        << curr->userData.username << ','
                        << curr->userData.userPassword << ','
                        << curr->userData.createdAt << endl;
                    curr = curr->next;
                    index++;
                }
                file.close();
                cout << "User data successfully saved to " << filename << endl;
        }

        void loadUserFromFile(string filename){
            bool firstLine = true;
            ifstream file(filename);

            if(!file.is_open()){
                cout << "Error opening file " << filename << endl;
                return;
            }

            string line;

            while(getline(file, line)){
                if(firstLine){
                    firstLine = false;
                    continue;
                }

                istringstream iss(line);
                int index;
                string field;

                UserData userData;
                getline(iss, field, ',');
                try{
                    index = stoi(field);
                } catch(const exception& e){
                    cerr << "Error converting index: " << e.what() << endl;
                    continue;
                }

                if(getline(iss, userData.userID, ',') &&
                    getline(iss, userData.username, ',') &&
                    getline(iss, userData.userPassword, ',') &&
                    getline(iss, userData.createdAt, ',')) {
                        UserNode* newNode = new UserNode(userData);
                        insertFront(userData);
                    }
            }
            file.close();
        }

        bool searchAndCompare(string inputId, string inputPassword){
            UserNode* curr = head;
            while(curr != nullptr){
                cout << inputId << endl;
                cout << curr->userData.userID << endl;
                cout << md5Hash(inputPassword) << endl;
                cout << curr->userData.userPassword << endl;

                if(normalizeID(curr->userData.userID) == normalizeID(inputId) && curr->userData.userPassword == md5Hash(inputPassword)){
                    cout << "goon";
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }
        

};

#endif