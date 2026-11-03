#include "Menu.h"
#include "Processing.h"
#include "Calculation.h"
#include "DateTimeIO.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;
using std::ostream;
using std::cout;
using std::cin;
using std::getline;
using std::fixed;
using std::setprecision;

bool ReadInteger(const string & prompt, int & value);
void PrintDeviation(ostream & output, int count, double deviation);
void PrintWind(const WeatherStore & data, int month, int year);
void PrintTemperature(const WeatherStore & data, int year);
void PrintCorrelation(const WeatherStore & data, int month);
void PrintPair(const string & label, const Vector<double> & x, const Vector<double> & y);

// Handle the user's choices and console output, leaving storage and maths to their own modules
// Options 1 2 and 3 use readings collected from the map and BST
// Option 4 is a visible unfinished stub in this Lab 11 version
void RunMenu(const WeatherStore & data)
{
    cout << fixed << setprecision(1); // Displays results to one decimal place
    int option = 0;
    while(option != 5)
    {
        cout << "\n1. Monthly wind average and sample standard deviation\n"
             << "2. Monthly temperature averages and sample standard deviations\n"
             << "3. Sample Pearson correlation for a month across all years\n"
             << "4. Yearly report with MAD (WIP)\n"
             << "5. Exit\n";
        if(!ReadInteger("Choose an option: ", option))
        {
            return;
        }
        if(option == 5)
        {
            return;
        }
        if(option < 1 || option > 5)
        {
            cout << "Choose a number from 1 to 5.\n";
        }
        else
        {
            int month = 0;
            int year;
            bool validMonth = true;
            if(option == 1 || option == 3)
            {
                if(!ReadInteger("Enter month (1-12): ", month))
                {
                    return;
                }
                if(month < 1 || month > 12)
                {
                    cout << "Month must be between 1 and 12.\n";
                    validMonth = false;
                }
            }
            if(validMonth)
            {
                if(option == 3)
                {
                    PrintCorrelation(data, month); // Option 3 asks for a month only and combines all loaded years
                }
                else if(option == 4)
                {
                    cout << "Option 4 not implemented yet\n";
                }
                else
                {
                    if(!ReadInteger("Enter year: ", year))
                    {
                        return;
                    }
                    switch(option)
                    {
                    case 1:
                        PrintWind(data, month, year);
                        break;
                    case 2:
                        PrintTemperature(data, year);
                        break;
                    }
                }
            }
        }
    }
}

bool ReadInteger(const string & prompt, int & value)
{
    string line;
    while(true)
    {
        cout << prompt;
        if(!getline(cin, line))
        {
            return false; // End of console input also ends the menu
        }
        cout << "--------------------------------------------------\n";
        stringstream input(line);
        char extra;
        if((input >> value) && !(input >> extra))
        {
            return true; // Accepts one integer with no extra characters
        }
        cout << "Enter an integer number.\n";
    }
}

void PrintDeviation(ostream & output, int count, double deviation)
{
    if(count >= 2)
    {
        output << deviation;
    }
    else
    {
        output << "N/A"; // One reading has a mean but no sample standard deviation
    }
}

void PrintWind(const WeatherStore & data, int month, int year)
{
    MonthSummary summary = SummariseMonth(data, month, year);
    cout << GetMonthName(month) << ' ' << year << ':';
    if(summary.GetSpeedCount() == 0)
    {
        cout << " No Data\n";
        return;
    }
    cout << "\nAverage speed: " << summary.GetMeanSpeed() << " km/h\nSample stdev: ";
    PrintDeviation(cout, summary.GetSpeedCount(), summary.GetSpeedDeviation());
    cout << '\n';
}

void PrintTemperature(const WeatherStore & data, int year)
{
    cout << year << '\n';
    for(int month = 1; month <= 12; month++)
    {
        MonthSummary summary = SummariseMonth(data, month, year);
        cout << GetMonthName(month) << ':';
        if(summary.GetTemperatureCount() == 0)
        {
            cout << " No Data\n";
        }
        else
        {
            cout << " average: " << summary.GetMeanTemperature() << " degrees C, stdev: ";
            PrintDeviation(cout, summary.GetTemperatureCount(), summary.GetTemperatureDeviation());
            cout << '\n';
        }
    }
}

// Check the pair before calculating, then print its coefficient or N/A when undefined
// A genuine zero correlation is a result, it is different from not having a defined correlation
void PrintPair(const string & label, const Vector<double> & x, const Vector<double> & y)
{
    cout << label << ": ";
    if(Calculation::CanCalculateSPCC(x, y))
    {
        cout << Calculation::sPCC(x, y) << '\n';
    }
    else
    {
        cout << "N/A\n"; // Fewer than two pairs or a constant series has no defined correlation
    }
}

// Option 3 uses the chosen month across all loaded years, so it passes year 0 to CollectMonth
// Build S_T S_R and T_R separately using two available sensors from the same timestamped record
// Separate pairs keep a missing sensor from removing a valid pair of the other two sensors
// Use raw solar readings here without the energy threshold, then print each coefficient to two decimals
void PrintCorrelation(const WeatherStore & data, int month)
{
    WeatherRecords records;
    CollectMonth(data, month, 0, records); // Zero means use this month from every year
    Vector<double> speedTemperatureS, speedTemperatureT;
    Vector<double> speedSolarS, speedSolarR;
    Vector<double> temperatureSolarT, temperatureSolarR;
    for(int i = 0; i < records.Size(); i++)
    {
        const DataRecType & record = records[i];
        if(record.HasSpeed() && record.HasTemperature())
        {
            speedTemperatureS.Insert(speedTemperatureS.Size(), record.GetSpeed());
            speedTemperatureT.Insert(speedTemperatureT.Size(), record.GetTemperature());
        }
        if(record.HasSpeed() && record.HasSolar())
        {
            speedSolarS.Insert(speedSolarS.Size(), record.GetSpeed());
            speedSolarR.Insert(speedSolarR.Size(), record.GetSolar());
        }
        if(record.HasTemperature() && record.HasSolar())
        {
            temperatureSolarT.Insert(temperatureSolarT.Size(), record.GetTemperature());
            temperatureSolarR.Insert(temperatureSolarR.Size(), record.GetSolar());
        }
    }
    cout << fixed << setprecision(2);
    cout << "Sample Pearson Correlation Coefficient for " << GetMonthName(month) << '\n';
    PrintPair("S_T", speedTemperatureS, speedTemperatureT); // Both values were taken from the same record
    PrintPair("S_R", speedSolarS, speedSolarR); // Raw available solar values are used here, not the energy threshold
    PrintPair("T_R", temperatureSolarT, temperatureSolarR);
    cout << setprecision(1); // Put the original menu precision back for options 1 2 and 4
}
