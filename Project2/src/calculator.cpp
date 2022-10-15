#include "calculator.hpp"

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <stack>

typedef pair<int, size_t> pit;

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
    if (a.error() || b.error()) return number("Error");

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
    if (a.error() || b.error()) return number("Error");

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
    if (a.error() || b.error()) return number("Error");
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
    short l, r, mid;
    for (size_t i = 0; i < max(a.digit.size(), b.digit.size())+precision; ++i) {
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
    if (x.error()) return number("Error");

    number ret;
    ret.copy(x);
    ret.negative = false;
    return ret;
}

// return the opposite number of x
number calculator::opp(const number &x) const {
    if (x.nan()) return number("NaN");
    if (x.error()) return number("Error");

    number ret;
    ret.copy(x);
    ret.negative = !x.negative;
    return ret;
}

// calculate the sqrt of x
number calculator::sqrt(const number &a) const {
    if (a.nan()) return number("NaN");
    if (a.error()) return number("Error");
    
    number x, ret, temp, eps(1);

    //minimize abs(exp) of x, add it up later
    ll exp = a.exp/2;
    x.copy(a);
    x.exp = a.exp%2;
    if (x.negative) {
        ret.digit.push_back(-1);
        return ret;
    }
    temp.copy(x);
    size_t limit = x.digit.size()+precision<<1;
    eps.exp = -precision<<1;
    int i = 0;
    while (compare(abs(add(ret, opp(temp))), eps) > 0) {  // Newton's method
        ret.copy(temp);
        temp = add(ret, opp(divide(add(multiply(ret, ret), opp(x)), multiply(ret, number(2)))));
        temp.limit_percision(limit);
    }
    ret.exp += exp;
    ret.limit_percision(x.digit.size()+precision);
    return ret;
}

// generate a random number
number calculator::random(size_t len, ll exp) const {
    number ret;
    while (len--) ret.digit.push_back(rand()%10);
    // make sure the MSB is not 0
    while (!ret.digit.back()) ret.digit.back() = rand()%10;
    ret.exp = exp;
    ret.simplify();
    return ret;
}

// define the priority of signs
int order(char c) {
    switch (c) {
        case '(':
        case ')':
            return 0;
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default: return -1;
    }
}

/*
 * return (data_type, length)
 * data_type:
 * -1: error
 *  0: number
 *  1: abs
 *  2: opp
 *  3: sqrt
 *  4: pow
 *  5: random
 */
pit get_a_data(const string &s, size_t begin) {
    if (isdigit(s[begin])) {  // a number without a sign
        size_t len = 1;
        while (begin+len < s.length() && isdigit(s[begin+len])) ++len;
        return make_pair(0, len);
    }
    switch (s[begin]) {  // signs or functions
        case '+':  // a number with a sign
        case '-':
            if (begin == s.length()-1 || !isdigit(s[begin+1]))
                return make_pair(-1, 0);
            else {
                size_t len = 2;
                while (begin+len < s.length() && isdigit(s[begin+len])) ++len;
                return make_pair(0, len);
            }
        case 'a':  // abs
            if (begin+2 <= s.length() && s.substr(begin, 3) == "abs")
                return make_pair(1, 3);
        case 'o':  // opp
            if (begin+2 <= s.length() && s.substr(begin, 3) == "opp")
                return make_pair(2, 3);
        case 's':  // sqrt
            if (begin+3 <= s.length() && s.substr(begin, 4) == "sqrt")
                return make_pair(3, 4);
        case 'p':  // pow
            if (begin+3 <= s.length() && s.substr(begin, 4) == "pow")
                return make_pair(4, 4);
        case 'r':  // random
            if (begin+5 <= s.length() && s.substr(begin, 6) == "random")
                return make_pair(5, 6);
    }
    return make_pair(-1, 0);
}

vector<string> split(string s, size_t begin, size_t len, char c) {
    vector<string> ret;
    
    return ret;
}

// calculate an expression
number calculator::calculate(const string &s) const {
    number ret;
    stack<char> op;
    stack<number> data;
    bool flag = true;
    for (size_t i = 0, len; i < s.length(); i += len, flag = !flag) {
        if (flag) { // expect a data or '('s
            while (s[i] == '(') {
                op.push('(');
                if (++i == s.length()) return number("Error");
            }
            pit temp = get_a_data(s, i);  // try to get a data
            int type = temp.first;
            len = temp.second;
            if (!~type) return number("Error");

            if (!type) {  // a simple number
                data.push(number(s.substr(i, len)));
                continue;
            }
            
            // functions
            switch (type) {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
            }
        } else {  // expect a operator or a ')'
            while (i < s.length() && s[i] == ')') {
                while (!op.empty() && op.top() != '(') {
                    data.push(number(op.top()));
                    op.pop();
                }
                if (op.empty()) return number("Error");
                else op.pop();
                ++i;
            }
            if (i == s.length()) break;
            if (!~order(s[i])) return number("Error");
            else {
                if (!op.empty() && order(op.top()) >= order(s[i])) {
                    data.push(number(op.top()));
                    op.pop();
                }
                op.push(s[i]);
            }
            len = 1;
        }
    }
    while (!op.empty()) {
        if (op.top() == '(') return number("Error");
        data.push(number(op.top()));
        op.pop();
    }
    while (!data.empty()) {
        cout << data.top().to_s();
        data.pop();
    }
    cout << '\n';
    if (!data.empty()) return number("Error");
    return ret;
}

// define or modify variables
void calculator::assign(const string &s, const number &x) {
    if (s == "precision") {  // modify precision setting
        if (x.nan() || x.error() || x.negative ||
            compare(x, number(__LONG_LONG_MAX__)) > 0) {
            cout << "Invalid precision\n";
            return;
        }
        precision = x.to_t();
    }
    variable.insert_or_assign(s, x);
}

/*
 * check the validity of the expression
 * classify expression to variable assignment or calculation
 */
string calculator::analyse(string &s) {
    // remove ' '
    if (count(s.begin(), s.end(), ' ')) {
        string temp;
        for (size_t i = 0; i < s.length(); ++i)
            if (s[i] != ' ') temp.push_back(s[i]);
        swap(s, temp);
    }
    cout << "s: " << s << '\n';
    // quit
    if (s == "quit") {
        cout << "\nThanks for using. Good bye!\n";
        exit(0);
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

    // calculate
    number ans = calculate(s);
    if (ans.error()) return "Syntax Error\n";
    return ans.to_s()+'\n';
}
