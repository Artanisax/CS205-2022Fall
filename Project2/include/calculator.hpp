#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

struct number {
    bool negative;
    int exp;
    vector<short> digit;  // use short to save memory

    number(const string &s);

    void simplify();

    void print() const;
};

struct calculator {
    map<string, number> string_to_number;

    void print(const number &number) const;

    bool isnumber(const string &s) const;

    void store(const string &s);

    number add(const number &a, const number &b) const;

    number minus(const number &a, const number &b) const;

    number multiply(const number &a, const number &b) const;

    number divide(const number &a, const number &b) const;

    number calculate(const string &s) const;
};