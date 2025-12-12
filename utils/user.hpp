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
                cout<< "No users avalilable.\n";
                return;
            }
            UserNode* curr = head;
            int index = 1;

            cout << left << setw(6) <<"No."
                 << setw(15) << "User ID"
                 << setw(20) << "Username"
                 << setw(40) << "Password (hashed)" << endl;

            cout<< string(80, '-') << endl;

            while(curr != nullptr){
                cout << left << setw(6) << index
                     << setw(15) << curr -> userData.userID
                     << setw(20) << curr -> userData.username
                     << setw(40) << curr -> userData.userPassword
                     << endl;

                curr = curr -> next;
                index++;
            }


        }

        void edit(string inputID){
            if(isEmpty()){
                cout << "User list is empty!" << endl;
                return;
            }

            UserNode* curr = head;
            while(curr != nullptr){
                if(curr->userData.userID == inputID){
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

                if(curr->userData.userID == inputId && curr->userData.userPassword == md5Hash(inputPassword)){
                    cout << "goon";
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }
        

};

#endif