#pragma once

class Date
{
public:
	Date();
	Date(int year, int month, int day, int hour, int minute);

	int _day;
	int _month;
	int _year;
	int _hour;
	int _minute;
};
