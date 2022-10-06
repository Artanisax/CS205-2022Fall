#include "calculator.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "Build successfully!" << endl;
    calculator cal;
    string s1, s2;
    while (true) {
      cin >> s1 >> s2;
        number n1(s1), n2(s2);
        n1.print();
        cout << ' ';
        n2.print();
        cout << endl;
        cout << cal.comp(n1, n2) << endl;
    }
    return 0;
}
