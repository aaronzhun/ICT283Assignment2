#ifndef CALCULATION_H_INCLUDED
#define CALCULATION_H_INCLUDED

#include "Vector.h"

/**
 * @file Calculation.h
 * @author Lau Ze Zhun Aaron
 * @brief Numeric statistics used by the menu and monthly processing
 */

/**
 * @class Calculation
 * @brief Groups the statistical routines without depending on weather data or I/O
 * @note Static methods use the supplied numbers and do not store object state
 */
class Calculation
{
public:
    /**
     * @brief Adds all values in the Vector
     * @param values Numeric values
     * @return Total, or zero for an empty Vector
     */
    static double CalculateTotal(const Vector<double> & values);

    /**
     * @brief Calculates the arithmetic mean
     * @param values Non-empty Vector of numbers
     * @return Mean of the values
     * @pre values.Size() > 0
     */
    static double CalculateMean(const Vector<double> & values);

    /**
     * @brief Calculates sample standard deviation
     * @param values Numeric values
     * @param mean Mean of those values
     * @param[out] deviation Receives the result, or zero when fewer than two values exist
     * @return True when sample standard deviation is defined
     * @note False means insufficient values, not a deviation of zero
     */
    static bool CalculateSampleStandardDeviation(const Vector<double> & values, double mean, double & deviation);

    /**
     * @brief Checks whether paired values have a defined correlation
     * @param x First numeric series
     * @param y Second numeric series paired by index with x
     * @return True for equal lengths with at least two pairs and nonconstant values in both series
     */
    static bool CanCalculateSPCC(const Vector<double> & x, const Vector<double> & y);

    /**
     * @brief Calculates the sample Pearson correlation coefficient
     * @param x First numeric series
     * @param y Matching numeric series
     * @return Correlation coefficient
     * @pre CanCalculateSPCC(x, y) is true
     */
    static double sPCC(const Vector<double> & x, const Vector<double> & y);
};

#endif
