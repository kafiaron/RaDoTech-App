#ifndef USER_H
#define USER_H

#include <QString>
#include <tuple>

#include "HistoricalData.h"
#include "Profile.h"

class User {
private:
    int age;
    QString firstName;
    QString lastName;
    QString gender;
    int weight;
    double height;
    QString birthDate;
    QString email;
    QString password;

    // Holds all past scan sessions
    HistoricalData historicalData;
    // Store user profiles
    std::vector<Profile> profiles;
public:
    // Constructors
    User(int age, const QString& firstName, const QString& lastName, const QString& gender,
         int weight, double height, const QString& birthDate,
         const QString& email, const QString& password);

    // Getters and Setters for User Information
    int getAge() const;
    void setAge(int age);

    QString getFirstName() const;
    void setFirstName(const QString& firstName);

    QString getLastName() const;
    void setLastName(const QString& lastName);

    QString getGender() const;
    void setGender(const QString& gender);

    int getWeight() const;
    void setWeight(int weight);

    double getHeight() const;
    void setHeight(double height);

    QString getBirthDate() const;
    void setBirthDate(const QString& birthDate);

    QString getEmail() const;
    void setEmail(const QString& email);

    QString getPassword() const;
    void setPassword(const QString& password);
    
    // Getter for historical data
    HistoricalData& getHistoricalData();

    // Method to save the session to historical data
    void setHistoricalData(const HistoricalData& history);

    // Method to save the session to historical data
    void saveCurrentSessionToHistory(const QString&, const QString&, const ScanSession&);


    // Profile management
    void addProfile(const Profile& profile);
    void removeProfile(const QString& profileName);
    const std::vector<Profile>& getProfiles();
    Profile* getProfileByName(const QString& profileName);
    void updateProfile(const QString& profileName, const QString& newProfileName, int newWeight, const QString& newGender, double newHeight, int age);


};

#endif // USER_H
