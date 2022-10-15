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

/*
 * compare two numbers
 * -1: a < b
 *  0: a = b
 *  1: a > b
 */
int calculator::compare(const number &a, const number &b) const {
    // compare by sign
    if (a.negative && !b.negative) return -1;
    if (!a.negative && b.negative) return 1;

    // When a or b is 0
    if (a.digit.empty()) return (b.negative ? 1 : -1);
    if (b.digit.empty()) return (a.negative ? -1 : 1);

    // compare by size
    int coe = (a.negative ? -1 : 1);
    if (a.exp+(ll)a.digit.size() > b.exp+(ll)b.digit.size()) return coe;
    if (a.exp+(ll)a.digit.size() < b.exp+(ll)b.digit.size()) return -coe;

    // compare by each bit
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
    if (a.nan() || b.nan()) return number("NaN");
    number ret;
    if (a.negative == b.negative) {  // same sign, true addition
        ret.negative = a.negative;

        // initialize the length of the result
        int low = min(a.exp, b.exp),
            high = max(a.exp+(ll)a.digit.size(), b.exp+(ll)b.digit.size())-1;
        ret.exp = low;

        // digit addition
        size_t idx;
        for (int i = low; i <= high; ++i) {
            idx = i-low;
            if (ret.digit.size() == idx) ret.digit.push_back(0);
            ret.digit[idx] += ((i >= a.exp && i < a.exp+(ll)a.digit.size()) ? a.digit[i-a.exp] : 0)+
                              ((i >= b.exp && i < b.exp+(ll)b.digit.size()) ? b.digit[i-b.exp] : 0);
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
    if (a.nan() || b.nan()) return number("NaN");
    number ret;
    if (a.digit.empty() || b.digit.empty()) return ret;
    ret.negative = a.negative^b.negative;
    ret.exp = a.exp+b.exp;
    for (size_t i = a.digit.size()+b.digit.size(); ~i; --i)
        ret.digit.push_back(0);  // initiallize possible length
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
    if (a.nan() || b.nan()) return number("NaN");
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
    if (!~compare(x, y)) {  // make sure x >= y
        ret.digit.push_back(0);
        if (~ia) x = add(multiply(x, number(10)), number(a.digit[ia--]));
        else {
            ++x.exp;
            --ret.exp;
        }
        x.simplify();
    }

    // calculate each bit of quotient
    for (size_t i = 0, l, r, mid; i < max(a.digit.size(), b.digit.size())+precision; ++i) {
        if ((ll)x.digit.size()+x.exp < (ll)y.digit.size()+y.exp || !~compare(x, y)) {  // skip 0
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

// return the absolute number of x
number calculator::abs(const number &x) const {
    if (x.nan()) return number("NaN");

    number ret;
    ret.copy(x);
    ret.negative = false;
    return ret;
}

// return the opposite number of x
number calculator::opp(const number &x) const {
    if (x.nan()) return number("NaN");

    number ret;
    ret.copy(x);
    ret.negative = !x.negative;
    return ret;
}

// calculate the sqrt of x
number calculator::sqrt(const number &x) const {
    if (x.nan()) return number("NaN");
    
    number ret, temp, eps(1);
    if (x.negative) {
        ret.digit.push_back(-1);
        return ret;
    }
    eps.exp = x.exp-(precision<<1);
    temp.copy(x);
    size_t limit = x.digit.size()+precision<<1;
    while (compare(abs(add(ret, opp(temp))), eps) > 0) {  // Newton's method
        ret.copy(temp);
        temp = add(ret, opp(divide(add(multiply(ret, ret), opp(x)), multiply(ret, number(2)))));
        temp.limit_percision(limit);
    }
    ret.limit_percision(x.digit.size()+precision);
    return ret;
}

// generate a random number
number calculator::random(size_t len, ll exp) const {
    number ret;
    while (len--) ret.digit.push_back(rand()%10);
    while (!ret.digit.back()) ret.digit.back() = rand()%10;  // make sure the MSB is not 0
    ret.exp = exp;
    ret.simplify();
    return ret;
}

// calculate an expression
number calculator::calculate(string s) const {
    number ret;
    return ret;
}

// define or modify variables
void calculator::assign(const string &s, const number &x) {
    variable.insert_or_assign(s, x);
}

/*
 * check the validity of the expression
 * classify expression to variable assignment or calculation
 */
string calculator::analyse(string s) {
    // remove ' '
    if (count(s.begin(), s.end(), '=')) {
        string temp;
        for (size_t i = 0; i < s.length(); ++i)
            if (s[i] != ' ') temp.push_back(s[i]);
    }

    // assignment
    size_t cnt = count(s.begin(), s.end(), '=');
    if (cnt)
        if (cnt == 1) {
            size_t idx = s.find("=");
            string name = s.substr(0, idx);
            number value = calculate(s.substr(idx+1, s.length()-idx-1));
            if (value.error()) return "Syntax Error\n";
            assign(name, value);
            return "";
        } else return "Syntax Error!\n";
    s.find('=');

    // calculate
    number ans = calculate(s);
    if (ans.error()) return "Syntax Error\n";
    return ans.to_s()+'\n';
}
