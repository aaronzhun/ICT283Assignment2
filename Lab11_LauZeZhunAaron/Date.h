#ifndef DATE_H_INCLUDED
#define DATE_H_INCLUDED

/**
 * @file Date.h
 * @author Lau Ze Zhun Aaron
 * @version 1.0
 * @date 04/10/2026
 * @brief Declaration of the Date class
 */

/**
 * @class Date
 * @brief Stores a calendar date using a day, month and year
 * @note Stores supplied values without checking calendar validity
 */
class Date
{
public:
    /**
     * @brief Initialises all stored values to zero
     */
    Date();

    /**
     * @brief Initialises all Date attributes
     * @param day Day of the month
     * @param month Month of the year
     * @param year Calendar year
     */
    Date(int day, int month, int year);

    /**
     * @brief Retrieves the day
     * @return Day of the month
     */
    int GetDay() const;

    /**
     * @brief Retrieves the month
     * @return Month of the year
     */
    int GetMonth() const;

    /**
     * @brief Retrieves the year
     * @return Calendar year
     */
    int GetYear() const;

    /**
     * @brief Sets the day
     * @param day Day value to store
     * @return void
     */
    void SetDay(int day);

    /**
     * @brief Sets the month
     * @param month Month value to store
     * @return void
     */
    void SetMonth(int month);

    /**
     * @brief Sets the year
     * @param year Year value to store
     * @return void
     */
    void SetYear(int year);

private:
    int m_day; ///< Stored day value
    int m_month; ///< Stored month number
    int m_year; ///< Stored year value
};

#endif // DATE_H_INCLUDED
