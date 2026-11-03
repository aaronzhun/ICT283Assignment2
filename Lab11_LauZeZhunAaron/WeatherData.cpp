#include "WeatherData.h"

DataRecType::DataRecType()
{
    m_speed = 0;
    m_temperature = 0;
    m_solar = 0;
    m_hasSpeed = false;
    m_hasTemperature = false;
    m_hasSolar = false;
}

void DataRecType::GetDate(Date & date) const
{
    date = m_date; // Copies the nested object through controlled access
}

void DataRecType::SetDate(const Date & date)
{
    m_date = date;
}

void DataRecType::GetTime(Time & time) const
{
    time = m_time;
}

void DataRecType::SetTime(const Time & time)
{
    m_time = time;
}

float DataRecType::GetSpeed() const
{
    return m_speed;
}

void DataRecType::SetSpeed(float speed)
{
    m_speed = speed;
    m_hasSpeed = true; // Stores availability separately so zero stays valid
}

bool DataRecType::HasSpeed() const
{
    return m_hasSpeed;
}

float DataRecType::GetTemperature() const
{
    return m_temperature;
}

void DataRecType::SetTemperature(float temperature)
{
    m_temperature = temperature;
    m_hasTemperature = true; // Stores availability separately so zero stays valid
}

bool DataRecType::HasTemperature() const
{
    return m_hasTemperature;
}

float DataRecType::GetSolar() const
{
    return m_solar;
}

void DataRecType::SetSolar(float solar)
{
    m_solar = solar;
    m_hasSolar = true; // Stores availability separately so zero stays valid
}

bool DataRecType::HasSolar() const
{
    return m_hasSolar;
}

MonthSummary::MonthSummary()
{
    m_speedCount = 0;
    m_temperatureCount = 0;
    m_solarCount = 0;
    m_meanSpeed = 0;
    m_speedDeviation = 0;
    m_meanTemperature = 0;
    m_temperatureDeviation = 0;
    m_totalSolar = 0;
}

int MonthSummary::GetSpeedCount() const
{
    return m_speedCount;
}

void MonthSummary::SetSpeedCount(int speedCount)
{
    m_speedCount = speedCount;
}

int MonthSummary::GetTemperatureCount() const
{
    return m_temperatureCount;
}

void MonthSummary::SetTemperatureCount(int temperatureCount)
{
    m_temperatureCount = temperatureCount;
}

int MonthSummary::GetSolarCount() const
{
    return m_solarCount;
}

void MonthSummary::SetSolarCount(int solarCount)
{
    m_solarCount = solarCount;
}

double MonthSummary::GetMeanSpeed() const
{
    return m_meanSpeed;
}

void MonthSummary::SetMeanSpeed(double meanSpeed)
{
    m_meanSpeed = meanSpeed;
}

double MonthSummary::GetSpeedDeviation() const
{
    return m_speedDeviation;
}

void MonthSummary::SetSpeedDeviation(double speedDeviation)
{
    m_speedDeviation = speedDeviation;
}

double MonthSummary::GetMeanTemperature() const
{
    return m_meanTemperature;
}

void MonthSummary::SetMeanTemperature(double meanTemperature)
{
    m_meanTemperature = meanTemperature;
}

double MonthSummary::GetTemperatureDeviation() const
{
    return m_temperatureDeviation;
}

void MonthSummary::SetTemperatureDeviation(double temperatureDeviation)
{
    m_temperatureDeviation = temperatureDeviation;
}

double MonthSummary::GetTotalSolar() const
{
    return m_totalSolar;
}

void MonthSummary::SetTotalSolar(double totalSolar)
{
    m_totalSolar = totalSolar;
}

// This comparison belongs to the weather application, not inside the generic BST
// Compare year month day hour and minute in that order, not the sensor readings
// The BST calls this operator through T so no weather comparison rules are built into BST.h
bool operator < (const DataRecType & left, const DataRecType & right)
{
    Date ld, rd;
    Time lt, rt;
    left.GetDate(ld);
    right.GetDate(rd);
    left.GetTime(lt);
    right.GetTime(rt);
    int leftKey[5] = {ld.GetYear(), ld.GetMonth(), ld.GetDay(), lt.GetHour(), lt.GetMinute()};
    int rightKey[5] = {rd.GetYear(), rd.GetMonth(), rd.GetDay(), rt.GetHour(), rt.GetMinute()};
    for(int i = 0; i < 5; i++)
    {
        if(leftKey[i] != rightKey[i])
        {
            return leftKey[i] < rightKey[i]; // Compare year then month then day then hour then minute
        }
    }
    return false; // The whole timestamp is equal
}

bool operator > (const DataRecType & left, const DataRecType & right)
{
    return right < left;
}

// Two records have the same key when their complete Date and Time match
// Different sensor values at that same timestamp still count as a duplicate
bool operator == (const DataRecType & left, const DataRecType & right)
{
    return !(left < right) && !(right < left); // Same timestamp even if sensor values differ
}
