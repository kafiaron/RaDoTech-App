#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include "User.h"
#include "Diagrams.h"
#include "Indicators.h"
#include "Recommendations.h"

#include "Profile.h"
#include "ScanSession.h"
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <iostream>
#include <map>
#include <QDateTime>
#include <QString>
#include <string>
#include <QLineEdit>
#include <QTimer>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Page navigation
    void goToCreateProfilePage();
    void goToHomepage();
    void goToLoginPage();
    void goToMeasurePage();
    void toggleMenu();
    void goToProfilePage();
    void goToNewProfilePage();
    void goToBodyPage();
    void goToChartPage();
    void goToIndicatorPage();
    void goToRecommendPage();
    void goToHistoryPage();

    // Profile management
    void onEditProfileClicked(const Profile& profile);
    void saveUser();
    void onSaveProfileClicked();
    void addNewProfile();
    void deleteProfile();
    void verifyUser();
    void login();

    // Diagrams processing
    void colorDiagrams(std::map<std::string, float>);
    void adjustChart(std::map<std::string, float>);
    void adjustStatus(std::map<std::string, float>, std::map<std::string, float>);

    // Device functionality
    void handleSkinContact();
    void resetSkinContact();
    void decreaseBattery();
    void goToScanSessionPage();

    void onProfileSelected(int index);
    void onFinishScanClicked();

    // History functionality
    void populateProfileDropdown();
    void populateHistoryPage();
    void onHistoryTableItemClicked(int row);


private:
    Ui::MainWindow *ui;
    void setPlaceHolders();
    QPropertyAnimation *menuAnimation;

    // Track the currently logged-in user
    User* currentUser = nullptr;
    Profile* currentEditingProfile = nullptr;
    Diagrams* currentDiagram = nullptr;
    Indicators* currentIndicator = nullptr;
    Recommendations* currentRecommendation = nullptr;
    ScanSession* currentScanSession = nullptr;
    HistoricalData* history = nullptr;

    // Side menu ui
    void updateMenu();
    void menuSetup();

    void displayProfileButtons();
    void populateProfileBox();
    void resetScanInputs();

    QTimer* skinContactTimer;
    int batteryLevel;
    bool scanCheck = 0;

};

#endif
