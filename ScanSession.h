#ifndef SCANSESSION_H
#define SCANSESSION_H

#include <QString>
#include <map>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

class ScanSession {
private:
    // Map to store meridian points and their values
    std::map<std::string, float> meridianPoints;
    // the date when scan session was created
    QString date;
    // Helper function to initialize meridian points
    void initializeMeridianPoints();

public:
    // Constructor
    ScanSession();

    // Getter for meridian points
    std::map<std::string, float> getMeridianPoints() const;

    // Setter to update a specific meridian point
    void setMeridianPoint(const std::string& pointKey, float value);

    // Getter for a specific meridian point
    float getMeridianPoint(const std::string& pointKey) const;

    // Destructor
    ~ScanSession() = default;

    // Declaration of the getter for the date
    QString getDate() const;


};

#endif // SCANSESSION_H
