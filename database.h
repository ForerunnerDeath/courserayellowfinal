#pragma once

#include "date.h"

#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

class Database {
public:

	Database();
	void Add(const Date& date, const string& event);
	void Print(ostream& os) const;
	string Last(const Date& date) const;


	template<typename Predicate>
		int RemoveIf(Predicate condition) {

		unsigned deleted_events = 0;
		auto order_it = dbVector.begin();
		for (auto stor_it = dbSet.begin(); stor_it != dbSet.end();) {
			set<string>& set_events = stor_it->second;
			unsigned init_size = set_events.size();
			unsigned deleted_events_in_date = 0;
			for (auto it = set_events.begin(); it != set_events.end();) {
				if (condition(stor_it->first, *it)) {
					it = set_events.erase(it);
					deleted_events_in_date++;
				} else {
					++it;
				}
			}
			deleted_events += deleted_events_in_date;
			if (deleted_events_in_date == init_size) {
				order_it = dbVector.erase(order_it);
				stor_it = dbSet.erase(stor_it);
			}
			else {
				vector<string>& events = order_it->second;
				auto predicate_in_set =
						[set_events](const string& event) {return set_events.find(event) == set_events.end();};
				auto new_end_of_vector = remove_if(events.begin(), events.end(), [predicate_in_set](const string& event) {return predicate_in_set(event);});
				events.erase(new_end_of_vector, events.end());
				++stor_it;
				++order_it;
			}

		}

		return deleted_events;
	}

	//template<typename Func>
	vector<string> FindIf(function<bool(Date, string)> pred) const  {
		vector<string> found;
		for (const auto& entry : dbVector) {
			for (const auto& event : entry.second) {
				if (pred(entry.first, event)) {
					ostringstream data;
					data << entry.first;
					found.push_back(data.str() + " " + event);
				}
			}
		}
		return found;
	}

private:
	map<Date, vector<string>> dbVector;
	map<Date, set<string>> dbSet;
};
