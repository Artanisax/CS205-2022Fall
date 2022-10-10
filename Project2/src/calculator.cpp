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

number calculator::add(const number &a, const number &b) const {
    number ret;
    if (a.negative == b.negative) {  // same sign
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
        int comp = compare(x, y);
        if (!comp) return ret;
        if (!~comp) {  // abs(b) > abs(a)
            ret.copy(b);
            swap(x, y);
        } else ret.copy(a);  // abs(a) > abs(b)
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

number calculator::multiply(const number &a, const number &b) const {
    number ret;
    if (a.digit.empty() || b.digit.empty()) return ret;
    ret.negative = a.negative^b.negative;
    ret.exp = a.exp+b.exp;
    for (int i = a.digit.size()+b.digit.size(); ~i; --i) ret.digit.push_back(0);
    for (size_t i = 0, idx; i < a.digit.size(); ++i) {
        if (!a.digit[i]) continue;
        for (size_t j = 0; j < b.digit.size(); ++j) {
            idx = i+j;
            ret.digit[idx] += a.digit[i]*b.digit[j];
            if (ret.digit[idx] >= 10) {
                ret.digit[idx] -= 10;
                ret.digit[idx+1] += 1;
            }
        }
    }
    ret.simplify();
    return ret;
}

number calculator::divide(const number &a, const number &b) const {
    number ret;
    if (b.digit.empty() || !~b.digit[0]) {
        ret.digit.push_back(-1);
        return ret;
    } 
    ret.negative = a.negative^b.negative;
    ret.exp = a.exp-b.exp;
    number x, y;
    x.negative = false, y.negative = true;
    size_t ia = a.digit.size()-1;
    for (size_t ib = b.digit.size()-1; ~ib; --ib) {
        if (~ia) x.digit.push_back(a.digit[ia--]);
        else {
            ++x.exp;
            --ret.exp;
        }
        y.digit.push_back(b.digit[ib]);
    }
    if (!~compare(x, y))
        if (~ia) x.digit.push_back(a.digit[ia--]);
        else {
            ++x.exp;
            --ret.exp;
        }
    for (size_t i = 0, l, r, mid; i <= max(a.digit.size(), b.digit.size())+precision; ++i) {
        if (x.digit.size()+x.exp < y.digit.size()+y.exp || !~compare(x, y)) {// skip 0
            ret.digit.push_back(0);
            if (~ia) x.digit.push_back(a.digit[ia--]);
            else {
                ++x.exp;
                --ret.exp;
            }
        }
        l = 2;
        r = (x.digit.size() == y.digit.size() ? x.digit.back()/y.digit.back() : 9);
        while (l <= r) {
            mid = l+r>>1;
            if (!~compare(add(x, multiply(y, number(to_string(mid)))), number(""))) r = mid-1;
            else l = mid+1;
        }
        ret.digit.push_back(r);
    }
    reverse(ret.digit.begin(), ret.digit.end());
    ret.simplify();
    return ret;
}

number calculator::abs(const number &x) const {
    number ret;
    ret.copy(x);
    ret.negative = false;
    return ret;
}

number calculator::opp(const number &x) const {
    number ret;
    ret.copy(x);
    ret.negative = !x.negative;
    return ret;
}

number calculator::calculate(const string &s) const {
    number ret("0");
    return ret;
}
