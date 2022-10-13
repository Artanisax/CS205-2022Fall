#include "number.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

number::number(const string &s) {
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

number::number() {}

number::number(const ll &x) {
    new (this) number(to_string(x));
}

void number::copy(const number &x) {
    negative = x.negative;
    exp = x.exp;
    digit.clear();
    for (size_t i = 0; i < x.digit.size(); ++i) digit.push_back(x.digit[i]);
}

/*
 * Delete prefix & postfix zeros
 * For efficiency, reload the whole vector instead of erasing iterators
 */
void number::simplify() {
    if (digit.empty()) {
        negative = false;
        exp = 0;
        return;
    }
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
string number::to_s() const {
    if (digit.empty()) return "0";  // 0 alone
    if (!~digit[0]) return "NaN";  // not a number
    string ret;
    if (negative) ret += "-";
    ret += to_string(digit[digit.size()-1]);
    if (exp >= 0) {  // positive exponent
        if (exp <= digit.size()) {  // use postfix 0
            for (int i = digit.size()-2; ~i; --i) ret += to_string(digit[i]);
            for (int i = 0; i < exp; ++i) ret += "0";
        } else {  // use scientific notation
            if (digit.size() > 1) {
                ret += ".";
                for (int i = digit.size()-2; ~i; --i) ret += to_string(digit[i]); 
            }
            ret += "e" + to_string(exp+(int)digit.size()-1);
        }
    }
    else {  // negative exponent
        if (-exp < digit.size()) {  // radix point in middle
            for (int i = digit.size()-2; i >= -exp; --i) ret += to_string(digit[i]);
            ret += ".";
            for (int i = -exp-1; ~i; --i) ret += to_string(digit[i]);
        } else {  // use scientific notation
            if (digit.size() > 1) {
                ret += ".";
                for (int i = digit.size()-2; ~i; --i) ret += to_string(digit[i]);
            }
            ret += "e" + to_string(exp+(int)digit.size()-1);
        }
    }
    return ret;
}
