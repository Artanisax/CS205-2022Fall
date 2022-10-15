#pragma once

#include "number.hpp"

#include <map>

using namespace std;

struct calculator {
    ll precision = 4;  // used in division & sqrt, 4 digit more by default
    map<string, number> variable;

    calculator();

    void print(const number &number) const;

    void assign(string &s, number &x);

    int compare(const number &a, const number &b) const;

    number add(const number &a, const number &b) const;

    number multiply(const number &a, const number &b) const;

    number divide(const number &a, const number &b) const;

    number abs(const number &x) const;

    number opp(const number &ax) const;

    number sqrt(const number &x) const;
    
    number pow(const number &x, int t) const;

    number random(size_t len, ll exp) const;

    void error() const;

    number calculate(string s) const;
};
