#pragma once

#include <string>

using namespace std;

struct Date {
	int year, month, day;
};

struct Time {
	int hours, minutes;
};
struct AirlineTicket {
	string from;
	string to;
	string airline;
	Date departure_date;
	Time departure_time;
	Date arrival_date;
	Time arrival_time;
	int price;
};

bool operator < (const Date& lhs, const Date& rhs);

bool operator == (const Date& lhs, const Date& rhs);

ostream& operator << (ostream& out, const Date& date);

bool operator < (const Time& lhs, const Time& rhs);

bool operator == (const Time& lhs, const Time& rhs);

ostream& operator << (ostream& out, const Time& time);
