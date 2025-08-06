#include "User.h"
#include <QDebug>

// Constructor
User::User(int age, const QString& firstName, const QString& lastName, const QString& gender, int weight, double height, const QString& birthDate,const QString& email, const QString& password):
    age(age), firstName(firstName), lastName(lastName), gender(gender), weight(weight), height(height), birthDate(birthDate), email(email), password(password), profiles(){}

// Getters and Setters
QString User::getFirstName() const { return firstName; }
void User::setFirstName(const QString& firstName) { this->firstName = firstName; }

QString User::getLastName() const { return lastName; }
void User::setLastName(const QString& lastName) { this->lastName = lastName; }

QString User::getGender() const { return gender; }
void User::setGender(const QString& gender) { this->gender = gender; }

int User::getWeight() const { return weight; }
void User::setWeight(int weight) { this->weight = weight; }

double User::getHeight() const { return height; }
void User::setHeight(double height) { this->height = height; }

QString User::getBirthDate() const { return birthDate; }
void User::setBirthDate(const QString& birthDate) { this->birthDate = birthDate; }

QString User::getEmail() const { return email; }
void User::setEmail(const QString& email) { this->email = email; }

QString User::getPassword() const { return password; }
void User::setPassword(const QString& password) { this->password = password; }



// Getter for historical data
HistoricalData& User::getHistoricalData(){
    return historicalData;
}

// Setter for historical data
void User::setHistoricalData(const HistoricalData& history){
    historicalData = history;
}

// Method to save the current session to historical data
void User::saveCurrentSessionToHistory(const QString& name, const QString& date, const ScanSession& scan){
    // Add the current session to historical data
    historicalData.addScanSession(name, date, scan);
}

// Profile management
void User::addProfile(const Profile& profile) {
    if (profiles.size() < 5) {
        profiles.push_back(profile);
    }
}

// Function to remove profiles
void User::removeProfile(const QString& profileName) {
    auto it = std::remove_if(profiles.begin(), profiles.end(),
                             [&](const Profile& p) { return p.getProfileName() == profileName; });
    if (it != profiles.end()) {
        profiles.erase(it, profiles.end());
    }
}

// Getter for profiles
const std::vector<Profile>& User::getProfiles() {
    return profiles;
}

// Function to get profile by name
Profile* User::getProfileByName(const QString& profileName) {
    for (auto& profile : profiles) {
        if (profile.getProfileName() == profileName) {
            return &profile;
        }
    }
    return nullptr;
}

// Function to update profiles
void User::updateProfile(const QString& profileName, const QString& newProfileName, int newWeight, const QString& newGender, double newHeight, int age ) {
    for (auto& profile : profiles) {
        if (profile.getProfileName() == profileName) {
            // Update profile details
            profile.setProfileName(newProfileName);
            profile.setWeight(newWeight);
            profile.setGender(newGender);
            profile.setHeight(newHeight);
            profile.setAge(age);
            return;
        }
    }
}








