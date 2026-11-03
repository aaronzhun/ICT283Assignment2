#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

/**
 * @file Time.h
 * @author Lau Ze Zhun Aaron
 * @version 1.0
 * @date 04/10/2026
 * @brief Declaration of the Time class
 */

/**
 * @class Time
 * @brief Stores a time using an hour and minute
 * @note Stores supplied values without checking time ranges
 */
class Time
{
public:
    /**
     * @brief Initialises all stored values to zero
     */
    Time();

    /**
     * @brief Initialises all Time attributes
     * @param hour Hour value
     * @param minute Minute value
     */
    Time(int hour, int minute);

    /**
     * @brief Retrieves the hour
     * @return Stored hour
     */
    int GetHour() const;

    /**
     * @brief Retrieves the minute
     * @return Stored minute
     */
    int GetMinute() const;

    /**
     * @brief Sets the hour
     * @param hour Hour value to store
     * @return void
     */
    void SetHour(int hour);

    /**
     * @brief Sets the minute
     * @param minute Minute value to store
     * @return void
     */
    void SetMinute(int minute);

private:
    int m_hour; ///< Stored hour value
    int m_minute; ///< Stored minute value
};

#endif // TIME_H_INCLUDED
