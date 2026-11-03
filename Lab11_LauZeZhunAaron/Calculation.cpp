#include "Calculation.h"
//testing git comments

//cannot use std::function
//cannot use range for loops
//cannot use lamda function

//must have map and tree, but vector optional

#include <cmath>

// This class only handles numbers so it can be reused without weather records or console code
// Add all the values and return zero when the Vector is empty
double Calculation::CalculateTotal(const Vector<double> & values)
{
    double sum = 0;
    for(int i = 0; i < values.Size(); i++)
    {
        sum += values[i]; // double reduces rounding when many readings are added
    }
    return sum;
}

// Divide the total by the number of values, the caller supplies a non-empty Vector
double Calculation::CalculateMean(const Vector<double> & values)
{
    return CalculateTotal(values) / values.Size();
}

// Add the squared differences from the mean and divide by N minus one before taking the square root
// A sample needs at least two values, false lets the menu show N/A rather than a misleading zero
bool Calculation::CalculateSampleStandardDeviation(const Vector<double> & values, double mean, double & deviation)
{
    deviation = 0;
    if(values.Size() < 2)
    {
        return false; // A sample of one has no defined sample standard deviation
    }
    double sum = 0;
    for(int i = 0; i < values.Size(); i++)
    {
        double difference = values[i] - mean;
        sum += difference * difference;
    }
    deviation = std::sqrt(sum / (values.Size() - 1)); // Sample formula divides by N - 1
    return true;
}


// Correlation needs equal sized Vectors with at least two matched pairs
bool Calculation::CanCalculateSPCC(const Vector<double> & x, const Vector<double> & y)
{
    if(x.Size() < 2 || x.Size() != y.Size())
    {
        return false;
    }
    double meanX = CalculateMean(x);
    double meanY = CalculateMean(y);
    double squareX = 0;
    double squareY = 0;
    for(int i = 0; i < x.Size(); i++)
    {
        double dx = x[i] - meanX;
        double dy = y[i] - meanY;
        squareX += dx * dx;
        squareY += dy * dy;
    }
    return squareX > 0 && squareY > 0; // A constant series makes correlation undefined
}

// Find both means, then add dx*dy dx*dx and dy*dy for the matching pairs
// Divide the product total by sqrt(squareX) * sqrt(squareY), the sample N minus one factors cancel
// The caller checks CanCalculateSPCC first so undefined cases are displayed as N/A
// These numeric Vectors keep the maths independent of weather records, files and menu output
double Calculation::sPCC(const Vector<double> & x, const Vector<double> & y)
{
    double meanX = CalculateMean(x);
    double meanY = CalculateMean(y);
    double product = 0;
    double squareX = 0;
    double squareY = 0;
    for(int i = 0; i < x.Size(); i++)
    {
        double dx = x[i] - meanX;
        double dy = y[i] - meanY;
        product += dx * dy; // x[i] and y[i] must belong to the same reading
        squareX += dx * dx;
        squareY += dy * dy;
    }
    return product / (std::sqrt(squareX) * std::sqrt(squareY)); // The sample N minus one factors cancel in this ratio
}
