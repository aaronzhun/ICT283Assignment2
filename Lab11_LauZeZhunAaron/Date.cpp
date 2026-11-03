#include "Date.h"


Date::Date()
{
    m_day = 0;
    m_month = 0;
    m_year = 0;
}

Date::Date(int day, int month, int year)
{
    m_day = day;
    m_month = month;
    m_year = year;
}

int Date::GetDay() const
{
    return m_day;
}

int Date::GetMonth() const
{
    return m_month;
}

int Date::GetYear() const
{
    return m_year;
}

void Date::SetDay(int day)
{
    m_day = day;
}

void Date::SetMonth(int month)
{
    m_month = month;
}

void Date::SetYear(int year)
{
    m_year = year;
}

