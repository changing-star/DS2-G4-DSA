#ifndef book_hpp
#define book_hpp
#include "shared.hpp"

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
            size++;

            return node;
        }

        BookNode *searchByIDHelper(BookNode *node, string id){
            if(node == nullptr){
                return nullptr;
            } else if (id == node->bookData.bookID){
                return node;
            } else if (id > node->bookData.bookID){
                return searchByIDHelper(node->right, id);
            } else if (id < node->bookData.bookID){
                return searchByIDHelper(node->left, id);
            }
        }

        BookNode *getSuccessor(BookNode* curr){
            curr = curr->right;
            while(curr != nullptr && curr->left != nullptr){
                curr = curr->left;
            }
            return curr;
        }

        BookNode *removeByIDHelper(BookNode *node, string id){
            if(node == nullptr){
                return nullptr;
            } else if (node->bookData.bookID > id){
                node->left = removeByIDHelper(node->left, id);
            } else if (node->bookData.bookID < id){
                node->right = removeByIDHelper(node->right, id);
            } else {
                if(node->left == nullptr){
                    BookNode* temp = node->right;
                    delete node;
                    return temp;
                }
                if (node->right == nullptr){
                    BookNode* temp = node->left;
                    delete node;
                    return temp;
                }
            }
            BookNode* succ = getSuccessor(node);
            node->bookData = succ->bookData;
            node->right = removeByIDHelper(root->right, succ->bookData.bookID);
            
        }

        vector<BookData> searchByAuthorHelper(BookNode *node, string inputAuthor){
            vector<BookData> bookDataVector;
            if(node != nullptr){
                searchByAuthorHelper(node->left, inputAuthor);
                if(inputAuthor == node->bookData.bookAuthor){
                    bookDataVector.push_back(node->bookData);
                }
                searchByAuthorHelper(node->right, inputAuthor);
            }
            return bookDataVector;
        }

        vector<BookData> searchByTitleHelper(BookNode *node, string inputTitle){
            vector<BookData> bookDataVector;
            //Implement

            return bookDataVector;
        }

        vector<BookData> preOrderHelper(BookNode *node){
            vector<BookData> bookDataVector;
            if(node != nullptr){
                bookDataVector.push_back(node->bookData);
                preOrderHelper(node->left);
                preOrderHelper(node->right);
            }
            return bookDataVector;
        }

        vector<BookData> inOrderHelper(BookNode *node){
            vector<BookData> bookDataVector;
            if(node != nullptr){
                inOrderHelper(node->left);
                bookDataVector.push_back(node->bookData);
                inOrderHelper(node->right);
            }
            return bookDataVector;
        }

        vector<BookData> postOrderHelper(BookNode *node){
            vector<BookData> bookDataVector;
            if(node != nullptr){
                inOrderHelper(node->left);
                inOrderHelper(node->right);
                bookDataVector.push_back(node->bookData);
            }
            return bookDataVector;
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

        //Insert book into a BST pre sorted lexographically
        void insert(BookData bookData){
            root = insertHelper(root, bookData);
        }

        //Returns book node by ID
        BookNode* searchByID(string id){
            searchByIDHelper(root, id);
        }


        //Remove entry by ID and reorder BST
        BookNode* removeByID(string id){
            removeByIDHelper(root, id);
        }

        
        //Save BST of books into db file
        void saveBooksToFile(string filename){
            ofstream file(filename);
            if(!file.is_open()){
                cout << "Error opening file " << filename << endl;
                return;
            }

            vector<BookData> bookDataVector = inOrderHelper(root);
            file << "index,book_id,title,author,isbn,total_copies,available_copies,availabilty" << 'endl'; 

            int index = 1;
            for(const BookData& itr : bookDataVector){
                file << index << ','
                    << itr.bookID << ','
                    << itr.bookTitle << ','
                    << itr.bookAuthor << ','
                    << itr.isbn << ','
                    << itr.totalCopies << ','
                    << itr.availableCopies << ','
                    << itr.availability << endl ;
                index++;
            }
            
            file.close();
            cout << "Book data successfully saved to " << filename << endl;
        }
        //Load books from db and insert into BST
        void loadBooksFromFile(string filename){
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

                BookData bookData;

                getline(iss, field, ',');
                try{
                    index = stoi(field);
                } catch(const exception& e){
                    cerr << "Error converting index: " << e.what() << endl;
                    continue;
                }

                string totalCopiesField;
                string availableCopiesField;
                string availabilityField;
                
                if(getline(iss, bookData.bookID, ',') &&
                    getline(iss, bookData.bookTitle, ',') &&
                    getline(iss, bookData.bookAuthor, ',') &&
                    getline(iss, bookData.isbn, ',') &&
                    getline(iss, totalCopiesField, ',') &&
                    getline(iss, availableCopiesField, ',') &&     
                    getline(iss, availabilityField, ',')){
                        bookData.totalCopies = stoi(totalCopiesField);
                        bookData.availableCopies = stoi(availableCopiesField);
                        bookData.availability = solve(availabilityField);

                        insert(bookData);
                }
            }

            file.close();

        }

        //Prints the entire book list
        void print(){
            vector<BookData> bookDataVector = inOrderHelper(root);
            if(bookDataVector.empty()) {
                cout << "No books available in the library." << endl;
                return;
            }
            cout<< setw(5) << left << "No:" 
                << setw(15) << left << "Book ID" 
                << setw(30) << left << "Title" 
                << setw(25) << left << "Author" 
                << setw(15) << left << "ISBN" 
                << setw(10) << left << "Total" 
                << setw(10) << left << "Available" 
                << setw(10) << left << "Status" << endl;
            int index = 1;
            for(const auto& book : bookDataVector) {
                cout << setw(5) << left << index
                    << setw(15) << left << book.bookID
                    << setw(30) << left << book.bookTitle
                    << setw(25) << left << book.bookAuthor
                    << setw(15) << left << book.isbn
                    << setw(10) << left << book.totalCopies
                    << setw(10) << left << book.availableCopies
                    << setw(10) << left << (book.availability ? "Yes" : "No") 
                    << endl;
                    index++;
            }
        }

        //Display and change tui i guess, ill do this one
        void transformList(){

        }

        //Minus from available_copies, also should proabably store "available" as either 
        //a variable of a book node to calculate as available copies > 0
        //return or borrow 
        bool updateBorrow(){
            //this is my code
          string id;
          cout<<""Enter Book ID to borrow: ";
          cin>>id;
          BookNode* node = searchByIDHelper(root, id);
          if(!node){
              cout<<"Book ID not found!"<<endl;
              return false;
          }
              if(node->bookData.availableCopies <=0){
                  cout<<"No available copies to borrow!"<<endl;
                  node->bookData.availability = false;
                  return false;
              }
                node->bookData.availableCopies--;
                node->bookData.availability = (node->bookData.availableCopies > 0);
                cout << "Borrow successful. Remaining copies: " 
               << node->bookData.availableCopies << endl;
                return true;
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
    void edit(string inputID) {
    BookNode* node = searchByIDHelper(root, inputID);

    if (!node) {
        cout << "Book ID not found.\n";
        return;
    }

    cout << "Editing Book ID: " << inputID << endl;

    string newTitle, newAuthor, newISBN;
    int newTotal, newAvailable;

    cout << "Enter new Title (" << node->bookData.bookTitle << "): ";
    cin.ignore();
    getline(cin, newTitle);

    cout << "Enter new Author (" << node->bookData.bookAuthor << "): ";
    getline(cin, newAuthor);

    cout << "Enter new ISBN (" << node->bookData.isbn << "): ";
    getline(cin, newISBN);

    cout << "Enter Total Copies (" << node->bookData.totalCopies << "): ";
    cin >> newTotal;

    cout << "Enter Available Copies (" << node->bookData.availableCopies << "): ";
    cin >> newAvailable;

    if (newAvailable > newTotal) {
        cout << "Available cannot exceed total. Edit canceled.\n";
        return;
    }
    node->bookData.bookTitle = newTitle;
    node->bookData.bookAuthor = newAuthor;
    node->bookData.isbn = newISBN;
    node->bookData.totalCopies = newTotal;
    node->bookData.availableCopies = newAvailable;
    node->bookData.availability = (newAvailable > 0);

    cout << "Book updated successfully.\n";
}


//Search by title, create a new searchByTitleHelper
void searchByTitle(string inputTitle){

        }

        //Search by author, create a new searchByAuthorHyper
        void searchByAuthor(string inputAuthorName){
            int itemPerPage = 5;
            int totalPage;
            int totalItem;
            int currentPage;
            vector<BookData> bookDataVector = searchByAuthorHelper(root, inputAuthorName);

            vector<BookData>::iterator vector_itr;

            totalItem = bookDataVector.size();
            totalPage = (int)(totalItem/itemPerPage) + 1;
            currentPage = 1;
            string navKey;

            int lower = 0;
            while(1){
                clearScreen();
                lower = ((currentPage-1)) * itemPerPage;
                cout << "----- Search by Author -----" << endl;
                cout << "Page " << currentPage << " of " << totalPage << endl;
                cout << "Search input: " << inputAuthorName << endl;
                cout << "Total Entries: " << bookDataVector.size() << endl;
                cout << endl;

                cout << setw(25) << left << "     Title" 
                    << setw(20) << left << "     Author" 
                    << setw(20) << left << "     Publish date" 
                    << setw(11) << left << "     avaialable" 
                    << endl;
                
                vector_itr = bookDataVector.begin();
                
            }

        }
};
#endif