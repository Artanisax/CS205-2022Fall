#pragma once

#include "typdef.hpp"

#include <vector>
#include <string>

using namespace std;

struct number {
	char op = 0;
	bool negative = false;
	ll exp = 0;
	vector<short> digit;  // use short to save memory

	number(const string &s);
	
	number();

	number(const char &c);

	number(const int &x);

	number(const ll &x);

	bool nan() const;

	bool error() const;

	void copy(const number &x);

	void simplify();

	void limit_percision(const size_t &limit);

	string to_s() const;

	size_t to_t() const;
};
