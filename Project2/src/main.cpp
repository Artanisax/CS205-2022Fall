#include "calculator.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "\nWelcome to A Better Calculator by Artanisax(12110524)!\n"
            "Used for CS205-2022Fall Project 2\n"
            "(Input \"quit\" to quit)\n\n";
    calculator cal;
    string s1, s2;
    size_t len;
    ll exp;
    while (true) {
        getline(cin, s1);
        cout << cal.analyse(s1) << "\n\n";
    }
    return 0;
}
