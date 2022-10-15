#include "calculator.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "Welcome to A Better Calculator by Artanisax(12110524)!\n"
            "Used for CS205-2022Fall Project 2\n"
            "(Input \"quit\" to quit)\n\n";
    calculator cal;
    string s1, s2;
    size_t len;
    ll exp;
    while (true) {
        // getline(cin, s1);
        cin >> s1 >> s2;
        number n1(s1), n2(s2);
        cout << cal.divide(n1, n2).to_s() << '\n' << cal.sqrt(n1).to_s() << ' ' << cal.sqrt(n2).to_s() << '\n';
    }
    return 0;
}
