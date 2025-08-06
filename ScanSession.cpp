#include "ScanSession.h"
#include <stdexcept> // For exception handling

// Constructor
ScanSession::ScanSession(){
    // Initialize current time to the system time
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);

    // Format date and time: YYYY-MM-DD HH:MM:SS
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    // Assign formatted date and time
    date = QString::fromStdString(oss.str()); 

    // Initialize meridian points with default values
    initializeMeridianPoints();
}

// Initialize meridian points with default values (e.g., 0.0)
void ScanSession::initializeMeridianPoints() {
    std::vector<std::string> keys = {
        "h1Left", "h2Left", "h3Left", "h4Left", "h5Left", "h6Left",
        "f1Left", "f2Left", "f3Left", "f4Left", "f5Left", "f6Left",
        "h1Right", "h2Right", "h3Right", "h4Right", "h5Right", "h6Right",
        "f1Right", "f2Right", "f3Right", "f4Right", "f5Right", "f6Right"
    };

    for (const std::string& key : keys) {
        meridianPoints[key] = 0.0f;
    }
}


// Getter for meridian points as a vector of values
std::map<std::string, float> ScanSession::getMeridianPoints() const {
    return this->meridianPoints;
}

// Setter to update a specific meridian point
void ScanSession::setMeridianPoint(const std::string& pointKey, float value) {
    auto it = meridianPoints.find(pointKey);
    if (it != meridianPoints.end()) {
        it->second = value; // Update the value
    } else {
        throw std::invalid_argument("Invalid meridian point key: " + pointKey);
    }
}

// Getter for a specific meridian point
float ScanSession::getMeridianPoint(const std::string& pointKey) const {
    auto it = meridianPoints.find(pointKey);
    if (it != meridianPoints.end()) {
        return it->second; // Return the value
    } else {
        throw std::invalid_argument("Invalid meridian point key: " + pointKey);
    }
}

// Getter for date
QString ScanSession::getDate() const {
    return date;
}
