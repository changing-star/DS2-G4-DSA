#ifndef book_hpp
#define book_hpp
#include "shared.hpp"
#include <algorithm>

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

        BookNode *insertHelper(BookNode* node, BookData* bookData){
            // Handle empty subtree (base case)
            if (node == nullptr) {
                return new BookNode(*bookData);
            }
            //String comparison
            if (bookData->bookTitle < node->bookData.bookTitle){
                node->left = insertHelper(node->left, bookData);
            } else if (bookData->bookTitle > node->bookData.bookTitle){
                node->right = insertHelper(node->right, bookData);
            } else if (bookData->bookTitle == node->bookData.bookTitle){
                node->bookData.bookID = bookData->bookID;
                node->bookData.bookTitle = bookData->bookTitle;
                node->bookData.bookAuthor = bookData->bookAuthor;
                node->bookData.isbn = bookData->isbn;
                node->bookData.totalCopies = bookData->totalCopies;
                node->bookData.availableCopies = bookData->availableCopies;
                node->bookData.availability = bookData->availability;
            }

            return node;
        }

        BookNode *searchByIDHelper(BookNode *node, string id){
            if(node == nullptr){
                return nullptr;
            } 

            if (node->bookData.bookID == id) {
                return node;
            }
                
            BookNode* leftResult = searchByIDHelper(node->left, id);
            if (leftResult != nullptr) {
                return leftResult;
            }
            return searchByIDHelper(node->right, id);
        }

        BookNode *getSuccessor(BookNode* curr){
            if (curr == nullptr || curr->right == nullptr) {
                return nullptr;
            }
            
            // Successor is leftmost node in right subtree
            curr = curr->right;
            while (curr != nullptr && curr->left != nullptr) {
                curr = curr->left;
            }
            return curr;
        }

        string generateNextBookID() {
            // Find the maximum existing ID number
            int maxID = 0;
            vector<BookData> allBooks = inOrder();
            
            for (const auto& book : allBooks) {
                // Extract numeric part from "BID-XXX"
                if (book.bookID.length() > 3 && book.bookID.substr(0, 4) == "BID-") {
                    try {
                        int num = stoi(book.bookID.substr(3));
                        if (num > maxID) {
                            maxID = num;
                        }
                    } catch (...) {
                    }
                }
            }
            
            // Generate next ID
            stringstream ss;
            ss << "BID-" << setw(4) << setfill('0') << (maxID + 1);
            return ss.str();
        }

        BookNode *removeByIDHelper(BookNode *node, string id){
            if(node == nullptr){
                return nullptr;
            } 

            if (node->bookData.bookID == id) {
                if (node->left == nullptr) {
                    BookNode* temp = node->right;
                    delete node;
                    size--;
                    return temp;
                } else if (node->right == nullptr) {
                    BookNode* temp = node->left;
                    delete node;
                    size--;
                    return temp;
                } else {
                    // Node has two children
                    // Find inorder successor in title-sorted tree
                    BookNode* succ = getSuccessor(node);
                    // Copy successor's data to current node
                    node->bookData = succ->bookData;
                    // Delete the successor (which is in title-sorted position)
                    node->right = removeByTitleHelper(node->right, succ->bookData.bookTitle);
                    size--;
                }
            } else {
                // Not this node, search both subtrees
                node->left = removeByIDHelper(node->left, id);
                node->right = removeByIDHelper(node->right, id);
            }
            
            return node;
        }

        BookNode* removeByTitleHelper(BookNode* node, string title) {
            if (node == nullptr) {
                return nullptr;
            }
            
            if (title < node->bookData.bookTitle) {
                node->left = removeByTitleHelper(node->left, title);
            } else if (title > node->bookData.bookTitle) {
                node->right = removeByTitleHelper(node->right, title);
            } else {
                // Found node to delete
                if (node->left == nullptr) {
                    BookNode* temp = node->right;
                    delete node;
                    return temp;
                } else if (node->right == nullptr) {
                    BookNode* temp = node->left;
                    delete node;
                    return temp;
                } else {
                    BookNode* succ = getSuccessor(node);
                    node->bookData = succ->bookData;
                    node->right = removeByTitleHelper(node->right, succ->bookData.bookTitle);
                }
            }
            return node;
        }

        vector<BookData> preOrderHelper(BookNode *node){
            vector<BookData> bookDataVector;
            if(node != nullptr){
                bookDataVector.push_back(node->bookData);
                vector<BookData> left = preOrderHelper(node->left);
                bookDataVector.insert(bookDataVector.end(), left.begin(), left.end());
                vector<BookData> right = preOrderHelper(node->right);
                bookDataVector.insert(bookDataVector.end(), right.begin(), right.end());
            }
            return bookDataVector;
        }

        vector<BookData> inOrderHelper(BookNode *node){
            vector<BookData> bookDataVector;
            if(node != nullptr){
                vector<BookData> left = inOrderHelper(node->left);
                bookDataVector.insert(bookDataVector.end(), left.begin(), left.end());
                bookDataVector.push_back(node->bookData);
                vector<BookData> right = inOrderHelper(node->right);
                bookDataVector.insert(bookDataVector.end(), right.begin(), right.end());
            }
            return bookDataVector;
        }

        vector<BookData> postOrderHelper(BookNode *node){
            vector<BookData> bookDataVector;
            if(node != nullptr){
                vector<BookData> left = postOrderHelper(node->left);
                bookDataVector.insert(bookDataVector.end(), left.begin(), left.end());
                vector<BookData> right = postOrderHelper(node->right);
                bookDataVector.insert(bookDataVector.end(), right.begin(), right.end());
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

        
        vector<BookData> inOrder(){
            return inOrderHelper(root);
        }
        vector<BookData> preOrder(){
            return preOrderHelper(root);
        }
        vector<BookData> postOrder(){
            return postOrderHelper(root);
        }

        //Show book information
        void showBookInfo(string id){

        }

        //Insert book into a BST pre sorted lexographically
        void insert(BookData* bookData){
            root = insertHelper(root, bookData);
            size++;
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
            file << "index,book_id,title,author,isbn,total_copies,available_copies,availability" << endl;
            int index = 1;
            for(const BookData& itr : bookDataVector){
                file << index << ','
                    << itr.bookID << ','
                    << itr.bookTitle << ','
                    << itr.bookAuthor << ','
                    << itr.isbn << ','
                    << itr.totalCopies << ','
                    << itr.availableCopies << ','
                    << itr.availability << endl;
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
                // Skip header and blank/whitespace lines
                if(firstLine){
                    firstLine = false;
                    continue;
                }
                if(line.empty() || all_of(line.begin(), line.end(), ::isspace)){
                    continue;
                }
                istringstream iss(line);
                int index;
                string field;
                BookData bookData; // stack allocation
                // Parse index field
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
                // Parse all fields, with error handling
                if(getline(iss, bookData.bookID, ',') &&
                   getline(iss, bookData.bookTitle, ',') &&
                   getline(iss, bookData.bookAuthor, ',') &&
                   getline(iss, bookData.isbn, ',') &&
                   getline(iss, totalCopiesField, ',') &&
                   getline(iss, availableCopiesField, ',') &&     
                   getline(iss, availabilityField, ',')){
                    try {
                        bookData.totalCopies = stoi(totalCopiesField);
                        bookData.availableCopies = stoi(availableCopiesField);
                    } catch(const exception& e){
                        cerr << "Error converting copies fields: " << e.what() << endl;
                        continue;
                    }
                    try {
                        bookData.availability = solve(availabilityField);
                    } catch(const exception& e){
                        cerr << "Error converting availability field: " << e.what() << endl;
                        continue;
                    }
                    insert(&bookData);
                } else {
                    cerr << "Malformed line or missing fields: " << line << endl;
                }
            }
            file.close();
        }

        //Prints the entire book list
        void print(){
            vector<BookData> bookDataVector = inOrderHelper(root);
            print(bookDataVector);
        }
        //Prints any vector of books
        void print(const vector<BookData>& bookDataVector){
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
            for(const BookData& book : bookDataVector) {
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

        // Helper function to get visible length of string (excluding ANSI codes)
        size_t getVisibleLength(const string& str) {
            size_t visibleLen = 0;
            bool inEscape = false;
            for (char c : str) {
                if (c == '\033') {
                    inEscape = true;
                } else if (inEscape && c == 'm') {
                    inEscape = false;
                } else if (!inEscape) {
                    visibleLen++;
                }
            }
            return visibleLen;
        }

        // Helper function to pad string to specific width (accounting for ANSI codes)
        string padString(const string& str, size_t width) {
            size_t visibleLen = getVisibleLength(str);
            if (visibleLen >= width) {
                return str;
            }
            return str + string(width - visibleLen, ' ');
        }

        //Prints any vector of books with highlighted search keyword
        void print(const vector<BookData>& bookDataVector, const string& searchKeyword, const string& searchField){
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
            for(const BookData& book : bookDataVector) {
                string displayTitle = book.bookTitle;
                
                // Apply highlighting based on search field
                if(searchField == "title") {
                    displayTitle = highlightMatch(book.bookTitle, searchKeyword);
                }
                
                // Manually pad strings to account for ANSI codes
                cout << padString(to_string(index), 5)
                    << padString(book.bookID, 15)
                    << padString(displayTitle, 30)
                    << padString(book.bookAuthor, 25)
                    << padString(book.isbn, 15)
                    << padString(to_string(book.totalCopies), 10)
                    << padString(to_string(book.availableCopies), 10)
                    << padString((book.availability ? "Yes" : "No"), 10)
                    << endl;
                    index++;
            }
        }

        //Display and change tui i guess, ill do this one
        void transformList(){
            vector<BookData> bookDataVector = inOrder();
            if (bookDataVector.empty()) {
                clearScreen();
                cout << "No books available in the library." << endl;
                cout << "Press any key to return..." << endl;
                cin.ignore();
                cin.get();
                return;
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
            int totalItem = bookDataVector.size();
            int totalPage = (totalItem + itemPerPage - 1) / itemPerPage;
            int currentPage = 1;
            string navKey;
            while (true) {
                clearScreen();
                cout << "===== View all book =====" << endl;
                cout << "Page " << currentPage << " of " << totalPage << endl;
                cout << "Total books: " << totalItem << endl;
                int availableCount = 0, borrowedCount = 0;
                for (const auto& book : bookDataVector) {
                    if (book.availableCopies > 0) availableCount++;
                    else borrowedCount++;
                }
                cout << "Available books: " << availableCount << endl;
                cout << "Borrowed books: " << borrowedCount << endl;
                cout << endl;
                int start = (currentPage - 1) * itemPerPage;
                int end = min(start + itemPerPage, totalItem);
                vector<BookData> pageBooks(bookDataVector.begin() + start, bookDataVector.begin() + end);
                print(pageBooks);
                cout << endl;
                cout << "*********************************************************" << endl;
                cout << "Navigation: previous page '<-', next page '->', exit 'q'" << endl;
                navKey = readNav();
                if (navKey == "left" && currentPage > 1) currentPage--;
                else if (navKey == "right" && currentPage < totalPage) currentPage++;
                else if (navKey == "exit") break;
            }
        }

        //Minus from available_copies, also should proabably store "available" as either 
        //a variable of a book node to calculate as available copies > 0
        //return or borrow 
        bool updateBorrow(string inputItemID){
            string id;
            id = inputItemID;
            BookNode* node = searchByIDHelper(root, id);
            if(!node){
                cout<<"Book ID not found!"<<endl;
                return false;
            }
            if(node->bookData.availableCopies <= 0){
                cout<<"No available copies to borrow!"<<endl;
                node->bookData.availability = false;
                return false;
            }
            node->bookData.availableCopies--;
            node->bookData.availability = (node->bookData.availableCopies > 0);
            cout << "Borrow successful. Remaining copies: " << node->bookData.availableCopies << endl;
            return true;
        }

        bool updateReturn(string inputItemID){
            string id;
            id = inputItemID;
            BookNode* node = searchByIDHelper(root,id);
            if(!node){
                cout << "Book ID not found!" << endl;
                return false;
            }
            node->bookData.availability = true;
            node->bookData.availableCopies++;
            cout << "Return successful. Total copies: " << node->bookData.availableCopies << endl;
            return true;
        }

        //ADMIN 
        //this needs to update total copies
        void add(BookData* inputBookData){
            //get total copies then + 1, use searchByIDHelper
            //Adding also incrementally increases bookID by 1, however it is a string so there 
            //needs to be regex str -> int -> str again

            BookData* bookData;

            bookData->bookTitle = inputBookData->bookTitle;
            bookData->bookAuthor = inputBookData->bookAuthor;
            bookData->isbn = inputBookData->isbn;

            string newID = generateNextBookID();
            inputBookData->bookID = newID;
            inputBookData->availability = (inputBookData->availableCopies > 0);
            
            insert(bookData);
            cout << "Book ID " << inputBookData->bookID << " has been added successfully." << endl;
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
        // Helper for case-insensitive, partial match for title
        bool titleMatches(const string& title, const string& input) {
            string titleLower = title, inputLower = input;
            transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);
            transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);
            return titleLower.find(inputLower) != string::npos;
        }
        // Highlight matched substring in title using ANSI color
        string highlightMatch(const string& title, const string& input) {
            string titleLower = title, inputLower = input;
            transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);
            transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);
            size_t pos = titleLower.find(inputLower);
            if (pos != string::npos) {
                return title.substr(0, pos) + "\033[1;33m" + title.substr(pos, input.length()) + "\033[0m" + title.substr(pos + input.length());
            }
            return title;
        }
        void searchByTitle(string inputTitle) {
            vector<BookData> allBooks = inOrder();
            vector<BookData> matches;
            for (const auto& book : allBooks) {
                if (titleMatches(book.bookTitle, inputTitle)) {
                    matches.push_back(book);
                }
            }
            int itemPerPage = 5;
            int totalItem = matches.size();
            int totalPage = (totalItem + itemPerPage - 1) / itemPerPage;
            int currentPage = 1;
            string navKey;
            if (totalItem == 0) {
                clearScreen();
                cout << "No books found for title: " << inputTitle << endl;
                cout << "Press any key to return..." << endl;
                cin.ignore();
                cin.get();
                return;
            }
            while (true) {
                clearScreen();
                cout << "----- Search by Title -----" << endl;
                cout << "Page " << currentPage << " of " << totalPage << endl;
                cout << "Search input: " << inputTitle << endl;
                cout << "Total Entries: " << totalItem << endl;
                cout << endl;
                int start = (currentPage - 1) * itemPerPage;
                int end = min(start + itemPerPage, totalItem);
                // Use print method to display current page with highlighting
                vector<BookData> pageBooks(matches.begin() + start, matches.begin() + end);
                this->print(pageBooks, inputTitle, "title");
                cout << endl;
                cout << "*********************************************************" << endl;
                cout << "Navigation: previous page '<-', next page '->', exit 'q'" << endl;
                navKey = readNav();
                if (navKey == "left" && currentPage > 1) currentPage--;
                else if (navKey == "right" && currentPage < totalPage) currentPage++;
                else if (navKey == "exit") break;
            }
        }

        //Search by author, create a new searchByAuthorHyper
        // Helper for case-insensitive, partial match
        bool authorMatches(const string& author, const string& input) {
            string authorLower = author, inputLower = input;
            transform(authorLower.begin(), authorLower.end(), authorLower.begin(), ::tolower);
            transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);
            return authorLower.find(inputLower) != string::npos;
        }

        void searchByAuthor(string inputAuthorName){
            vector<BookData> allBooks = inOrder();
            vector<BookData> matches;
            for (const BookData& book : allBooks) {
                if (authorMatches(book.bookAuthor, inputAuthorName)) {
                    matches.push_back(book);
                }
            }
            int itemPerPage = 5;
            int totalItem = matches.size();
            int totalPage = (totalItem + itemPerPage - 1) / itemPerPage;
            int currentPage = 1;
            string navKey;
            if (totalItem == 0) {
                clearScreen();
                cout << "----- Search by Author -----" << endl;
                cout << "No books found for author: " << inputAuthorName << endl;
                cout << "Press any key to return..." << endl;
                cin.ignore();
                cin.get();
                return;
            }
            while (true) {
                clearScreen();
                cout << "----- Search by Author -----" << endl;
                cout << "Page " << currentPage << " of " << totalPage << endl;
                cout << "Search input: " << inputAuthorName << endl;
                cout << "Total Entries: " << totalItem << endl;
                cout << endl;
                int start = (currentPage - 1) * itemPerPage;
                int end = min(start + itemPerPage, totalItem);
                // Use print method to display current page with highlighting
                if (matches.size() > 0) {
                    vector<BookData> pageBooks(matches.begin() + start, matches.begin() + end);
                    this->print(pageBooks, inputAuthorName, "author");
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

        void searchById(string inputId){
            vector<BookData> allBooks = inOrder();
            vector<BookData> matches;
            for (const BookData& book : allBooks) {
                if (book.bookID == inputId) {
                    matches.push_back(book);
                }
            }
            int itemPerPage = 5;
            int totalItem = matches.size();
            int totalPage = (totalItem + itemPerPage - 1) / itemPerPage;
            int currentPage = 1;
            string navKey;
            if (totalItem == 0) {
                clearScreen();
                cout << "----- Search by ID -----" << endl;
                cout << "No books found for ID: " << inputId << endl;
                cout << "Press any key to return..." << endl;
                cin.ignore();
                cin.get();
                return;
            }
            while (true) {
                clearScreen();
                cout << "----- Search by ID -----" << endl;
                cout << "Page " << currentPage << " of " << totalPage << endl;
                cout << "Search input: " << inputId << endl;
                cout << "Total Entries: " << totalItem << endl;
                cout << endl;
                int start = (currentPage - 1) * itemPerPage;
                int end = min(start + itemPerPage, totalItem);
                // Use print method to display current page (no highlighting for ID search)
                if (matches.size() > 0) {
                    vector<BookData> pageBooks(matches.begin() + start, matches.begin() + end);
                    this->print(pageBooks);
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
};
#endif