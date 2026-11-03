#include "DataIO.h"
#include "DateTimeIO.h"

#include <fstream>
#include <sstream>

using std::string;
using std::ifstream;
using std::stringstream;
using std::getline;

// Helper declarations used by the file reader
bool ReadDateTime(const string & text, Date & date, Time & time);
bool HasText(const string & text);
int FindColumnIndex(const string & header, const string & columnName);
bool ReadNumber(const string & text, float & value);
bool ReadWeatherData(const string & fileName, WeatherStore & data, int & added, int & duplicates);

// Read every filename from the source list and put all files into the same map and BST storage
// File reading stays here so changing the CSV format does not require changing the generic BST
bool LoadData(const string & sourceFile, WeatherStore & data, int & added, int & duplicates) // Read every listed CSV into the same daily map
{
    added = 0;
    duplicates = 0;
    ifstream source(sourceFile.c_str()); //Opens the source text file for reading
    string fileName;
    if(!source)
    {
        return false;
    }
    bool foundFile = false;
    while(source >> fileName) // Reads every filename in the order supplied
    {
        foundFile = true;
        if(!ReadWeatherData("data/" + fileName, data, added, duplicates)) // Add readings to the same day groups
        {
            source.close();
            return false;
        }
    }
    source.close();
    return foundFile; // An empty source list does not provide any data files
}

bool HasText(const string & text) //return true if the line has text, return false if blank line
{
    bool hasText = false; //default initilisation
    int index = 0;
    int length = text.length(); //store the length
    while(index < length && !hasText) // Continues while characters remain and no field value has been found
    {
        if(text[index] != ' ' && text[index] != '\t' && text[index] != '\r'
           && text[index] != ',') // A row of empty CSV fields contains no reading
        {
            hasText = true; // At least one field contains a nonblank value
        }
        index++;
    }
    return hasText;
}

// Look for WAST S T and SR by heading name rather than assuming fixed column positions
// Each file has its own heading row so the column order can change between files
int FindColumnIndex(const string & header, const string & columnName) // Find the heading by name so columns can move
{
    stringstream fields(header); //creates a stream containing the header text, eg; WAST,S,T,SR,RB,RO,GB
    string field;
    int index = 0;
    while(getline(fields, field, ',')) //loops and read characters into the field eg wast, and uses comma as a seperator
    {
        stringstream nameStream(field); //another stream just for the field
        string name;
        nameStream >> name; // Reads the heading without surrounding whitespace remove all the blank spaces "   T" = "T"
        if(name == columnName) //compares string to the parameter column name if matched or not
        {
            return index;
        }
        index++;
    }
    return -1;
}

// Read the field as a number and reject leftover characters such as 12abc
// A failed conversion leaves that sensor unavailable, the other sensors in the record can still be used
// Zero is a valid reading and is kept separately from the availability flag
bool ReadNumber(const string & text, float & value) //Tries to read a floating-point value from the field
{
    value = 0;
    stringstream numberStream(text); //Creates a stream containing the text, so it can be read using the >>
    if(!(numberStream >> value))
    {
        return false; // Blank or nonnumeric fields cannot supply a reading
    }
    char extra;
    return !(numberStream >> extra); // Reject values such as 12abc instead of accepting just 12
}

// Build one complete weather record from each nonblank row using the heading positions
// The day key selects its BST and Insert rejects a timestamp already stored in that tree
// The first duplicate is kept, a later duplicate does not fill missing sensors in the first record
bool ReadWeatherData(const string & fileName, WeatherStore & data, int & added, int & duplicates) // Loads the date, time and sensor readings using their column positions WeatherRecords is the vector
{
    ifstream inputFile(fileName.c_str());
    if(!inputFile) // Stops if the CSV file cannot be opened
    {
        return false;
    }
    string header;
    bool foundHeader = false;
    while(!foundHeader && getline(inputFile, header))
    {
        foundHeader = HasText(header); // Stops once a nonblank heading row is found
    }

    int dateIndex = FindColumnIndex(header, "WAST"); // Timestamp heading
    int speedIndex = FindColumnIndex(header, "S");
    int temperatureIndex = FindColumnIndex(header, "T");
    int solarIndex = FindColumnIndex(header, "SR");

    if(dateIndex < 0 || speedIndex < 0 || temperatureIndex < 0 || solarIndex < 0) //small error handling if index cannot be found, meaning any whole column cannot be found, then return false
    {
        return false;
    }

    string line;
    while(getline(inputFile, line)) // Reads one CSV record per line
    {
        if(HasText(line)) // Only process nonblank rows
        {
            stringstream row(line); //read one line
            string field;           //hold one field at atime
            string dateText;        //save the datetime for later processing
            DataRecType record; //Starts a weather record with all sensor readings marked unavailable
            int column = 0;
            while(getline(row, field, ',')) // Reads each comma-separated field in the current row
            {
                if(column == dateIndex)
                {
                    dateText = field;
                }
                else if(column == speedIndex)
                {
                    float value;
                    if(ReadNumber(field, value))
                    {
                        record.SetSpeed(value); // A successfully parsed number is marked available
                    }
                }
                else if(column == temperatureIndex)
                {
                    float value;
                    if(ReadNumber(field, value))
                    {
                        record.SetTemperature(value); // A successfully parsed number is marked available
                    }
                }
                else if(column == solarIndex)
                {
                    float value;
                    if(ReadNumber(field, value))
                    {
                        record.SetSolar(value); // A successfully parsed number is marked available
                    }
                }
                column++;
            }
            Date date;
            Time time;
            if(!ReadDateTime(dateText, date, time)) // Both Date and Time form the unique key
            {
                return false; // A record without its date/time cannot be assigned to a month
            }
            record.SetDate(date);
            record.SetTime(time);
            int dayKey = GetDayKey(date);
            if(data[dayKey].Insert(record)) // operator[] gets this day's tree or creates an empty one
            {
                added++;
            }
            else
            {
                duplicates++; // Keep the first row with this same date and time
            }
        }
    }

    inputFile.close();
    return true;
}

// Check the timestamp separators, calendar date and time before setting Date and Time
// A bad timestamp stops loading because the record cannot be placed into the correct day tree
bool ReadDateTime(const string & text, Date & date, Time & time)
{
    stringstream input(text);
    int day, month, year, hour, minute;
    char slash1, slash2, colon, extra;
    if(!(input >> day >> slash1 >> month >> slash2 >> year >> hour >> colon >> minute))
    {
        return false;
    }
    if(slash1 != '/' || slash2 != '/' || colon != ':' || (input >> extra))
    {
        return false;
    }
    if(year < 1 || year > 9999 || month < 1 || month > 12 || hour < 0 || hour > 23 || minute < 0 || minute > 59)
    {
        return false;
    }
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
    {
        days[1] = 29; // February has an extra day in a leap year
    }
    if(day < 1 || day > days[month - 1])
    {
        return false;
    }
    date.SetDay(day);
    date.SetMonth(month);
    date.SetYear(year);
    time.SetHour(hour);
    time.SetMinute(minute);
    return true;
}
