#ifndef WEATHER_STORE_H_INCLUDED
#define WEATHER_STORE_H_INCLUDED

#include "WeatherData.h"
#include "BST.h"
#include <map>

/**
 * @file WeatherStore.h
 * @author Lau Ze Zhun Aaron
 * @brief Groups daily reading trees and collects their records for processing
 */
// Each YYYYMMDD key gives one daily BST, so both the map and BST are used by the menu
typedef std::map<int, Bst<DataRecType> > WeatherStore; ///< YYYYMMDD key associated with one daily tree

/**
 * @brief Makes the day key used by the map
 * @param date Reading date
 * @return Year month and day packed as YYYYMMDD
 */
int GetDayKey(const Date & date);

/**
 * @brief Collects readings for a month using the map and BST callbacks
 * @param data Daily trees
 * @param month Requested month from 1 to 12
 * @param year Requested year or zero for all years
 * @param records Receives the readings appended to this Vector
 */
void CollectMonth(const WeatherStore & data, int month, int year, WeatherRecords & records);

/**
 * @class RecordCollector
 * @brief Connects the plain BST callback to a destination Vector
 * @note Used for one traversal at a time in this single-thread console program
 */
class RecordCollector
{
public:
    /**
     * @brief Appends the tree values to the client's Vector
     * @param tree Tree to traverse without changing its keys
     * @param records Destination owned by the caller
     */
    static void Collect(const Bst<DataRecType> & tree, WeatherRecords & records);

private:
    /**
     * @brief Receives one record from the traversal
     * @param record Read-only record stored in the tree
     */
    static void AddRecord(const DataRecType & record);
    static WeatherRecords * m_records; ///< Temporary destination during the synchronous traversal
};

#endif
