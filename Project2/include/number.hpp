#pragma once

#include <vector>
#include <string>

using namespace std;

struct number {
	bool negative;
	int exp;
	vector<short> digit;  // use short to save memory

	number();

	number(const string &s);

	void copy(const number &x);

	void simplify();

	void print() const;
};
