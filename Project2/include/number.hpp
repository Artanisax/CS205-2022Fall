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

	bool isnumber(const string &s) const;

	void simplify();

	void print() const;
};
