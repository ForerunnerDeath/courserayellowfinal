#include "date.h"
#include "database.h"
#include "node.h"
#include "condition_parser.h"
#include "test_runner.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

string ParseEvent(istream& is) {
	while (is.peek() == ' ') {
		is.ignore(1);
	}
	string event;
	getline(is, event);
	return event;
}

void TestAll();

int main() {
	TestAll();

	Database db;

	for (string line; getline(cin, line); ) {
		istringstream is(line);

		string command;
		is >> command;
		if (command == "Add") {
			const auto date = ParseDate(is);
			const auto event = ParseEvent(is);
			db.Add(date, event);
		}
		else if (command == "Print") {
			db.Print(cout);
		}
		else if (command == "Del") {
			auto condition = ParseCondition(is);
			auto predicate = [condition](const Date& date, const string& event) {
				return condition->Evaluate(date, event);
			};
			int count = db.RemoveIf(predicate);
			cout << "Removed " << count << " entries" << endl;
		}
		else if (command == "Find") {
			auto condition = ParseCondition(is);
			auto predicate = [condition](const Date& date, const string& event) {
				return condition->Evaluate(date, event);
			};

			const auto entries = db.FindIf(predicate);
			for (const auto& entry : entries) {
				cout << entry << endl;
			}
			cout << "Found " << entries.size() << " entries" << endl;
		}
		else if (command == "Last") {
			try {
				cout << db.Last(ParseDate(is)) << endl;
			}
			catch (invalid_argument&) {
				cout << "No entries" << endl;
			}
		}
		else if (command.empty()) {
			continue;
		}
		else {
			throw logic_error("Unknown command: " + command);
		}
	}


	system("pause");
	return 0;
}

void TestParseEvent() {
	{
		istringstream is("event");
		AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
	}
	{
		istringstream is("   sport event ");
		AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
	}
	{
		istringstream is("  first event  \n  second event");
		vector<string> events;
		events.push_back(ParseEvent(is));
		events.push_back(ParseEvent(is));
		AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
	}
}

void TestDatabaseLast() {
	{
		Database db;

		db.Add({ 2017, 11, 21 }, "Tuesday");
		db.Add({ 2017, 11, 20 }, "Monday");
		db.Add({ 2017, 11, 21 }, "Weekly meeting");

		db.Last({ 2017, 11, 21 });
		AssertEqual(db.Last({ 2017, 11, 20 }), "2017-11-20 Monday", "last case 1");
		AssertEqual(db.Last({ 2017, 11, 21 }), "2017-11-21 Weekly meeting", "last case 2");

		auto predicate = [](const Date& date, const string& event) {
			return event == "Weekly meeting";
		};
		db.RemoveIf(predicate);
		AssertEqual(db.Last({ 2017, 11, 21 }), "2017-11-21 Tuesday" , "last case 4");
		AssertEqual(db.Last({ 9999, 12, 31 }), "2017-11-21 Tuesday", "last case 5");

		db.Add({ 2017, 11, 21 }, "Weekly meeting");

		AssertEqual(db.Last({ 2017, 11, 21 }), "2017-11-21 Weekly meeting" , "last case 6");
		AssertEqual(db.Last({ 9999, 12, 31 }), "2017-11-21 Weekly meeting", "last case 7");

	}
}

void TestDatabasePrint() {
  {
	Database db;
	db.Add({ 2017,1,1 }, "Holiday");
	db.Add({ 2017,1,1 }, "Holiday");
	db.Add({ 2017,1,1 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,1,1 }, "New Year");
	db.Add({ 2017,1,1 }, "New Year");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,3,8 }, "Holiday");
	db.Add({ 2017,1,1 }, "Holiday");
	db.Add({ 2017,1,1 }, "Holiday");
	db.Add({ 2017,1,1 }, "Holiday");

	ostringstream out;
	db.Print(out);

	AssertEqual(out.str(), "2017-01-01 Holiday\n2017-01-01 New Year\n2017-03-08 Holiday\n", "case 1");

  }
  {
	Database db;
	ostringstream out;
	db.Print(out);

	AssertEqual(out.str(), "", "case 2");
  }
}

