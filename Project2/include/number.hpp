#pragma once

#include <vector>
#include <string>

typedef long long ll;

using namespace std;

struct number {
	bool negative = false;
	ll exp = 0;
	vector<short> digit;  // use short to save memory

	number();

	number(const int &x);

	number(const ll &x);

	number(const string &s);

	void copy(const number &x);

	void simplify();

	void print() const;
};
