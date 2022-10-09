#include "calculator.hpp"

#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

void calculator::print(const number &x) const {
    x.print();
    cout << endl;
}

void calculator::store(const string &s) {
    size_t eq = s.find('=');
    string_to_number.insert(make_pair(s.substr(0, eq), number(s.substr(eq+1, s.length()-(eq+1)))));
}

/*
 * Compare two numbers
 * -1: a < b
 *  0: a = b
 *  1: a > b
 */
int calculator::compare(const number &a, const number &b) const {
    if (a.negative && !b.negative) return -1;
    if (!a.negative && b.negative) return 1;
    int coe = (a.negative ? -1 : 1);
    if (a.exp+a.digit.size() > b.exp+b.digit.size()) return coe;
    if (a.exp+a.digit.size() < b.exp+b.digit.size()) return -coe;
    for (size_t ia = a.digit.size()-1, ib = b.digit.size()-1; ~ia && ~ib; --ia, --ib) {
        if (a.digit[ia] > b.digit[ib]) return coe;
        if (a.digit[ia] < b.digit[ib]) return -coe;
    }
    if (a.digit.size() > b.digit.size()) return coe;
    if (a.digit.size() < b.digit.size()) return -coe;
    return 0;
}

number calculator::add(const number &a, const number &b) const {
    number ret;
    if (a.negative == b.negative) {  // same sign
        cout << "add:" <<endl;
        ret.negative = a.negative;
        int low = min(a.exp, b.exp),
            high = max(a.exp+(int)a.digit.size(), b.exp+(int)b.digit.size())-1;
        ret.exp = low;
        size_t idx;
        for (int i = low; i <= high; ++i) {
            idx = i-low;
            if (ret.digit.size() == idx) ret.digit.push_back(0);
            ret.digit[idx] += ((i >= a.exp && i < a.exp+(int)a.digit.size()) ? a.digit[i-a.exp] : 0)+
                              ((i >= b.exp && i < b.exp+(int)b.digit.size()) ? b.digit[i-b.exp] : 0);
            if (ret.digit[idx] >= 10) {
                ret.digit[idx] -= 10;
                ret.digit.push_back(1);
            }
        }
    } else {  // oposite sign
        number x = abs(a), y = abs(b);
        int comp = compare(a, b);
        if (!comp) return ret;
        if (!~comp) {  // abs(b) > abs(a)
            ret = b;
            swap(x, y);
        } else ret = a;  // abs(a) > abs(b)
        vector<short> temp;
        for (int i = x.exp-y.exp; i; --i) temp.push_back(0);
        for (size_t i = 0; i < ret.digit.size(); ++i) temp.push_back(ret.digit[i]);
        swap(ret.digit, temp);
        ret.exp = y.exp;
        for (size_t i = 0; i < y.digit.size(); ++i) ret.digit[i] -= y.digit[i];
        for (size_t i = 0; i < ret.digit.size(); ++i)
            if (ret.digit[i] < 0) {
                ret.digit[i] += 10;
                ret.digit[i+1] -= 1;
            }
    }
    ret.simplify();
    return ret;
}

// number calculator::minus(const number &a, const number &b) const {

// }

// number calculator::multiply(const number &a, const number &b) const {

// }

// number calculator::divide(const number &a, const number &b) const {

// }

number calculator::abs(const number &a) const {
    number ret = a;
    ret.negative = true;
    return ret;
}

number calculator::calculate(const string &s) const {
    number ret("0");
    return ret;
}
