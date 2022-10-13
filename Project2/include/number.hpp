#pragma once

#include "typdef.hpp"

#include <vector>
#include <string>

using namespace std;

struct number {
	bool negative = false;
	ll exp = 0;
	vector<short> digit;  // use short to save memory

	number(const string &s);
	
	number();

	number(const ll &x);

	void copy(const number &x);

	void simplify();

	void print() const;
};
