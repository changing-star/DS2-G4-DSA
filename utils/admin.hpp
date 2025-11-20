#ifndef admin_hpp
#define admin_hpp
#include "shared.hpp"
#include "hash.hpp"

class AdminNode{
    private:
        string adminId;
        string adminUsername;
        string adminPassword;
        AdminNode* prev;
        AdminNode* next;

        AdminNode(string adminId, string adminUsername, string adminPassword){
            this->adminId = adminId;
            this->adminUsername = adminUsername;
            this->adminPassword = md5Hash(adminPassword);
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
        void insertFront(string id, string adminId, string adminUsername, string adminPassword){

        }

        //Insert Back
        void insertBack(string id, string adminId, string adminUsername, string adminPassword){

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

        }

        //Load Admins from file
        void loadAdminFromFile(string filename){
            
        }

        //Search and compare if user is the user 
        bool searchAndCompare(string inputId, string inputPassword){
            AdminNode* curr = head;
            while(curr != nullptr){
                if(curr->adminId == inputId && curr->adminPassword == md5Hash(inputPassword)){
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }


};

#endif