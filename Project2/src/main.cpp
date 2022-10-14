#include "calculator.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "Build successfully!" << endl;
    calculator cal;
    string s1, s2;
    size_t len;
    ll exp;
    // cal.precision = 10;
    while (true) {
        cin >> s1;
        number n1(s1);
        cout << cal.sqrt(n1).to_s() << '\n';
    }
    return 0;
}
