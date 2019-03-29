#include "date.h"

#include <iomanip>

Date::Date(int new_year, int new_month, int new_day) {
	year = new_year;
	if (new_month > 12 || new_month < 1) {
		throw logic_error("Month value is invalid: " + to_string(new_month));
	}
	month = new_month;
	if (new_day > 31 || new_day < 1) {
		throw logic_error("Day value is invalid: " + to_string(new_day));
	}
	day = new_day;
}


int Date::GetYear() const {
	return year;
}

int Date::GetMonth() const {
	return month;
}

int Date::GetDay() const {
	return day;
}

ostream& operator<<(ostream& stream, const Date& date) {
	stream << setw(4) << setfill('0') << date.GetYear() <<
		"-" << setw(2) << setfill('0') << date.GetMonth() <<
		"-" << setw(2) << setfill('0') << date.GetDay();
	return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
		vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator<=(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <=
		vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator>(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >
		vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator>=(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >=
		vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator==(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} ==
		vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator!=(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} !=
		vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

Date ParseDate(istream& is) { // парсинг даты, и возвращение в правильном виде
	int year, month, day;
	is >> year;
	is.ignore(1);
	is >> month;
	is.ignore(1);
	is >> day;
	return Date(year, month, day);
}
