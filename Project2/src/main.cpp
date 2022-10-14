#include "calculator.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "Build successfully!" << endl;
    calculator cal;
    string s1, s2;
    size_t len;
    ll exp;
    while (true) {
        cin >> s1;
        cout << cal.sqrt(number(s1)).to_s() << '\n';
    }
    return 0;
}
