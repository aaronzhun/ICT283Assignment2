#include "../Calculation.h"
#include <iostream>

int main()
{
    Vector<double> x;
    Vector<double> positive;
    Vector<double> negative;
    Vector<double> zero;
    Vector<double> constant;
    double a[3] = {-1, 0, 1};
    double b[3] = {-2, 0, 2};
    double c[3] = {2, 0, -2};
    double d[3] = {1, -2, 1};
    for(int i = 0; i < 3; i++)
    {
        x.Insert(x.Size(), a[i]);
        positive.Insert(positive.Size(), b[i]);
        negative.Insert(negative.Size(), c[i]);
        zero.Insert(zero.Size(), d[i]);
        constant.Insert(constant.Size(), 5);
    }
    std::cout << "Positive correlation: " << Calculation::sPCC(x, positive) << '\n';
    std::cout << "Negative correlation: " << Calculation::sPCC(x, negative) << '\n';
    std::cout << "Zero correlation: " << Calculation::sPCC(x, zero) << '\n';
    std::cout << "Constant series unavailable: " << !Calculation::CanCalculateSPCC(x, constant) << '\n';
    Vector<double> empty;
    std::cout << "Empty series unavailable: " << !Calculation::CanCalculateSPCC(empty, empty) << '\n';
    Vector<double> one;
    one.Insert(0, 1);
    std::cout << "Singleton unavailable: " << !Calculation::CanCalculateSPCC(one, one) << '\n';
    std::cout << "Unequal lengths unavailable: " << !Calculation::CanCalculateSPCC(x, one) << '\n';
    double deviation;
    Calculation::CalculateSampleStandardDeviation(x, Calculation::CalculateMean(x), deviation);
    std::cout << "Sample standard deviation: " << deviation << '\n';
    Vector<double> readings;
    readings.Insert(0, 10);
    readings.Insert(1, 20);
    readings.Insert(2, 30);
    std::cout << "Total of 10 20 30: " << Calculation::CalculateTotal(readings) << '\n';
    double mean = Calculation::CalculateMean(readings);
    std::cout << "Mean of 10 20 30: " << mean << '\n';
    Calculation::CalculateSampleStandardDeviation(readings, mean, deviation);
    std::cout << "Sample standard deviation of 10 20 30: " << deviation << '\n';
    return 0;
}
