#include "Processing.h"
#include "Calculation.h"

// Get the selected readings through the map and BST before building temporary numeric Vectors
// Convert wind speed to km/h here so the reusable maths routines only need numbers
// Return the monthly results to Menu, which decides how to display them
MonthSummary SummariseMonth(const WeatherStore & data, int month, int year) // Select matching month and year
{
    WeatherRecords selected;
    CollectMonth(data, month, year, selected); // Get readings through the map and the BST callback first
    Vector<double> speeds;
    Vector<double> temperatures;
    Vector<double> solarEnergy;
    MonthSummary summary;
    int solarCount = 0;
    for(int i = 0; i < selected.Size(); i++)
    {
        const DataRecType & record = selected[i];
        Date date;
        record.GetDate(date); // Obtain the Date through the record's public interface
        if(date.GetMonth() == month && date.GetYear() == year)
        {
            if(record.HasSpeed())
            {
                speeds.Insert(speeds.Size(), record.GetSpeed() * 3.6); // Convert m/s to km/h
            }
            if(record.HasTemperature())
            {
                temperatures.Insert(temperatures.Size(), record.GetTemperature());
            }
            if(record.HasSolar())
            {
                solarCount++;
                if(record.GetSolar() >= 100)
                {
                    solarEnergy.Insert(solarEnergy.Size(), record.GetSolar() / 6000.0); // Ten-minute energy in kWh/m2
                }
            }
        }
    }
    summary.SetSpeedCount(speeds.Size());
    summary.SetTemperatureCount(temperatures.Size());
    summary.SetSolarCount(solarCount);
    if(speeds.Size() > 0)
    {
        double mean = Calculation::CalculateMean(speeds);
        double deviation;
        Calculation::CalculateSampleStandardDeviation(speeds, mean, deviation);
        summary.SetMeanSpeed(mean);
        summary.SetSpeedDeviation(deviation);
    }
    if(temperatures.Size() > 0)
    {
        double mean = Calculation::CalculateMean(temperatures);
        double deviation;
        Calculation::CalculateSampleStandardDeviation(temperatures, mean, deviation);
        summary.SetMeanTemperature(mean);
        summary.SetTemperatureDeviation(deviation);
    }
    summary.SetTotalSolar(Calculation::CalculateTotal(solarEnergy)); // Valid sub-threshold readings give zero, not missing
    return summary;
}
