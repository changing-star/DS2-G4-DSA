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
    private:
        UserData userData;
        UserNode* prev;
        UserNode* next;

        UserNode(UserData userData){
            this->userData.userID = userData.userID;
            this->userData.username = userData.username;
            this->userData.userPassword = md5Hash(userData.userPassword);
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
        }

        void displayUser(){

        }

        void removeFront(){

        }
        
        void removeBack(){

        }

        void removeByID(string adminID){

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
                cout << "Admin data successfully saved to " << filename << endl;
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
                if(curr->userData.userID == inputId && curr->userData.userPassword == md5Hash(inputPassword)){
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }
        

};

#endif