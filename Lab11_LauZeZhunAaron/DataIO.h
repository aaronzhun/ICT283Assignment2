#ifndef DATA_IO_H_INCLUDED
#define DATA_IO_H_INCLUDED

#include "WeatherStore.h"
#include <string>

/**
 * @file DataIO.h
 * @author Lau Ze Zhun Aaron
 * @brief Loads the listed CSV files into daily reading trees
 */
/**
 * @brief Reads every listed CSV and rejects repeated date-time keys
 * @param sourceFile Filename list such as data/data_source.txt
 * @param data Destination map of daily BSTs
 * @param added Receives the number of inserted readings
 * @param duplicates Receives the number of repeated timestamps
 * @return False for an unreadable file, missing heading or invalid timestamp
 * @note The first reading at a timestamp is kept if later files repeat it
 * @note Missing or invalid sensor fields remain unavailable independently
 * @note A failed load can leave some readings loaded and main then stops
 */
bool LoadData(const std::string & sourceFile, WeatherStore & data, int & added, int & duplicates);

#endif
