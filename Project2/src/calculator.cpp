#include "calculator.hpp"

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

number::number(const string &s) {
    negative = false;
    exp = 0;
    if (s[0] == '-') negative = true;
    int dot = -1, e = -1, ms = -1, temp = 0;
    for (int i = (s[0] == '-'); i < s.length(); i++)
        switch (s[i]) {
            case '.':
                dot = i;
                break;
            case 'E':
            case 'e':
                e = i;
                break;
            case '-':
                ms = i;
                break;
            default:
                if (~e) temp = temp*10+(s[i]-'0');
                else {
                    digit.push_back(s[i]-'0');
                    if (~dot) --exp;
                }
                break;
        }
    if (~ms) temp *= -1;
    exp += temp;
    reverse(digit.begin(), digit.end());
    simplify();
}

void number::simplify() {  // delete prefix & postfix zeros, for efficiency, reloading the whole vector instead of erasing iterators
    vector<short> temp;
    vector<short>::iterator front = digit.begin(), back = digit.end()-1;
    while (front < digit.end() && !*front) ++front;
    while (back >= digit.begin() && !*back) --back;
    if (back < front) {
        digit.clear();
        negative = false;
        exp = 0;
    } else {
        for (vector<short>::iterator it = front; it <= back; ++it) temp.push_back(*it);
        exp += distance(digit.begin(), front);
        swap(digit, temp);
    }
}

void number::print() const {  // just code to make numbers looked more friendly ~qwq~
    if (digit.empty()) { // 0 alone
        cout << 0;
        return;
    }
    if (negative) cout << '-';
    cout << digit[digit.size()-1];
    if (digit.size() == 1) {  // only one precise digit
        if (exp) cout << 'e' << exp;
        return;
    }
    if (exp >= 0) {  // positive exponent
        if (exp < (digit.size()>>1)) {  // use postfix 0
            for (int i = digit.size()-2; ~i; --i) cout << digit[i];
            for (int i = 0; i < exp; ++i) cout << 0;
        } else {  // use scientific notation
            cout << '.';
            for (int i = digit.size()-2; ~i; --i) cout << digit[i];
            cout << 'e' << exp+(int)digit.size()-1;
        }
    }
    else {  // negative exponent
        if (-exp < digit.size()) {  // radix point in middle
            for (int i = digit.size()-2; i >= -exp; --i) cout << digit[i];
            cout << '.';
            for (int i = -exp-1; ~i; --i) cout << digit[i];
        } else {  // use scientific notation
            cout << '.';
            for (int i = digit.size()-2; ~i; --i) cout << digit[i];
            cout << 'e' << exp+(int)digit.size()-1;
        }
    }
}

void calculator::print(const number &x) const {
    x.print();
    cout << endl;
}

bool calculator::isnumber(const string &s) const {
    return true;
}

void calculator::store(const string &s) {
    size_t eq = s.find('=');
    string_to_number.insert(make_pair(s.substr(0, eq), number(s.substr(eq+1, s.length()-(eq+1)))));
}

// number calculator::add(const number &a, const number &b) const {

// }

// number calculator::minus(const number &a, const number &b) const {

// }

// number calculator::multiply(const number &a, const number &b) const {

// }

// number calculator::divide(const number &a, const number &b) const {

// }

number calculator::calculate(const string &s) const {
    number ret("0");
    return ret;
}