#ifndef admin_hpp
#define admin_hpp
#include "shared.hpp"
#include "hash.hpp"

struct AdminData{
    string adminId;
    string adminUsername;
    string adminPassword;
};

class AdminNode{
    private:
        AdminData adminData;
        AdminNode* prev;
        AdminNode* next;

        AdminNode(AdminData adminData){
            this->adminData.adminId = adminData.adminId;
            this->adminData.adminUsername = adminData.adminUsername;
            this->adminData.adminPassword = md5Hash(adminData.adminPassword);
            prev = next = nullptr;
        }

        friend class AdminList;
};

class AdminList{
    private:
        AdminNode* head;
        AdminNode* tail;
        int size;
    public:
        AdminList(){
            head = tail = nullptr;
            size = 0;
        }

        int getSize(){
            return size;
        }

        //Insert Front
        void insertFront(AdminData adminData){

        }

        //Insert Back
        void insertBack(AdminData adminData){

        }

        //Display List of Admins
        void displayAdmin(){

        }

        //Remove Front
        void removeFront(){

        }

        //Remove Back
        void removeBack(){

        }

        //Remove by ID
        void removeByID(string adminId){

        }

        //Save Admins to file
        void saveAdmin(string filename){
            ofstream file(filename);
            if(!file.is_open()){
                cout << "Error opening file " << filename << endl;
                return;
            }

            AdminNode* curr = head;
            int index = 1;
            while(curr != nullptr){
                file << index << ','
                    << curr->adminData.adminId << ','
                    << curr->adminData.adminUsername << ','
                    << curr->adminData.adminPassword << 'endl';
                curr = curr->next;
                index++;
            }
            file.close();
            cout << "Admin data successfully saved to " << filename << endl;
        }

        //Load Admins from file
        void loadAdminFromFile(string filename){
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
                AdminData adminData;

                getline(iss, field, ',');
                try{
                    index = stoi(field);
                } catch(const exception& e){
                    std::cerr << "Error converting index: " << e.what() << std::endl;
                    continue;
                }

                if(getline(iss, adminData.adminId, ',') &&
                    getline(iss, adminData.adminUsername, ',') &&
                    getline(iss, adminData.adminPassword, ',')){
                        AdminNode* newNode = new AdminNode(adminData);
                        if(head == nullptr){
                            head = tail = newNode;
                        } else {
                            tail->next = newNode;
                            newNode->prev = tail;
                            tail = newNode;
                        }
                        size++;
                    }
            }
            file.close();
        }

        //Search and compare if user is the user 
        bool searchAndCompare(string inputId, string inputPassword){
            AdminNode* curr = head;
            while(curr != nullptr){
                if(curr->adminData.adminId == inputId && curr->adminData.adminPassword == md5Hash(inputPassword)){
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }


};

#endif