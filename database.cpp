#include <algorithm>
#include <iterator>
#include <sstream>
#include "database.h"

Database :: Database() {}


void Database :: Add(const Date& date, const string& event) {
	auto result_of_insert = dbSet[date].insert(event);
	if (result_of_insert.second) {
		dbVector[date].push_back(event);
	}
}

void Database :: Print(ostream& os) const {
	for (const auto& item : dbVector) {
		for (const auto& i : item.second) {
			os << item.first << ' ' << i << endl;
		}
	}
}

string Database :: Last(const Date& date) const {
	ostringstream out;
	if (dbVector.empty()) {
		out << "No entries";
	}
	else if (date < dbVector.begin()->first) {
		out << "No entries";
	} else if (dbVector.count(date) == 1) {
		string event = dbVector.at(date)[dbVector.at(date).size() - 1];
		out << date << " " << event;
	} else {
		auto it = dbVector.upper_bound(date);
		--it;
		Date d = it->first;
		string event = it->second[it->second.size() - 1];
		out << d << " " << event;
	}
	return out.str();
}

