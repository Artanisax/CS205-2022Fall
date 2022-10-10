#include "number.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

number::number() {
    number("");
}

number::number(const string &s) {
    negative = false;
    exp = 0;
    if (s[0] == '-') negative = true;
    int dot = -1, e = -1, ms = -1, temp = 0;
    for (size_t i = (s[0] == '-'); i < s.length(); i++)
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

void number::copy(const number &x) {
    negative = x.negative;
    exp = x.exp;
    for (size_t i = x.digit.size()-1; ~i; --i) digit.push_back(x.digit[i]);
}

/*
 * Delete prefix & postfix zeros
 * For efficiency, reload the whole vector instead of erasing iterators
 */
void number::simplify() {
    int tail = 0, head = digit.size()-1;
    while (tail < digit.size() && !digit[tail]) ++tail;
    while (~head && !digit[head]) --head;
    if (!tail && head == digit.size()-1) return;
    if (head < tail) {
        digit.clear();
        negative = false;
        exp = 0;
    } else {
        vector<short> temp;
        for (size_t i = tail; i <= head; ++i) temp.push_back(digit[i]);
        exp += tail;
        swap(digit, temp);
    }
}

// Make numbers look more friendly ~qwq~
void number::print() const {
    if (digit.empty()) { // 0 alone
        cout << 0;
        return;
    }
    if (!~digit[0]) {
        cout << "Nan";
        return;
    }
    if (negative) cout << '-';
    cout << digit[digit.size()-1];
    // if (digit.size() == 1) {  // only one precise digit
    //     if (exp) cout << 'e' << exp;
    //     return;
    // }
    if (exp >= 0) {  // positive exponent
        if (exp <= digit.size()) {  // use postfix 0
            for (int i = digit.size()-2; ~i; --i) cout << digit[i];
            for (int i = 0; i < exp; ++i) cout << 0;
        } else {  // use scientific notation
            if (digit.size() > 1) {
               cout << '.';
                for (int i = digit.size()-2; ~i; --i) cout << digit[i]; 
            }
            cout << 'e' << exp+(int)digit.size()-1;
        }
    }
    else {  // negative exponent
        if (-exp < digit.size()) {  // radix point in middle
            for (int i = digit.size()-2; i >= -exp; --i) cout << digit[i];
            cout << '.';
            for (int i = -exp-1; ~i; --i) cout << digit[i];
        } else {  // use scientific notation
            if (digit.size() > 1) {
               cout << '.';
                for (int i = digit.size()-2; ~i; --i) cout << digit[i]; 
            }
            cout << 'e' << exp+(int)digit.size()-1;
        }
    }
}