void TestDatabaseFind() {
	{
		Database db;

		db.Add({ 2017, 6, 1 }, "work");
		db.Add({ 2017, 6, 1 }, "sleep");
		db.Add({ 2017, 6, 2 }, "work");
		db.Add({ 2017, 6, 2 }, "sleep");
		db.Add({ 2017, 6, 3 }, "work");
		db.Add({ 2017, 6, 3 }, "sleep");
		db.Add({ 2017, 6, 4 }, "work");
		db.Add({ 2017, 6, 4 }, "sleep");
		db.Add({ 2017, 6, 5 }, "play computer games");
		db.Add({ 2017, 6, 5 }, "sleep");
		db.Add({ 2017, 6, 6 }, "visit parents");
		db.Add({ 2017, 6, 6 }, "sleep");
		db.Add({ 2017, 6, 7 }, "work");
		db.Add({ 2017, 6, 7 }, "sleep");
		db.Add({ 2017, 6, 8 }, "sleep");

		auto predicate1 = [](const Date& date, const string& event) {
			return event == "work";
		};

		vector<string> found1 = db.FindIf(predicate1);
		AssertEqual(found1, vector<string> { "2017-06-01 work", "2017-06-02 work", "2017-06-03 work",
			"2017-06-04 work", "2017-06-07 work" }, "find case 1");
	}
	{
		Database db;

		db.Add({ 2017, 6, 1 }, "work");
		db.Add({ 2017, 6, 1 }, "sleep");
		db.Add({ 2017, 6, 2 }, "work");
		db.Add({ 2017, 6, 2 }, "sleep");
		db.Add({ 2017, 6, 3 }, "work");
		db.Add({ 2017, 6, 3 }, "sleep");
		db.Add({ 2017, 6, 4 }, "work");
		db.Add({ 2017, 6, 4 }, "sleep");
		db.Add({ 2017, 6, 5 }, "play computer games");
		db.Add({ 2017, 6, 5 }, "sleep");
		db.Add({ 2017, 6, 6 }, "visit parents");
		db.Add({ 2017, 6, 6 }, "sleep");
		db.Add({ 2017, 6, 7 }, "work");
		db.Add({ 2017, 6, 7 }, "sleep");
		db.Add({ 2017, 6, 8 }, "sleep");

		auto predicate1 = [](const Date& date, const string& event) {
			return event != "work";
		};

		vector<string> found1 = db.FindIf(predicate1);
		AssertEqual(found1, vector<string> {"2017-06-01 sleep", "2017-06-02 sleep", "2017-06-03 sleep",
			"2017-06-04 sleep", "2017-06-05 play computer games", "2017-06-05 sleep",
			"2017-06-06 visit parents", "2017-06-06 sleep", "2017-06-07 sleep", "2017-06-08 sleep"}, "find case 2");
	}
}

