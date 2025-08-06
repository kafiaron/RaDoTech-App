#include "Profile.h"

// Constructor
Profile::Profile(const QString& profileName, const QString& gender, int weight, int age, double height)
    : profileName(profileName), gender(gender), weight(weight), age(age), height(height), currentScanSession()
    {
        conductanceNormMin = 0;
        conductanceNormMax = 0;
    }

// Getters
QString Profile::getProfileName() const {
    return profileName;
}

QString Profile::getGender() const {
    return gender;
}

int Profile::getWeight() const {
    return weight;
}
double Profile::getHeight() const {
    return height;
}

QString Profile::getBirthDate() const {
    return birthDate;
}
int Profile::getAge() const{
    return age;
}

double Profile::getConductanceNormMin() const {
    return conductanceNormMin;
}

double Profile::getConductanceNormMax() const {
    return conductanceNormMax;
}

const ScanSession& Profile::getCurrentScanSession() const {
    return this->currentScanSession;
}

// Setters
void Profile::setProfileName(const QString& profileName) {
    this->profileName = profileName;
}

void Profile::setGender(const QString& gender) {
    this->gender = gender;
}

void Profile::setWeight(int weight) {
    this->weight = weight;
}

void Profile::setBirthDate(const QString& birthDate) {
    this->birthDate = birthDate;
}

void Profile::setHeight(double height) { this->height = height; }

void Profile::setAge(int age){this->age = age;}


void Profile::setCurrentScanSession(const ScanSession& session) {
    currentScanSession = session;
}

// Helper Function
void Profile::calculateConductanceRange() {
    auto setRange = [&](int min, int max) {
        conductanceNormMax = max;
        conductanceNormMin = min;
    };
    if (age < 10) {
        if (weight < 15) setRange(20, 40);
        else if (weight <= 30) setRange(25, 45);
        else setRange(35, 55);
    } else if (age < 20) {
        if (weight < 50) setRange(55, 75);
        else if (weight <= 70) setRange(60, 85);
        else setRange(50, 80);
    } else if (age < 40) {
        if (weight < 50) setRange(50, 70);
        else if (weight <= 70) setRange(55, 80);
        else setRange(50, 75);
    } else if (age < 60) {
        if (weight < 50) setRange(45, 65);
        else if (weight <= 70) setRange(50, 70);
        else setRange(45, 65);
    } else if (age < 80) {
        if (weight < 50) setRange(45, 65);
        else if (weight <= 70) setRange(50, 70);
        else setRange(45, 65);
    } else {
        if (weight < 50) setRange(35, 50);
        else if (weight <= 70) setRange(40, 55);
        else setRange(35, 50);
    }
}


