// Tasks from the course "Fundamentals of C++: Red Belt
// Done by LiubovPil

#include "Airline_ticket_part2.h"
#include "Test_runner.h"

#include <sstream>

using namespace std;

#define UPDATE_FIELD(ticket, field, values) {		\
	map<string, string>::const_iterator it;			\
	it = values.find(#field);						\
	if (it != values.end()) {						\
		istringstream is(it->second);               \
		is >> ticket.field;							\
	}												\
}
// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time

bool operator < (const Date& lhs, const Date& rhs) {
	if (lhs.year == rhs.year) {
		if (lhs.month == rhs.month) {
			return lhs.day < rhs.day;
		}
		return lhs.month < rhs.month;
	}
	else
		return lhs.day < rhs.day;
}

bool operator == (const Date& lhs, const Date& rhs) {
	if (lhs.year == rhs.year &&
		lhs.month == rhs.month &&
		lhs.day == rhs.day)
		return true;
	else
		return false;
}

ostream& operator << (ostream& out, const Date& date) {
	out << date.year << "-" << date.month << "-" << date.day << endl;
	return out;
}

istream& operator >> (istream& is, Date& date) {
	is >> date.year;
	is.ignore(1);
	is >> date.month;
	is.ignore(1);
	is >> date.day;
	return is;
}

bool operator < (const Time& lhs, const Time& rhs) {
	if (lhs.hours == rhs.hours)
		return lhs.minutes < rhs.minutes;
	else
		return lhs.hours < rhs.hours;
}

bool operator == (const Time& lhs, const Time& rhs) {
	if (lhs.hours == rhs.hours &&
		lhs.minutes == rhs.minutes)
		return true;
	else
		return false;
}

ostream& operator << (ostream& out, const Time& time) {
	out << time.hours << ":" << time.minutes << endl;
	return out;
}

istream& operator >> (istream& is, Time& time) {
	is >> time.hours;
	is.ignore(1);
	is >> time.minutes;
	return is;
}

void TestUpdate() {
	AirlineTicket t;
	t.price = 0;

	const map<string, string> updates1 = {
	{"departure_date", "2018-2-28"},
	{"departure_time", "17:40"},
	};
	UPDATE_FIELD(t, departure_date, updates1);
	UPDATE_FIELD(t, departure_time, updates1);
	UPDATE_FIELD(t, price, updates1);

	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 0);

	const map<string, string> updates2 = {
	{"price", "12550"},
	{"arrival_time", "20:33"},
	};
	UPDATE_FIELD(t, departure_date, updates2);
	UPDATE_FIELD(t, departure_time, updates2);
	UPDATE_FIELD(t, arrival_time, updates2);
	UPDATE_FIELD(t, price, updates2);

	// updates2 не содержит ключей "departure_date" и "departure_time", поэтому значения этих полей не должны измениться
	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 12550);
	ASSERT_EQUAL(t.arrival_time, (Time{ 20, 33 }));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestUpdate);
}
