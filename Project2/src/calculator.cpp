#include "calculator.hpp"

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <stack>

using namespace std;

calculator::calculator() {
    srand(time(NULL));
}

void calculator::print(const number &x) const {
    cout << x.to_s() << '\n';
}

// define or modify variables
void calculator::assign(string s) {
    size_t eq = s.find('=');
    variable.insert_or_assign(s.substr(0, eq),
                              calculate(s.substr(eq+1, s.length()-(eq+1))));
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
    if (a.exp+(int)a.digit.size() > b.exp+(int)b.digit.size()) return coe;
    if (a.exp+(int)a.digit.size() < b.exp+(int)b.digit.size()) return -coe;
    for (size_t ia = a.digit.size()-1, ib = b.digit.size()-1; ~ia && ~ib; --ia, --ib) {
        if (a.digit[ia] > b.digit[ib]) return coe;
        if (a.digit[ia] < b.digit[ib]) return -coe;
    }
    if (a.digit.size() > b.digit.size()) return coe;
    if (a.digit.size() < b.digit.size()) return -coe;
    return 0;
}

// addition and substraction
number calculator::add(const number &a, const number &b) const {
    number ret;
    if (a.negative == b.negative) {  // same sign, true addition
        ret.negative = a.negative;

        // initialize the length of the result
        int low = min(a.exp, b.exp),
            high = max(a.exp+(int)a.digit.size(), b.exp+(int)b.digit.size())-1;
        ret.exp = low;

        // digit addition
        size_t idx;
        for (int i = low; i <= high; ++i) {
            idx = i-low;
            if (ret.digit.size() == idx) ret.digit.push_back(0);
            ret.digit[idx] += ((i >= a.exp && i < a.exp+(int)a.digit.size()) ? a.digit[i-a.exp] : 0)+
                              ((i >= b.exp && i < b.exp+(int)b.digit.size()) ? b.digit[i-b.exp] : 0);
            if (ret.digit[idx] >= 10) {  // carry
                ret.digit[idx] -= 10;
                ret.digit.push_back(1);
            }
        }
    } else {  // oposite sign, substraction actually

        // make sure it's big - small
        number x = abs(a), y = abs(b);
        int comp = compare(x, y);
        if (!comp) return ret;
        if (!~comp) {  // abs(b) > abs(a)
            ret.copy(b);
            swap(x, y);
        } else ret.copy(a);  // abs(a) > abs(b)

        // digit substraction
        vector<short> temp;
        if (x.exp > y.exp) {
            for (int i = x.exp-y.exp; i > 0; --i) {
                temp.push_back(0);
                --ret.exp;
            }
            for (size_t i = 0; i < ret.digit.size(); ++i) temp.push_back(ret.digit[i]);
            swap(ret.digit, temp);
        } else if (y.exp > x.exp) {
            for (int i = y.exp-x.exp; i > 0; --i) temp.push_back(0);
            for (size_t i = 0; i < y.digit.size(); ++i) temp.push_back(y.digit[i]);
            swap(y.digit, temp);
        }
        for (size_t i = 0; i < y.digit.size(); ++i) ret.digit[i] -= y.digit[i];

        // borrow
        for (size_t i = 0; i < ret.digit.size(); ++i)
            if (ret.digit[i] < 0) {
                ret.digit[i] += 10;
                ret.digit[i+1] -= 1;
            }
    }
    ret.simplify();
    return ret;
}


// multiplication
number calculator::multiply(const number &a, const number &b) const {
    number ret;
    if (a.digit.empty() || b.digit.empty()) return ret;
    ret.negative = a.negative^b.negative;
    ret.exp = a.exp+b.exp;
    for (size_t i = a.digit.size()+b.digit.size(); ~i; --i) ret.digit.push_back(0);  // initiallize possible length
    for (size_t i = 0, idx; i < a.digit.size(); ++i) {
        if (!a.digit[i]) continue;
        for (size_t j = 0; j < b.digit.size(); ++j) {
            idx = i+j;
            ret.digit[idx] += a.digit[i]*b.digit[j];
            if (ret.digit[idx] >= 10) {  // carry
                ret.digit[idx+1] += ret.digit[idx]/10;
                ret.digit[idx] %= 10;
            }
        }
    }
    ret.simplify();
    return ret;
}

// division
number calculator::divide(const number &a, const number &b) const {
    number ret;

    // speacial cases
    if (b.digit.empty() || !~b.digit[0] || (!a.digit.empty() && !~a.digit[0])) {  // NaN
        ret.digit.push_back(-1);
        return ret;
    }
    if (a.digit.empty()) return ret;  // 0

    ret.negative = a.negative^b.negative;
    ret.exp = a.exp-b.exp+1;

    // initialize dividend x and divisor y
    number x, y;
    size_t ia = a.digit.size()-1;
    for (size_t ib = 0; ib < b.digit.size(); ++ib) {
        if (~ia) x.digit.push_back(a.digit[ia--]);
        else {
            ++x.exp;
            --ret.exp;
        }
        y.digit.push_back(b.digit[ib]);
    }
    reverse(x.digit.begin(), x.digit.end());
    x.simplify();

    // calculate each bit of quotient
    for (size_t i = 0, l, r, mid; i <= max(a.digit.size(), b.digit.size())+precision; ++i) {
        if ((int)x.digit.size()+x.exp < (int)y.digit.size()+y.exp || !~compare(x, y)) {  // skip 0
            ret.digit.push_back(0);
            if (~ia) x = add(multiply(x, number(10)), number(a.digit[ia--]));
            else {
                ++x.exp;
                --ret.exp;
            }
            x.simplify();
            continue;
        }
        l = 2, r = 9;
        while (l <= r) {  // try possible digits, with binary optimization
            mid = l+r>>1;
            if (!~compare(add(x, multiply(y, number(-mid))), number())) r = mid-1;
            else l = mid+1;
        }
        ret.digit.push_back(r);
        x = add(x, multiply(y, number(-r)));
        if (~ia) x = add(multiply(x, number(10)), number(a.digit[ia--]));
        else {
            ++x.exp;
            --ret.exp;
        }
        x.simplify();
    }
    reverse(ret.digit.begin(), ret.digit.end());
    ret.simplify();
    return ret;
}

// generate the absolute number of x
number calculator::abs(const number &x) const {
    number ret;
    ret.copy(x);
    ret.negative = false;
    return ret;
}

// generate the opposite number of x
number calculator::opp(const number &x) const {
    number ret;
    ret.copy(x);
    ret.negative = !x.negative;
    return ret;
}

// generate a random number
number calculator::random(size_t len, ll exp) const {
    number ret;
    while (len--) ret.digit.push_back(rand()%10);
    while (!ret.digit.back()) ret.digit.back() = rand()%10;  // make sure the most significant bit is not 0
    ret.exp = exp;
    ret.simplify();
    return ret;
}

void calculator::error() const {
    cout << "Syntax Error!\n";
}

number calculator::calculate(string s) const {
    number ret("0");
    return ret;
}
