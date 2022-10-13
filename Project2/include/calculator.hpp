#pragma once

#include "number.hpp"

#include <map>

using namespace std;

struct calculator {
    int precision = 4;  // used in division & sqrt, 4 digit more by default
    map<string, number> string_to_number;

    calculator();

    void print(const number &number) const;

    void store(const string &s);

    int compare(const number &a, const number &b) const;

    number add(const number &a, const number &b) const;

    number multiply(const number &a, const number &b) const;

    number divide(const number &a, const number &b) const;

    number abs(const number &x) const;

    number opp(const number &ax) const;

    number sqrt(const number &x) const;
    
    number pow(const number &x, int t) const;

    number random() const;

    number calculate(const string &s) const;
};
