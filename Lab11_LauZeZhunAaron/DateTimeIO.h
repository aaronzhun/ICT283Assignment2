#ifndef DATE_TIME_IO_H_INCLUDED
#define DATE_TIME_IO_H_INCLUDED

#include "Date.h"
#include "Time.h"

#include <iostream>

/**
 * @file DateTimeIO.h
 * @author Lau Ze Zhun Aaron
 * @brief Date and time stream formatting kept separate from the data classes
 */
/**
 * @brief Extracts a numeric day, separator, month, separator and year
 * @param[in,out] input Input stream
 * @param[out] date Destination date, updated only when extraction succeeds
 * @return Input stream
 * @note Separator characters and calendar ranges are not validated
 */
std::istream & operator >> (std::istream & input, Date & date);
/**
 * @brief Extracts a numeric hour, separator and minute
 * @param[in,out] input Input stream
 * @param[out] time Destination time, updated only when extraction succeeds
 * @return Input stream
 * @note The separator and time ranges are not validated
 */
std::istream & operator >> (std::istream & input, Time & time);
/**
 * @brief Writes day, full month name and year
 * @param os Output stream
 * @param date Date to write
 * @return Output stream
 * @note An out-of-range month is displayed as its stored number
 */
std::ostream & operator << (std::ostream & os, const Date & date);
/**
 * @brief Writes hour:minute
 * @param os Output stream
 * @param time Time to write
 * @return Output stream
 */
std::ostream & operator << (std::ostream & os, const Time & time);
/**
 * @brief Gets a full month name
 * @param month Numeric month from 1 to 12
 * @pre month >= 1 and month <= 12
 * @return Month name
 */
const char * GetMonthName(int month);

#endif
