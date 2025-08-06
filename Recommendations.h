#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <QString>
#include <map>
#include <vector>

class Recommendations{
public:
    //Constructor
    Recommendations(std::map<std::string, float>);
    //Main functions
    void calculateAbnormities(double, double);
    //getter
    std::vector<std::string> getRecommendations();
private:
    std::map<std::string, float> meridianPoint;
    std::vector<std::string> recommend;
};

#endif
