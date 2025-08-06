#ifndef DIAGRAMS_H
#define DIAGRAMS_H

#include <QString>
#include <map>

class Diagrams{
public:
    //Constructor
    Diagrams(std::map<std::string, float>);
    //Main functions
    void calculateBodyChartData(double, double);
    void calculateBarChartData(double, double);
    //Getters
    const std::map<std::string, float>& getbodyChartData();
    const std::map<std::string, float>& getbarChartData();
private:
    std::map<std::string, float> meridianPoint;
    std::map<std::string, float> bodyChartData;
    std::map<std::string, float> barChartData;
};

#endif
