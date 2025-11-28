# DS2-G4-DSA
Repository for final project of Data Structure and Algorithm (C++) class.

## Data Structures Used
- Admin List, Doubly Linked List
- User List, Doubly Linked List
- Book Binary tree, Binary Search Tree
- Transaction Stack, Stack for history
# Development


## Compiling
```bash
#Linux
g++ -I./utils main.cpp -o library_manager -lssl -lcrypto
#Windows
g++ -I./include -I./utils src/main.cpp -o library_manager.exe
#MacOS
g++ -I./include -I./utils -o library_manager -std=c++11
```