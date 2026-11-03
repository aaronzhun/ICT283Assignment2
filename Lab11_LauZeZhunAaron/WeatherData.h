#ifndef WEATHER_DATA_H_INCLUDED
#define WEATHER_DATA_H_INCLUDED

#include "Date.h"
#include "Time.h"
#include "Vector.h"

/**
 * @file WeatherData.h
 * @author 35362795 Lau Ze Zhun Aaron
 * @brief weather records and monthly results
 */

/**
 * @class DataRecType
 * @brief Composite class containing a Date, a Time and weather readings
 * @note Availability is checked separately from the stored numeric value
 */
class DataRecType
{
public:
    /**
     * @brief Creates a record with zero values and unavailable sensors
     */
    DataRecType();

    /**
     * @brief Copies the reading date into the supplied object
     * @param date Receives the date
     */
    void GetDate(Date & date) const;

    /**
     * @brief Stores the reading date
     * @param date Date to copy
     */
    void SetDate(const Date & date);

    /**
     * @brief Copies the reading time into the supplied object
     * @param time Receives the time
     */
    void GetTime(Time & time) const;

    /**
     * @brief Stores the reading time
     * @param time Time to copy
     */
    void SetTime(const Time & time);

    /**
     * @brief Retrieves the speed value
     * @return Wind speed in m/s
     * @pre HasSpeed() is true when the value is to be used as a reading
     */
    float GetSpeed() const;

    /**
     * @brief Stores an available speed reading
     * @param speed Wind speed in m/s
     * @note A zero reading is available, not missing
     */
    void SetSpeed(float speed);

    /**
     * @brief Checks whether a speed reading is available
     * @return True when a speed value has been stored
     */
    bool HasSpeed() const;

    /**
     * @brief Retrieves the temperature value
     * @return Ambient temperature in degrees C
     * @pre HasTemperature() is true when the value is to be used as a reading
     */
    float GetTemperature() const;

    /**
     * @brief Stores an available temperature reading
     * @param temperature Ambient temperature in degrees C
     * @note A zero reading is available, not missing
     */
    void SetTemperature(float temperature);

    /**
     * @brief Checks whether a temperature reading is available
     * @return True when a temperature value has been stored
     */
    bool HasTemperature() const;

    /**
     * @brief Retrieves the solar value
     * @return Solar power in W/m2
     * @pre HasSolar() is true when the value is to be used as a reading
     */
    float GetSolar() const;

    /**
     * @brief Stores an available solar reading
     * @param solar Solar power in W/m2
     * @note A zero reading is available, not missing
     */
    void SetSolar(float solar);

    /**
     * @brief Checks whether a solar reading is available
     * @return True when a solar value has been stored
     */
    bool HasSolar() const;

private:
    Date m_date; ///< Date owned by this record
    Time m_time; ///< Time owned by this record
    float m_speed; ///< Wind speed in m/s
    float m_temperature; ///< Ambient temperature in degrees C
    float m_solar; ///< Solar power in W/m2
    bool m_hasSpeed; ///< True when the sensor reading is available
    bool m_hasTemperature; ///< True when the sensor reading is available
    bool m_hasSolar; ///< True when the sensor reading is available
};

typedef Vector<DataRecType> WeatherRecords; ///< Vector containing weather records

/**
 * @class MonthSummary
 * @brief Stores monthly calculation results without performing I/O
 * @note Counts distinguish missing readings from valid zero values
 * @note Sample deviation results require their corresponding count to be at least two
 */
class MonthSummary
{
public:
    /**
     * @brief Initialises all counts and results to zero
     */
    MonthSummary();

    /**
     * @brief Retrieves the number of available wind readings
     * @return Number of available wind readings
     */
    int GetSpeedCount() const;

    /**
     * @brief Stores the number of available wind readings
     * @param speedCount Number of available wind readings
     */
    void SetSpeedCount(int speedCount);

    /**
     * @brief Retrieves the number of available temperature readings
     * @return Number of available temperature readings
     */
    int GetTemperatureCount() const;

    /**
     * @brief Stores the number of available temperature readings
     * @param temperatureCount Number of available temperature readings
     */
    void SetTemperatureCount(int temperatureCount);

    /**
     * @brief Retrieves the number of available solar readings
     * @return Number of available solar readings, including those below 100
     */
    int GetSolarCount() const;

    /**
     * @brief Stores the number of available solar readings
     * @param solarCount Number of available solar readings, including those below 100
     */
    void SetSolarCount(int solarCount);

    /**
     * @brief Retrieves the monthly mean wind speed
     * @return Mean wind speed in km/h
     */
    double GetMeanSpeed() const;

    /**
     * @brief Stores the monthly mean wind speed
     * @param meanSpeed Mean wind speed in km/h
     */
    void SetMeanSpeed(double meanSpeed);

    /**
     * @brief Retrieves the wind sample standard deviation
     * @return Sample wind standard deviation in km/h
     */
    double GetSpeedDeviation() const;

    /**
     * @brief Stores the wind sample standard deviation
     * @param speedDeviation Sample wind standard deviation in km/h
     */
    void SetSpeedDeviation(double speedDeviation);

    /**
     * @brief Retrieves the monthly mean temperature
     * @return Mean ambient temperature in degrees C
     */
    double GetMeanTemperature() const;

    /**
     * @brief Stores the monthly mean temperature
     * @param meanTemperature Mean ambient temperature in degrees C
     */
    void SetMeanTemperature(double meanTemperature);

    /**
     * @brief Retrieves the temperature sample standard deviation
     * @return Sample temperature standard deviation
     */
    double GetTemperatureDeviation() const;

    /**
     * @brief Stores the temperature sample standard deviation
     * @param temperatureDeviation Sample temperature standard deviation
     */
    void SetTemperatureDeviation(double temperatureDeviation);

    /**
     * @brief Retrieves the monthly solar energy total
     * @return Total qualifying solar energy in kWh/m2
     */
    double GetTotalSolar() const;


    /**
     * @brief Stores the monthly solar energy total
     * @param totalSolar Total qualifying solar energy in kWh/m2
     */
    void SetTotalSolar(double totalSolar);

private:
    int m_speedCount; ///< Number of available wind readings
    int m_temperatureCount; ///< Number of available temperature readings
    int m_solarCount; ///< Number of available solar readings, including those below 100
    double m_meanSpeed; ///< Mean wind speed in km/h
    double m_speedDeviation; ///< Sample wind standard deviation in km/h
    double m_meanTemperature; ///< Mean ambient temperature in degrees C
    double m_temperatureDeviation; ///< Sample temperature standard deviation
    double m_totalSolar; ///< Total qualifying solar energy in kWh/m2
};


/**
 * @brief Compares reading timestamps without using sensor values as the key
 * @param left First reading
 * @param right Second reading
 * @return True when left has the earlier date and time
 */
bool operator < (const DataRecType & left, const DataRecType & right);
/**
 * @brief Compares timestamps in the reverse direction
 * @param left First reading
 * @param right Second reading
 * @return True when left has the later timestamp
 */
bool operator > (const DataRecType & left, const DataRecType & right);
/**
 * @brief Checks whether two readings have the same date and time
 * @param left First reading
 * @param right Second reading
 * @return True when timestamps match
 */
bool operator == (const DataRecType & left, const DataRecType & right);

#endif // WEATHER_DATA_H_INCLUDED
