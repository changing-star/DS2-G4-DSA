#ifndef book_hpp
#define book_hpp
#include "shared.hpp"

//Maybe implemnet a binary search tree here so we can store it alphabetically automatically
//Also probably change the booknode data into a struct because this is fucking ugly as shit
class BookNode{
    private:
        string bookID;
        string bookTitle;
        string bookAuthor;
        string isbn;
        int total_copies;
        int available_copies;
        bool availability;

        BookNode *left, *right;
    public:
        BookNode(string bookID, string bookTitle, string bookAuthor, string isbn, int total_copies, int available_copies, bool status){
            this->bookID = bookID;
            this->bookTitle = bookTitle;
            this->bookAuthor = bookAuthor;
            this->isbn = isbn;
            this->total_copies = total_copies;
            this->available_copies = available_copies;
            this->availability = status;
            this->left = this->right = nullptr;
        }
        friend class BookList;
};

class BookList{
    private:
        BookNode* root;
        int size;

        BookNode *insertHelper(BookNode* node, string bookID, string bookTitle, string bookAuthor, string isbn, int total_copies, int available_copies, bool status){
            if (node==nullptr){
                size++;
                return new BookNode(bookID, bookTitle, bookAuthor, isbn, total_copies, available_copies, status);
            }
            
            if (bookTitle < node->bookTitle){
                node->left = insertHelper(node->left, bookID, bookTitle, bookAuthor, isbn, total_copies, available_copies, status);
            } else if (bookTitle > node->bookTitle){
                node->right = insertHelper(node->right, bookID, bookTitle, bookAuthor, isbn, total_copies, available_copies, status);
            } else if (bookTitle == node->bookTitle){
                node->bookID = bookID;
                node-> bookTitle = bookTitle;
                node->bookAuthor = bookAuthor;
                node->isbn = isbn;
                node->total_copies = total_copies;
                node->availability = available_copies;
                node->availability = status;
            }

            return node;
        }

        BookNode *searchByIDHelper(BookNode *node, string id){
            if(node == NULL){
                return nullptr;
            } else if (id == node->bookID){
                return node;
            } else if (id > node->bookID){
                return searchByIDHelper(node->right, id);
            } else if (id < node->bookID){
                return searchByIDHelper(node->left, id);
            }
        }
    public:
        BookList(string bookID, string bookTitle, string bookAuthor, string isbn , int total_copies, int available_copies, bool status){
            root = new BookNode(bookID, bookTitle, bookAuthor, isbn, total_copies, available_copies, status);
            size = 1;
        }
        BookList(){
            root = nullptr;
            size = 0;
        }

        int getSize(){
            return size;
        }

        BookNode* getRoot(){
            return root;
        }

        //Show book information
        void showBookInfo(string id){

        }

        //Remove entry by ID and reorder BST
        void removeByID(string id){

        }

        //Insert book into a BST pre sorted lexographically
        void insert(string id, string title, string author, string isbn, int total_copies, int available_copies, bool status){
            root = insertHelper(root, id, title, author, isbn, total_copies, available_copies, status);
        }

        //Returns book node by ID
        BookNode* searchByID(string id){
            searchByIDHelper(root, id);
        }

        //Load books from db and insert into BST
        void loadBooksFromDB(string filename){

        }

        //Save BST of books into db file
        void saveBooksToDB(string filename){

        }

        //Prints the entire book list
        void print(){

        }

        //Display and change tui i guess, ill do this one
        void transformList(){

        }

        //Minus from available_copies, also should proabably store "available" as either 
        //a variable of a book node to calculate as available copies > 0
        //return or borrow 
        bool updateBorrow(){

        }

        //ADMIN 
        //this needs to update total copies
        void add(string inputID, string inputTitle, string inputAuthorName, string isbn, int total_copies, int available_copies){
            //get total copies then + 1, use searchByIDHelper
            //Adding also incrementally increases bookID by 1, however it is a string so there 
            //needs to be regex str -> int -> str again
            insert(inputID, inputTitle, inputAuthorName, isbn, total_copies, available_copies, true);
            cout << "Book ID " << inputID << " has been added successfully." << endl;
        }

        //Edit book by ID
        void edit(string inputID){
        }

        //Remove book by ID
        void remove(string inputID){
        }

        //Search by title, create a new searchByTitleHelper
        void searchByTitle(string inputTitle){

        }

        //Search by author, create a new searchByAuthorHyper
        void searchByAuthor(string inputAuthorName){

        }
};
#endif