#ifndef PROCESSING_H_INCLUDED
#define PROCESSING_H_INCLUDED

#include "WeatherStore.h"

/**
 * @file Processing.h
 * @author Lau Ze Zhun Aaron
 * @brief Selects monthly weather data for general statistical calculations
 */
/**
 * @brief Calculates the results for one month and year
 * @param data Stored weather readings
 * @param month Numeric month from 1 to 12
 * @param year Requested year
 * @return Summary with independent sensor counts and results
 * @pre month is between 1 and 12
 * @note Speed is converted to km/h and qualifying solar readings to kWh/m2
 * @note Solar count includes available readings below the 100 W/m2 threshold
 * @note A sample deviation is meaningful only when its sensor count is at least two
 */
MonthSummary SummariseMonth(const WeatherStore & data, int month, int year);

#endif
