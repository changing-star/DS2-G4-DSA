#ifndef book_hpp
#define book_hpp
#include "shared.hpp"

//Maybe implemnet a binary search tree here so we can store it alphabetically automatically
//Also probably change the booknode data into a struct because this is fucking ugly as shit

struct BookData{
    string bookID;
    string bookTitle;
    string bookAuthor;
    string isbn;

    int totalCopies;
    int availableCopies;
    bool availability;
};

class BookNode{
    private:
        BookData bookData;
        BookNode *left, *right;
    public:
        BookNode(BookData bookData){
            this->bookData.bookID = bookData.bookID;
            this->bookData.bookTitle = bookData.bookTitle;
            this->bookData.bookAuthor = bookData.bookAuthor;
            this->bookData.isbn = bookData.isbn;
            this->bookData.totalCopies = bookData.totalCopies;
            this->bookData.availableCopies = bookData.availableCopies;
            this->bookData.availability = bookData.availability;
            this->left = this->right = nullptr;
        }
        friend class BookList;
};

class BookList{
    private:
        BookNode* root;
        int size;

        BookNode *insertHelper(BookNode* node, BookData bookData){
            if (node==nullptr){
                size++;
                return new BookNode(bookData);
            }
            
            //String comparison
            if (bookData.bookTitle < node->bookData.bookTitle){
                node->left = insertHelper(node->left, bookData);
            } else if (bookData.bookTitle > node->bookData.bookTitle){
                node->right = insertHelper(node->right, bookData);
            } else if (bookData.bookTitle == node->bookData.bookTitle){
                node->bookData.bookID = bookData.bookID;
                node->bookData.bookTitle = bookData.bookTitle;
                node->bookData.bookAuthor = bookData.bookAuthor;
                node->bookData.isbn = bookData.isbn;
                node->bookData.totalCopies = bookData.totalCopies;
                node->bookData.availableCopies = bookData.availableCopies;
                node->bookData.availability = bookData.availability;
            }

            return node;
        }

        BookNode *searchByIDHelper(BookNode *node, string id){
            if(node == NULL){
                return nullptr;
            } else if (id == node->bookData.bookID){
                return node;
            } else if (id > node->bookData.bookID){
                return searchByIDHelper(node->right, id);
            } else if (id < node->bookData.bookID){
                return searchByIDHelper(node->left, id);
            }
        }
    public:
        BookList(BookData bookData){
            root = new BookNode(bookData);
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
        void insert(BookData bookData){
            root = insertHelper(root, bookData);
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
        void add(BookData inputBookData){
            //get total copies then + 1, use searchByIDHelper
            //Adding also incrementally increases bookID by 1, however it is a string so there 
            //needs to be regex str -> int -> str again
            insert(inputBookData);
            cout << "Book ID " << inputBookData.bookID << " has been added successfully." << endl;
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