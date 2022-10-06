#include "calculator.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "Build successfully!" << endl;
    calculator cal;
    string s;
    cin >> s;
    number temp(s);
    temp.print();
    cout << endl;
    return 0;
}