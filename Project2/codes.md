Please See [My GitHub Repository](https://github.com/Artanisax/CS205-2022Fall/tree/main/Project2)

(Note: just in case, all the codes are here but might not that readable because PDF's column limit)

```c++
// typedef.hpp

#pragma once

typedef long long ll;
typedef unsigned long long ull;
```

```c++
// number.hpp

#pragma once

#include "typdef.hpp"

#include <vector>
#include <string>

using std::vector;
using std::string;

struct number {
	char op = 0;
	bool negative = false;
	ll exp = 0;
	vector<short> digit;  // use short to save memory

	number(const string &s);
	
	number();

	number(const char &c);

	number(const short &x);

	number(const int &x);

	number(const ll &x);

	bool nan() const;

	bool error() const;

	void copy(const number &x);

	void simplify();

	void limit_precision(const size_t &limit);

	string to_s() const;

	size_t to_t() const;
};
```

```c++
// calculator.hpp

#pragma once

#include "number.hpp"

#include <map>

using std::map;
using std::pair;

typedef pair<int, size_t> pit;

struct calculator {
    size_t precision = 4;  // used in division & sqrt, 4 digit more by default
    const map<string, size_t> fun = {{"abs", 1}, {"opp", 1}, {"sqrt", 1},
                                     {"pow", 2}, {"random", 0}};
    map<string, number> var;

    calculator();

    int compare(const number &a, const number &b) const;

    number add(const number &a, const number &b) const;

    number multiply(const number &a, const number &b) const;

    number divide(const number &a, const number &b) const;

    number abs(const number &x) const;

    number opp(const number &ax) const;

    number sqrt(const number &x) const;
    
    number pow(const number &ax, const number &t) const;

    number random() const;

    pit get_a_data(const string &s, size_t begin) const;

    number bin_calc(const number &a, const number &b, const char &c) const;

    number fun_calc(const string &name, const vector<string> &parameter) const;

    number calculate(const string &s) const;

    void assign(const string &s, const number &x);

    string analyse(string &s);

};
```

```c++
// number.cpp

#include "number.hpp"

#include <iostream>
#include <algorithm>

using std::to_string;

number::number() {}

number::number(const char &c) {
    op = c;
}

number::number(const short &x) {
    new (this) number(to_string(x));
}

number::number(const int &x) {
    new (this) number(to_string(x));
}

number::number(const ll &x) {
    new (this) number(to_string(x));
}

number::number(const string &s) {
    if (s == "NaN" || s == "Error") {
        digit.push_back(s == "NaN" ? -1 : -2);
        return;
    }
    if (s[0] == '-') negative = true;
    size_t dot = -1, e = -1, ms = -1;
    ll temp = 0;
    for (size_t i = (s[0] == '-' || s[0] == '+'); i < s.length(); i++)
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

bool number::nan() const {
    return !digit.empty() && digit[0] == -1;
}

bool number::error() const {
    return !digit.empty() && digit[0] == -2;
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

// limit the percision of the number
void number::limit_precision(const size_t &limit) {
    reverse(digit.begin(), digit.end());
    while (digit.size() > limit) {  // limit percision
        digit.pop_back();
        ++exp;
    }
    reverse(digit.begin(), digit.end());
    simplify();
}

// convert number to string for output
string number::to_s() const {
    if (digit.empty()) return "0";  // 0 alone
    if (digit[0] == -2) return "Syntax Error!";
    if (!~digit[0]) return "NaN";  // not a number
    string ret;
    if (op) return ret += op;
    if (negative) ret += "-";
    if (-exp == digit.size()) {  // 0.
        ret += "0.";
        for (int i = digit.size()-1; ~i; --i) ret += to_string(digit[i]);
    } else {
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
                ret += "e"+to_string(exp+(int)digit.size()-1);
            }
        }
    }
    return ret;
}

// convert number to size_t for percision
size_t number::to_t() const {
    size_t ret = 0;
    for (size_t i = digit.size()-1; ~i; --i) ret = ret*10+digit[i];
    for (size_t i = 0; i < exp; ++i) ret *= 10;
    return ret;
}
```

```c++
// calculator.cpp

#include "calculator.hpp"

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <stack>

using std::min;
using std::max;
using std::swap;
using std::make_pair;
using std::stack;
using std::cout;
using std::endl;

// initiallize random seed and functions' (name, parameters)
calculator::calculator() {
    srand(time(NULL));
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
    if (a.digit.empty() && b.digit.empty()) return 0;
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
    if (a.error() || b.error()) return number("Error");
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
    if (a.error() || b.error()) return number("Error");
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
    if (a.error() || b.error()) return number("Error");
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
    if (x.error()) return number("Error");
    if (x.nan()) return number("NaN");

    number ret;
    ret.copy(x);
    ret.negative = false;
    return ret;
}

// return the opposite number of x
number calculator::opp(const number &x) const {
    if (x.error()) return number("Error");
    if (x.nan()) return number("NaN");

    number ret;
    ret.copy(x);
    ret.negative = !x.negative;
    return ret;
}

// calculate the sqrt of x
number calculator::sqrt(const number &a) const {
    if (a.error()) return number("Error");
    if (a.nan()) return number("NaN");
    
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
        temp = divide(add(multiply(ret, ret), x), multiply(ret, number(2)));
        temp.limit_precision(limit);
    }
    ret.exp += exp;
    ret.limit_precision(x.digit.size()+precision);
    return ret;
}

// calculate the N-th pow of x
number calculator::pow(const number &x, const number &y) const {
    // check the validity of y
    if (x.error() || y.error() || y.negative || y.exp < 0 ||
        compare(y, number(__LONG_LONG_MAX__)) > 0)
        return number("Error");
    if (x.nan() || y.nan()) return number("NaN");
    
    size_t t = y.to_t();  // convert y into integer
    number ret = number(1), temp;
    temp.copy(x);
    while (t) {  // binary accelerate
        if (t&1) ret = multiply(ret, temp);
        temp = multiply(temp, temp);
        t >>= 1;
    }
    return ret;
}

// generate a random number
number calculator::random() const {
    number ret;
    for (int i = 0; i <= precision; ++i) ret.digit.push_back(rand()%10);
    // make sure the MSB is not 0
    while (!ret.digit.back()) ret.digit.back() = rand()%10;
    ll exp = (rand()&1 ? rand() : -rand());
    ret.exp = exp%precision-(exp < 0)*precision;
    ret.negative = rand()&1;
    ret.simplify();
    return ret;
}

// check whether c can be used in a variable name
bool validch(char c) {
    return isdigit(c) || isalpha(c) || c == '_';
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
 *  1: functions
 */
pit calculator::get_a_data(const string &s, size_t begin) const {
    size_t len = 0;
    if (isdigit(s[begin])) {
        bool dot = false;  // a flag for decimal
        while (begin+len < s.length() &&
                (isdigit(s[begin+len]) || s[begin+len] == '.')) {
            if (s[begin+len] == '.') {
                if (dot) return make_pair(-1, 0);
                else dot = true;
            }
            ++len;
        }
        return make_pair(0, len);
    } else {  // a variable or a function
        while (begin+len < s.length() && validch(s[begin+len])) ++len;
        string name = s.substr(begin, len);
        if (var.count(name)) return make_pair(0, len);
        if (fun.count(name)) return make_pair(1, len);
    }
    return make_pair(-1, 0);
}

// for "fun(s)", split s into seperate parameters
vector<string> split(const string &s) {
    vector<string> ret;
    size_t cnt = 0, begin = 0;
    for (size_t i = 0; i < s.length(); ++i) {
        cnt += (s[i] == '(')-(s[i]==')');
        if (!cnt && s[i] == ',') {  // split using ','
            ret.push_back(s.substr(begin, i-begin));
            begin = i+1;
        }
    }
    // add the final substring or make it as 
    if(!s.empty()) ret.push_back(s.substr(begin, s.length()-begin));
    return ret;
}

// calculate two numbers
number calculator::bin_calc(const number &a, const number &b, const char &c) const {
    switch (c) {
        case '+': return add(a, b);
        case '-': return add(a, opp(b));
        case '*': return multiply(a, b);
        case '/': return divide(a, b);
        default: return number("Error");
    }
}

number calculator::fun_calc(const string &name, const vector<string> &parameter) const {
    if (parameter.size() != fun.find(name)->second) return number("Error");
    if (name == "abs") return abs(calculate(parameter[0]));
    if (name == "opp") return opp(calculate(parameter[0]));
    if (name == "sqrt") return sqrt(calculate(parameter[0]));
    if (name == "pow") return pow(calculate(parameter[0]), calculate(parameter[1]));
    if (name == "random") return random();
    return number("Error");
}

// calculate an expression
number calculator::calculate(const string &s) const {
    stack<char> op;
    stack<number> data;
    bool flag = true;
    for (size_t i = 0, len; i < s.length(); i += len, flag = !flag) {
        if (flag) { // expect a data or '('s
            while (s[i] == '(') {
                op.push('(');
                if (++i == s.length()) return number("Error");
            }
            bool sign = (s[i] == '+' || s[i] == '-');  // data start with a sign
            pit temp = get_a_data(s, i+sign);  // try to get a data (type, len)
            int type = temp.first;
            len = temp.second;
            string sub;
            switch (type) {
                case -1: return number("Error");
                case 0:  // a number
                    len += sign;
                    sub = s.substr(i, len);
                    if (isdigit(sub[0]) || sub[0] == '+' || sub[0] == '-')
                        data.push(number(sub));
                    else data.push(var.find(sub)->second);
                    break;
                case 1:  // a function
                    i += sign;
                    if (i+len == s.length() || s[i+len] != '(') return number("Error");
                    size_t cnt = 1, j;
                    for (j = i+len+1; j < s.length() && cnt; ++j)  // find the paired ')'
                        cnt += (s[j] == '(')-(s[j]==')');
                    if (cnt) return number("Error");
                    // split parameters and calculate the function
                    number result = fun_calc(s.substr(i, len), split(s.substr(i+len+1, j-i-len-2)));
                    if (result.error()) return number("Error");
                    data.push((sign && s[i-1] == '-') ? opp(result) : result);
                    len = j-i;
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

    while (!op.empty()) {  // convert into reversed postfix expression
        if (op.top() == '(') return number("Error");
        data.push(number(op.top()));
        op.pop();
    }

    // cnt(operator) must be cnt(number)-1 in postfix expression
    if (!(data.size()&1)) return number("Error");

    // calculate postfix expression
    stack<number> temp;
    while (!data.empty()) {  // generate a postfix expression
        temp.push(data.top());
        data.pop();
    }
    number cur, a, b;
    while (!temp.empty()) {  // calculate the postfix expression
        cur = temp.top();
        if (cur.op) {
            b = data.top();
            data.pop();
            a = data.top();
            data.pop();
            data.push(bin_calc(a, b, cur.op));
        } else data.push(cur);
        temp.pop();
    }
    return data.top();
}

// define or modify variables
void calculator::assign(const string &s, const number &x) {
    if (s == "precision") {  // modify precision setting
        if (x.nan() || x.error() || x.negative || x.exp < 0 ||
            compare(x, number(__LONG_LONG_MAX__)) > 0) {
            cout << "Invalid precision";
            return;
        }
        precision = x.to_t();
        cout << "Precision is set to " << x.to_t();
        return;
    }
    if (fun.count(s)) {
        cout << "Can not define names of functions as variables";
        return;
    }
    if (isdigit(s[0])) {
        cout << "Variable names can not start with a number";
        return;
    }
    for (int i = 0; i < s.length(); ++i)
        if (!validch(s[i])) {
            cout << "Unsupported character(s) for variable names";
            return;
        }
    var.insert_or_assign(s, x);
    cout << "Assign " << x.to_s() <<" to " << s;
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

    // quit
    if (s == "quit") {
        cout << "\nThanks for using. Good bye!\n" << endl;
        exit(0);
    }

    // assignment
    size_t cnt = count(s.begin(), s.end(), '=');
    if (cnt)
        if (cnt == 1) {
            size_t idx = s.find("=");
            string name = s.substr(0, idx);
            number value = calculate(s.substr(idx+1, s.length()-idx-1));
            if (value.error()) return "Syntax Error!";
            assign(name, value);
            return "";
        } else return "Syntax Error!";

    // calculate
    number ans = calculate(s);
    if (ans.error()) return "Syntax Error!";
    return ans.to_s();
}
```

```c++
// main.cpp

#include "calculator.hpp"

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
    cout << "\nWelcome to A Better Calculator by Artanisax(12110524)!\n"
            "Used for CS205-2022Fall Project 2\n"
            "(Input \"quit\" to quit)\n" << endl;
    calculator cal;
    string s, expression;
    while (true) {
        getline(cin, s);
        if (s.empty()) continue;
        expression += s;
        if (s.back() == '\\') expression.pop_back();  // new line
        else {
            cout <<cal.analyse(expression) << '\n' << endl;
            expression.clear();
        }
    }
    return 0;
}
```

```cmake
# CMakeList.txt

cmake_minimum_required(VERSION 3.10)

project(Calculator)

aux_source_directory(./src DIR_SRCS)

include_directories(include)

add_executable(Calculator ${DIR_SRCS})
```
