#ifndef INDICATORS_H
#define INDICATORS_H

#include <QString>
#include <map>

class Indicators{
public:
    //Constructor
    Indicators(std::map<std::string, float>);
    //Destructor
    ~Indicators();

    //Main indicators
    void calculateEnergyLevel(double, double);
    void calculateImmuneSystem(double, double);
    void calculateMetabolism(double, double);
    void calculatePsychoEmotional(double, double);
    void calculateMusculoskeletal(double, double);
    
    //Summary indicators
    // - Average level is the Energy Level
    // - We'll skip the golden ratios
    void calculateLeftMeridian();
    void calculateRightMeridian();
    void calculateUpperMeridian();
    void calculateLowerMeridian();

    //Getters
    std::map<std::string, float> getProcessedData() const;
    std::map<std::string, float> getStatus() const;

private:
    std::map<std::string, float> meridianPoint;
    std::map<std::string, float> processedData;
    std::map<std::string, float> status;
};

#endif
