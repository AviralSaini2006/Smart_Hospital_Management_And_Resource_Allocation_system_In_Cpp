
#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include <iostream>
#include <string>
#include <conio.h>
#include <cstdlib>
#include <windows.h>

namespace UI {

    const std::string RESET = "\033[0m";
    const std::string CYAN = "\033[96m";
    const std::string BLUE = "\033[94m";
    const std::string GREEN = "\033[92m";
    const std::string YELLOW = "\033[93m";
    const std::string RED = "\033[91m";
    const std::string WHITE = "\033[97m";
    const std::string GRAY = "\033[90m";

    inline void init() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;

    if (GetConsoleMode(hOut, &mode)) {
        SetConsoleMode(hOut, mode | 0x0004);
    }
}

    inline void clear() {
        system("cls");
    }

    inline void line() {
        std::cout << CYAN
                  << "+------------------------------------------------------------+"
                  << RESET << '\n';
    }

    inline void header(const std::string& title,
                       const std::string& subtitle = "SMART HOSPITAL MANAGEMENT AND RESOURCE ALLOCATION SYSTEM") {
        clear();

        line();
        std::cout << CYAN << "|"
                  << WHITE << "                         MediAlloc                          "
                  << CYAN << "|" << RESET << '\n';

        line();

        std::cout << CYAN << "| " << YELLOW << title;

        int spaces = 57 - static_cast<int>(title.length());
        for (int i = 0; i < spaces; i++)
            std::cout << ' ';

        std::cout << CYAN << "|" << RESET << '\n';

        std::cout << CYAN << "| " << GRAY << subtitle;

        spaces = 57 - static_cast<int>(subtitle.length());
        for (int i = 0; i < spaces; i++)
            std::cout << ' ';

        std::cout << CYAN << "|" << RESET << '\n';

        line();
        std::cout << '\n';
    }

    inline void menuItem(char key, const std::string& text) {
        std::cout << "   "
                  << GREEN << "[" << key << "]" << RESET
                  << "  " << WHITE << text << RESET << '\n';
    }

    inline void footer() {
        std::cout << '\n';
        line();
        std::cout << GRAY
                  << "   Press a number key to select an option"
                  << RESET << '\n';
        std::cout << GRAY
                  << "   Press 0 to go back or exit"
                  << RESET << '\n';
        line();
        std::cout << CYAN << "\n   Selection: " << RESET;
    }

    inline int getChoice(int maximum) {
        while (true) {
            int key = _getch();

            if (key >= '0' && key <= '9') {
                int choice = key - '0';

                if (choice <= maximum) {
                    std::cout << GREEN << key << RESET << '\n';
                    return choice;
                }
            }
        }
    }

    inline void pause() {
        std::cout << '\n'
                  << GRAY
                  << "   Press any key to continue..."
                  << RESET;

        _getch();
    }

    inline void message(const std::string& text,
                        bool success = true) {
        std::cout << '\n'
                  << (success ? GREEN : RED)
                  << "   " << text
                  << RESET << '\n';
    }

    inline void section(const std::string& title) {
        std::cout << '\n'
                  << CYAN << "   --- " << title << " ---"
                  << RESET << '\n';
    }

}

#endif
