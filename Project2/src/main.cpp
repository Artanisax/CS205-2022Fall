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
        cin >> len >> exp;
        cout << cal.random(len, exp).to_s() << '\n';
    }
    return 0;
}
