#include "calculator.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "Welcome to A Better Calculator by Artanisax(12110524)!\nUsed for CS205-2022Fall Project 2\n";
    calculator cal;
    string s1, s2;
    size_t len;
    ll exp;
    // cal.precision = 10;
    while (true) {
        cin >> s1 >> s2;
        number num(s2);
        cal.assign(s1, num);
        cout << cal.variable[s1].to_s() << '\n';
    }
    return 0;
}
