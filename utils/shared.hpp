#ifndef shared_hpp
#define shared_hpp

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <tuple>
#include <cstring>
#include <bits/stdc++.h>

using namespace std;
// detect OS
#ifdef _WIN32
    //WIN32
    #define OS "Windows 32-bit"
    #include <conio.h>
    #include <iomanip>
    string readArrowKeys() {
        std::cout << "Press arrow keys or 'q' to quit.\n";
        while (true) {
            int ch = _getch(); // Read the first character

            if (ch == 0 || ch == 224) { // Special key prefix (arrow keys)
                ch = _getch();          // Read the actual key code

                switch (ch) {
                    case 77: // Right arrow
                        std::cout << "Right arrow key pressed.\n";
                        return "right";
                        break;
                    case 75: // Left arrow
                        std::cout << "Left arrow key pressed.\n";
                        return "left";
                        break;
                    default:
                        std::cout << "Unknown key pressed.\n";
                }
            } else if (ch == 'q' || ch == 'Q') { // Press 'q' to quit
                std::cout << "Exiting...\n";
                return "exit";
                break;
            }
        }
    }
    string readNav() {
        string input;
        input = readArrowKeys();
        return input;
    }
    void clearScreen() {
        system("cls");
    }
    //END_WIN32
#elif _WIN64
    //WIN64
    #define OS "Windows 64-bit"
    #include <conio.h>
    #include <iomanip>
    string readArrowKeys() {
        std::cout << "Press arrow keys or 'q' to quit.\n";
        while (true) {
            int ch = _getch(); // Read the first character

            if (ch == 0 || ch == 224) { // Special key prefix (arrow keys)
                ch = _getch();          // Read the actual key code

                switch (ch) {
                    case 77: // Right arrow
                        std::cout << "Right arrow key pressed.\n";
                        return "right";
                        break;
                    case 75: // Left arrow
                        std::cout << "Left arrow key pressed.\n";
                        return "left";
                        break;
                    default:
                        std::cout << "Unknown key pressed.\n";
                }
            } else if (ch == 'q' || ch == 'Q') { // Press 'q' to quit
                std::cout << "Exiting...\n";
                return "exit";
                break;
            }
        }
    }
    string readNav() {
        string input;
        input = readArrowKeys();
        return input;
    }
    void clearScreen() {
        system("cls");
    }
    //END_WIN64
#elif __APPLE__ || __MACH__
    //MAC
    #define OS "Mac OSX"
    #include <termios.h>
    #include <unistd.h>
    void setTerminalMode(bool enable) {
        static struct termios oldt, newt;
        if (enable) {
            // Get current terminal attributes
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            // Disable canonical mode and echo
            newt.c_lflag &= ~(ICANON | ECHO);
            // Apply new terminal settings
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        } else {
            // Restore old terminal settings
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
    }
    string readArrowKeys() {
        char ch;
        while (true) {
            ch = getchar(); // Read the first character
            if (ch == '\033') { // Escape character
                getchar();      // Skip the '[' character
                ch = getchar(); // Read the actual key

                switch (ch) {
                    case 'C': // Right arrow
                        std::cout << "Right arrow key pressed.\n";
                        return "right";
                        break;
                    case 'D': // Left arrow
                        std::cout << "Left arrow key pressed.\n";
                        return "left";
                        break;
                    default:
                        std::cout << "Unknown escape sequence.\n";
                }
            } else if (ch == 'q') { // Press 'q' to quit
                std::cout << "Exiting...\n";
                return "exit";
                break;
            }
        }
    }

    string readNav() {
        string input;
        setTerminalMode(true);
        input = readArrowKeys();
        setTerminalMode(false);
        return input;
    }
    void clearScreen() {
        system("clear");
    }
//END_MAC   
#elif __linux__
    #define OS "Linux"
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>

    void setTerminalMode(bool enable) {
        static struct termios oldt, newt;
        if (enable) {
            // Get current terminal attributes
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            // Disable canonical mode and echo
            newt.c_lflag &= ~(ICANON | ECHO);
            // Apply new terminal settings
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        } else {
            // Restore old terminal settings
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
    }

    string readArrowKeys() {
        char ch;
        while (true) {
            ch = getchar(); // Read the first character
            if (ch == '\033') { // Escape character
                getchar();      // Skip the '[' character
                ch = getchar(); // Read the actual key

                switch (ch) {
                    case 'C': // Right arrow
                        std::cout << "Right arrow key pressed.\n";
                        return "right";
                        break;
                    case 'D': // Left arrow
                        std::cout << "Left arrow key pressed.\n";
                        return "left";
                        break;
                    default:
                        std::cout << "Unknown escape sequence.\n";
                }
            } else if (ch == 'q' || ch == 'Q') { // Press 'q' to quit
                std::cout << "Exiting...\n";
                return "exit";
                break;
            }
        }
    }
    
    string readNav() {
        string input;
        setTerminalMode(true);
        input = readArrowKeys();
        setTerminalMode(false);
        return input;
    }
    
    void clearScreen() {
        system("clear");
    }
    //END_LINUX
#else
    #define OS "Other"
#endif

// end of detect OS
#endif