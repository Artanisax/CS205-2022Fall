#pragma once

#include "number.hpp"

#include <map>

typedef pair<int, size_t> pit;

struct calculator {
    size_t precision = 4;  // used in division & sqrt, 4 digit more by default
    const map<string, size_t> fun = {{"abs", 1}, {"opp", 1}, {"sqrt", 2},
                                     {"pow", 2}, {"random", 5}};
    map<string, number> var;

    calculator();

    void print(const number &number) const;

    int compare(const number &a, const number &b) const;

    number add(const number &a, const number &b) const;

    number multiply(const number &a, const number &b) const;

    number divide(const number &a, const number &b) const;

    number abs(const number &x) const;

    number opp(const number &ax) const;

    number sqrt(const number &x) const;
    
    number pow(const number &x, int t) const;

    number random(size_t len, ll exp) const;

    pit get_a_data(const string &s, size_t begin) const;

    number bin_calc(const number &a, const number &b, const char &c) const; 

    number calculate(const string &s) const;

    void assign(const string &s, const number &x);

    string analyse(string &s);

};
