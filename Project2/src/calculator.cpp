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
int calculator::comp(const number &a, const number &b) const {
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
    if (a.negative == b.negative) {

    } else {

    }
}

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
