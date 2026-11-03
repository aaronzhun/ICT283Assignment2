#include "DateTimeIO.h"

const char * GetMonthName(int month)
{
    const char * MONTH_NAMES[12] = { "January", "February", "March", "April",
                                     "May", "June", "July", "August", "September", "October", "November", "December"
                                   };
    return MONTH_NAMES[month - 1]; // Month one uses array index zero
}

std::istream & operator >> (std::istream & input, Date & date)
{
    int day;
    int month;
    int year;
    char separator1;
    char separator2;
    if(input >> day >> separator1 >> month >> separator2 >> year)
    {
        date.SetDay(day);
        date.SetMonth(month);
        date.SetYear(year);
    }
    return input;
}

std::istream & operator >> (std::istream & input, Time & time)
{
    int hour;
    int minute;
    char separator;
    if(input >> hour >> separator >> minute)
    {
        time.SetHour(hour);
        time.SetMinute(minute);
    }
    return input;
}

ostream & operator << (ostream & os, const Date & date)
{
    os << date.GetDay() << ' ';
    if(date.GetMonth() >= 1 && date.GetMonth() <= 12)
    {
        os << GetMonthName(date.GetMonth());
    }
    else
    {
        os << date.GetMonth(); // Allows the default date to be displayed during testing
    }
    os << ' ' << date.GetYear();
    return os;
}

ostream & operator << (ostream & os, const Time & time)
{
    os << time.GetHour() << ':';
    if(time.GetMinute() < 10)
    {
        os << '0';
    }
    os << time.GetMinute();
    return os;
}
