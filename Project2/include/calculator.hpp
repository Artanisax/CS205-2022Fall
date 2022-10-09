#pragma once

#include "number.hpp"

#include <map>

using namespace std;

struct calculator {
    map<string, number> string_to_number;

    void print(const number &number) const;

    void store(const string &s);

    int compare(const number &a, const number &b) const;

    number add(const number &a, const number &b) const;

    number minus(const number &a, const number &b) const;

    number multiply(const number &a, const number &b) const;

    number divide(const number &a, const number &b) const;

    number abs(const number &a) const;

    number calculate(const string &s) const;
};
