#ifndef admin_hpp
#define admin_hpp
#include "shared.hpp"
#include "hash.hpp"

struct AdminData{
    string adminID;
    string adminUsername;
    string adminPassword;
};

class AdminNode{
    private:
        AdminData adminData;
        AdminNode* prev;
        AdminNode* next;

        AdminNode(AdminData adminData){
            this->adminData.adminID = adminData.adminID;
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

        //Insert Back
        void insertBack(AdminData adminData){
            AdminNode* newNode = new AdminNode(adminData);
            if(head == nullptr){
                head = tail = newNode;
            } else {
                newNode->prev = tail;
                tail->next = newNode;
                tail = newNode;
            };
        }
        //Display List of Admins
        void displayAdmin(){
            //testing
            if(head == nullptr){
                cout<< "No admins avalilable.\n";
                return;
            }
            AdminNode* curr = head;
            int index = 1;

            cout << left << setw(6) <<"No."
                 << setw(15) << "Admin ID"
                 << setw(20) << "Username"
                 << setw(40) << "Password (hashed)" << endl;

            cout<< string(80, '-') << endl;

            while(curr != nullptr){
                cout << left << setw(6) << index
                     << setw(15) << curr -> adminData.adminID
                     << setw(20) << curr -> adminData.adminUsername
                     << setw(40) << curr -> adminData.adminPassword
                     << endl;

                curr = curr -> next;
                index++;
            }

        }

        //Remove Front
        void removeFront(){

        }

        //Remove Back
        void removeBack(){

        }

        //Remove by ID
        void removeByID(string adminID) {
    if (head == nullptr) {
        cout << "Admin list is empty.\n";
        return;
    }

    AdminNode* curr = head;
    while (curr != nullptr && curr->adminData.adminID != adminID) {
        curr = curr->next;
    }

    if (curr == nullptr) {
        cout << "Admin ID " << adminID << " not found.\n";
        return;
    }
    if (curr == head) {
        head = head->next;
        if (head != nullptr)
            head->prev = nullptr;
        else
            tail = nullptr; 
    }
    else if (curr == tail) {
        tail = tail->prev;
        if (tail != nullptr)
            tail->next = nullptr;
    }
    else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }

    delete curr;
    size--;

    cout << "Admin ID " << adminID << " removed successfully.\n";
}

        //Save Admins to file
        void saveAdminToFile(string filename){
            ofstream file(filename);
            if(!file.is_open()){
                cout << "Error opening file " << filename << endl;
                return;
            }

            AdminNode* curr = head;

            file << "index,admin_id,admin_username,admin_password" << endl;

            int index = 1;
            while(curr != nullptr){
                file << index << ','
                    << curr->adminData.adminID << ','
                    << curr->adminData.adminUsername << ','
                    << curr->adminData.adminPassword << endl ;
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
                    cerr << "Error converting index: " << e.what() << endl;
                    continue;
                }

                if(getline(iss, adminData.adminID, ',') &&
                    getline(iss, adminData.adminUsername, ',') &&
                    getline(iss, adminData.adminPassword, ',')){
                        AdminNode* newNode = new AdminNode(adminData);
                        insertFront(adminData);
                    }
            }
            file.close();
        }

        //Search and compare if user is the user 
        bool searchAndCompare(string inputId, string inputPassword){
            AdminNode* curr = head;
            while(curr != nullptr){
                if(curr->adminData.adminID == inputId && curr->adminData.adminPassword == md5Hash(inputPassword)){
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }


};

#endif