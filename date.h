#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Date {
public:
	Date(int new_year, int new_month, int new_day);

	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;

private:
	int year, month, day;
};


bool operator<(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);


ostream& operator<<(ostream& stream, const Date& date);

Date ParseDate(istream& date);
