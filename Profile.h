#ifndef PROFILE_H
#define PROFILE_H

#include "ScanSession.h"

#include <QString>

class Profile {
private:
    QString profileName;
    QString gender;
    int weight;
    QString birthDate;
    int age;
    double height;
    double conductanceNormMin;
    double conductanceNormMax;
    ScanSession currentScanSession;
public:
    // Constructor
    Profile(const QString& profileName, const QString& gender, int weight, int age, double height);

    // Getters
    QString getProfileName() const;
    QString getGender() const;
    int getWeight() const;
    QString getBirthDate() const;
    double getHeight() const;
    int getAge() const;
    double getConductanceNormMin() const;
    double getConductanceNormMax() const;
    const ScanSession& getCurrentScanSession() const;

    // Setters
    void setProfileName(const QString& profileName);
    void setGender(const QString& gender);
    void setWeight(int weight);
    void setBirthDate(const QString& birthDate);
    void setHeight(double height);
    void setAge(int age);
    void setCurrentScanSession(const ScanSession& session);

    //Helper function
    void calculateConductanceRange();
};

#endif // PROFILE_H
