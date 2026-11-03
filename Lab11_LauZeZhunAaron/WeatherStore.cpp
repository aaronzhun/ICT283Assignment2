#include "WeatherStore.h"

typedef WeatherStore::const_iterator WeatherIterator; // Short name for a read-only map iterator, following the Lecture 20 map example

WeatherRecords * RecordCollector::m_records = nullptr;

// Pack year month and day into one key so readings from the same day use the same tree
int GetDayKey(const Date & date)
{
    return date.GetYear() * 10000 + date.GetMonth() * 100 + date.GetDay(); // 2 March 2015 becomes 20150302
}

// Pass AddRecord without () so the traversal calls it later for each stored reading
// The callback only receives one record, so remember the destination Vector in a private static pointer
// Clear that pointer afterwards, this simple collector supports one traversal at a time
void RecordCollector::Collect(const Bst<DataRecType> & tree, WeatherRecords & records)
{
    m_records = &records; // Remember the caller's Vector while this traversal is running
    tree.InOrder(AddRecord); // Pass the function without () so the BST can call it for every record
    m_records = nullptr; // Do not keep pointing at the caller's Vector after the traversal
}

// This callback returns void and takes const DataRecType &, matching void (*visit)(const T &)
// It is static so no object or hidden this parameter is needed by the plain function pointer
// A non-static member or CalculateMean cannot be passed here because its function type does not match
// Append a copy to the caller's Vector, const prevents this callback from changing the tree's key
void RecordCollector::AddRecord(const DataRecType & record)
{
    m_records->Insert(m_records->Size(), record); // Copy the reading without changing the key inside the tree
}

// Check each map day key and collect readings from the matching daily trees
// A year of 0 means use this month from every loaded year, as required by option 3
// Daily trees limit the number of nodes per tree but do not make the BST balanced
// This scans the day keys and makes temporary copies for calculation
void CollectMonth(const WeatherStore & data, int month, int year, WeatherRecords & records)
{
    // Start at the first entry, stop after the last entry and move to the next day each time
    for(WeatherIterator day = data.begin(); day != data.end(); day++)
    {
        int storedMonth = (day->first / 100) % 100; // Extract MM from YYYYMMDD
        int storedYear = day->first / 10000;
        if(storedMonth == month && (year == 0 || storedYear == year))
        {
            RecordCollector::Collect(day->second, records); // first is the key and second is the daily BST
        }
    }
}