void TestDatabaseDel() {
	{
		Database db;

		db.Add({ 2017, 6, 1 }, "1st of June");
		db.Add({ 2017, 6, 2 }, "2nd of June");
		db.Add({ 2017, 6, 3 }, "3rd of June");
		db.Add({ 2017, 6, 4 }, "4th of June");
		db.Add({ 2017, 6, 5 }, "5th of June");
		db.Add({ 2017, 7, 8 }, "8th of July");
		db.Add({ 2017, 7, 8 }, "Someone's birthday");
		db.Add({ 2017, 7, 8 }, "Buy groceries");
		db.Add({ 2017, 7, 9 }, "9th of July");
		db.Add({ 2017, 7, 10 }, "10th of July");
		db.Add({ 2017, 7, 11 }, "11th of July");
		db.Add({ 2017, 7, 12 }, "12th of July");
		db.Add({ 2017, 7, 13 }, "13th of July");
		db.Add({ 2017, 7, 14 }, "14th of July");

		auto predicate = [](const Date& date, const string& event) {
			return date == Date{ 2017, 7, 8 };
		};

		int removed = db.RemoveIf(predicate);
		AssertEqual(removed, 3, "delete case 1");
		ostringstream out;
		db.Print(out);

		AssertEqual(out.str(), "2017-06-01 1st of June\n2017-06-02 2nd of June\n2017-06-03 3rd of June\n2017-06-04 4th of June\n2017-06-05 5th of June\n2017-07-09 9th of July\n2017-07-10 10th of July\n2017-07-11 11th of July\n2017-07-12 12th of July\n2017-07-13 13th of July\n2017-07-14 14th of July\n", "case 1");
	}
	{
		Database db;

		db.Add({ 2017, 6, 1 }, "1st of June");
		db.Add({ 2017, 6, 2 }, "2nd of June");
		db.Add({ 2017, 6, 3 }, "3rd of June");
		db.Add({ 2017, 6, 4 }, "4th of June");
		db.Add({ 2017, 6, 5 }, "5th of June");
		db.Add({ 2017, 7, 8 }, "8th of July");
		db.Add({ 2017, 7, 8 }, "Someone's birthday");
		db.Add({ 2017, 7, 9 }, "9th of July");
		db.Add({ 2017, 7, 10 }, "10th of July");
		db.Add({ 2017, 7, 11 }, "11th of July");
		db.Add({ 2017, 7, 12 }, "12th of July");
		db.Add({ 2017, 7, 13 }, "13th of July");
		db.Add({ 2017, 7, 14 }, "14th of July");

		auto predicate = [](const Date& date, const string& event) {
			return date >= Date{ 2017, 7, 1 };
		};

		int removed = db.RemoveIf(predicate);
		AssertEqual(removed, 8, "delete case 2");

		ostringstream out;
		db.Print(out);

		AssertEqual(out.str(), "2017-06-01 1st of June\n2017-06-02 2nd of June\n2017-06-03 3rd of June\n2017-06-04 4th of June\n2017-06-05 5th of June\n", "case 2");
	}
	{
		Database db;

		db.Add({ 2017, 6, 1 }, "work");
		db.Add({ 2017, 6, 1 }, "sleep");
		db.Add({ 2017, 6, 2 }, "work");
		db.Add({ 2017, 6, 2 }, "sleep");
		db.Add({ 2017, 6, 3 }, "work");
		db.Add({ 2017, 6, 3 }, "sleep");
		db.Add({ 2017, 6, 4 }, "work");
		db.Add({ 2017, 6, 4 }, "sleep");
		db.Add({ 2017, 6, 5 }, "play computer games");
		db.Add({ 2017, 6, 5 }, "sleep");
		db.Add({ 2017, 6, 6 }, "visit parents");
		db.Add({ 2017, 6, 6 }, "sleep");
		db.Add({ 2017, 6, 7 }, "work");
		db.Add({ 2017, 6, 7 }, "sleep");
		db.Add({ 2017, 6, 8 }, "sleep");

		auto predicate = [](const Date& date, const string& event) {
			return event == "work";
		};

		int removed = db.RemoveIf(predicate);
		AssertEqual(removed, 5, "delete case 3");

		ostringstream out;
		db.Print(out);

		AssertEqual(out.str(), "2017-06-01 sleep\n2017-06-02 sleep\n2017-06-03 sleep\n2017-06-04 sleep\n2017-06-05 play computer games\n2017-06-05 sleep\n2017-06-06 visit parents\n2017-06-06 sleep\n2017-06-07 sleep\n2017-06-08 sleep\n", "case 3");

	}
}

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestParseEvent, "TestParseEvent");
	tr.RunTest(TestDatabasePrint, "TestDatabasePrint");
	tr.RunTest(TestDatabaseFind, "TestDatabaseFind");
	tr.RunTest(TestDatabaseDel, "TestDatabaseDel");

	tr.RunTest(TestDatabaseLast, "TestDatabaseLast");
}
