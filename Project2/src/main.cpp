#include "calculator.hpp"

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
    cout << "\nWelcome to A Better Calculator by Artanisax(12110524)!\n"
            "Used for CS205-2022Fall Project 2\n"
            "(Input \"quit\" to quit)\n" << endl;
    calculator cal;
    string s, expression;
    while (true) {
        getline(cin, s);
        expression += s;
        if (s.back() == '\\') expression.pop_back();  // new line
        else {
            cout << cal.analyse(expression);
            expression.clear();
        }
    }
    return 0;
}
