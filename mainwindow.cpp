#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , batteryLevel(100)
{
    ui->setupUi(this);

    // Set up the connections for page navigation
    connect(ui->createProfileButton, &QPushButton::clicked, this, &MainWindow::goToCreateProfilePage);
    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::goToHomepage);
    connect(ui->backButton_2, &QPushButton::clicked, this, &MainWindow::goToHomepage);
    connect(ui->backButton_3, &QPushButton::clicked, this, &MainWindow::goToMeasurePage);
    connect(ui->profileButton, &QPushButton::clicked, this, &MainWindow::goToProfilePage);
    connect(ui->newProfileButton, &QPushButton::clicked, this, &MainWindow::goToNewProfilePage);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveUser);
    connect(ui->newProfileBackButton, &QPushButton::clicked, this, &MainWindow::goToProfilePage);
    connect(ui->homeButton, &QPushButton::clicked, this, &MainWindow::goToMeasurePage);
    connect(ui->saveNewProfileButton, &QPushButton::clicked, this, &MainWindow::onSaveProfileClicked);
    connect(ui->addProfileButton, &QPushButton::clicked, this, &MainWindow::addNewProfile);
    connect(ui->deleteProfileButton, &QPushButton::clicked, this, &MainWindow::deleteProfile);

    connect(ui->backButtonBody, &QPushButton::clicked, this, &MainWindow::goToMeasurePage);
    connect(ui->backButtonChart, &QPushButton::clicked, this, &MainWindow::goToMeasurePage);
    connect(ui->backButtonIn, &QPushButton::clicked, this, &MainWindow::goToMeasurePage);
    connect(ui->backButtonRec, &QPushButton::clicked, this, &MainWindow::goToMeasurePage);

    connect(ui->bodyButtonBody, &QPushButton::clicked, this, &MainWindow::goToBodyPage);
    connect(ui->chartButtonBody, &QPushButton::clicked, this, &MainWindow::goToChartPage);
    connect(ui->diagramButtonBody, &QPushButton::clicked, this, &MainWindow::goToBodyPage);
    connect(ui->indicatorButtonBody, &QPushButton::clicked, this, &MainWindow::goToIndicatorPage);
    connect(ui->recommendButtonBody, &QPushButton::clicked, this, &MainWindow::goToRecommendPage);

    connect(ui->bodyButtonChart, &QPushButton::clicked, this, &MainWindow::goToBodyPage);
    connect(ui->chartButtonChart, &QPushButton::clicked, this, &MainWindow::goToChartPage);
    connect(ui->diagramButtonChart, &QPushButton::clicked, this, &MainWindow::goToBodyPage);
    connect(ui->indicatorButtonChart, &QPushButton::clicked, this, &MainWindow::goToIndicatorPage);
    connect(ui->recommendButtonChart, &QPushButton::clicked, this, &MainWindow::goToRecommendPage);

    connect(ui->diagramButtonIn, &QPushButton::clicked, this, &MainWindow::goToBodyPage);
    connect(ui->indicatorButtonIn, &QPushButton::clicked, this, &MainWindow::goToIndicatorPage);
    connect(ui->recommendButtonIn, &QPushButton::clicked, this, &MainWindow::goToRecommendPage);

    connect(ui->diagramButtonRec, &QPushButton::clicked, this, &MainWindow::goToBodyPage);
    connect(ui->indicatorButtonRec, &QPushButton::clicked, this, &MainWindow::goToIndicatorPage);
    connect(ui->recommendButtonRec, &QPushButton::clicked, this, &MainWindow::goToRecommendPage);

    connect(ui->detailButton, &QPushButton::clicked, this, &MainWindow::goToBodyPage);
    connect(ui->logOutButton, &QPushButton::clicked, this, &MainWindow::goToHomepage);
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::verifyUser);
    connect(ui->startLoginButton, &QPushButton::clicked, this, &MainWindow::login);
    connect(ui->historyButton, &QPushButton::clicked, this, &MainWindow::goToHistoryPage);
    connect(ui->measureNowButton, &QPushButton::clicked, this, &MainWindow::goToScanSessionPage);
    connect(ui->measureButton, &QPushButton::clicked, this, &MainWindow::goToScanSessionPage);


    skinContactTimer = new QTimer(this);
    skinContactTimer->setSingleShot(true);
    connect(skinContactTimer, &QTimer::timeout, this, &MainWindow::resetSkinContact);
    // Disabled the text boxes only on the scanner page after value has been input
    QFrame* scannerFrame = this->findChild<QFrame*>("scannerFrame"); 
    if (scannerFrame) {
        QList<QLineEdit*> inputs = scannerFrame->findChildren<QLineEdit*>();
        for (QLineEdit* input : inputs) {
            connect(input, &QLineEdit::returnPressed, this, &MainWindow::handleSkinContact);
            connect(input, &QLineEdit::returnPressed, this, &MainWindow::decreaseBattery);
        }
    }
    ui->progressBar->setValue(batteryLevel);

    connect(ui->profileBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onProfileSelected);
    connect(ui->finishScanButton, &QPushButton::clicked, this, &MainWindow::onFinishScanClicked);
    connect(ui->historyTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onHistoryTableItemClicked);

    ui->menuButton->setVisible(false);
    menuSetup();
}


// Function to populate profile box
void MainWindow::populateProfileDropdown() {
    if (!currentUser) {
        qDebug() << "No user logged in.";
        return;
    }
    ui->profileBox->clear();
    ui->profileBox->setPlaceholderText("Select Profile");

    for (const auto& profile : currentUser->getProfiles()) {
        QString profileName = profile.getProfileName();
        if (!profileName.isEmpty()) {
            ui->profileBox->addItem(profileName);
        }
    }
}

// Function to save the scann session information
void MainWindow::onFinishScanClicked() {
    if (!currentEditingProfile) {
        QMessageBox::warning(this, "Error", "No profile selected!");
        return;
    }

    currentScanSession = new ScanSession;
    QList<QLineEdit*> inputs = ui->scansessionpage->findChildren<QLineEdit*>();
    for (QLineEdit* input : inputs) {
        QString pointName = input->objectName();
        bool ok;
        float value = input->text().toFloat(&ok);
        if (ok) {
            currentScanSession->setMeridianPoint(pointName.toStdString(), value);
        } else {
            input->setStyleSheet("border: 1px solid red;");
        }
    }

    currentUser->saveCurrentSessionToHistory(currentEditingProfile->getProfileName(), currentScanSession->getDate(), *currentScanSession);
    currentEditingProfile->setCurrentScanSession(*currentScanSession);
    currentEditingProfile->calculateConductanceRange();
    resetScanInputs();
    QMessageBox::information(this, "Success", "Scan session saved successfully!");
    scanCheck = 1;

    goToBodyPage();
}

// Function to reset the input fields once the scansession has finished
void MainWindow::resetScanInputs() {
    QList<QLineEdit*> inputs = ui->scansessionpage->findChildren<QLineEdit*>();
    for (QLineEdit* input : inputs) {
        input->clear(); // Clear text
        input->setEnabled(true);
        input->setStyleSheet(
            "QLineEdit {"
            "    background-color: white;"
            "    border: 2px solid #FFA500;"
            "    border-radius: 10px;"
            "    padding: 5px;"
            "}"
        );
    }
    resetSkinContact();

}

// Function to save the selected profile
void MainWindow::onProfileSelected(int index) {
    if (!currentUser) {
        QMessageBox::warning(this, "Error", "No user is logged in.");
        return;
    }
    if (index == -1) {
        currentEditingProfile = nullptr;
        return;
    }
    QString profileName = ui->profileBox->currentText();
    currentEditingProfile = currentUser->getProfileByName(profileName);

}

// Function to navigate to scan session page
void MainWindow::goToScanSessionPage() {
    if (currentUser) {
        populateProfileDropdown();
    } else {
        qDebug() << "No user logged in to populate profiles.";
    }
    ui->stackedWidget->setCurrentWidget(ui->scansessionpage);
    resetScanInputs();
    updateMenu();
}

// Function to show skin contact
void MainWindow::handleSkinContact()
{
    ui->skinContactIndicator->setStyleSheet("QLabel { background-color: green; }");
    ui->skinContactIndicator->setText("On Skin");
    QLineEdit *input = qobject_cast<QLineEdit *>(sender());
    if (input) {
        input->setStyleSheet("QLineEdit { background-color: grey; }");
        input->setEnabled(false);
    }
    skinContactTimer->start(2000);
}

// Function to reset skin contact indicator
void MainWindow::resetSkinContact()
{
    ui->skinContactIndicator->setStyleSheet("QLabel { background-color: red; }");
    ui->skinContactIndicator->setText("Off Skin");
}

// Function to display battery status
void MainWindow::decreaseBattery()
{
    int decrement = 100 / 24;
    batteryLevel = std::max(0, batteryLevel - decrement);
    ui->progressBar->setValue(batteryLevel);

    if (batteryLevel > 50) {
        ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    } else if (batteryLevel > 20) {
        ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: yellow; }");
    } else if (batteryLevel > 0) {
        ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    } else {
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setWindowTitle("Battery Dead");
        msgBox->setText("RaDoTech Pen Battery Dead. Shutting Down");
        msgBox->setStandardButtons(QMessageBox::NoButton);
        msgBox->show();
        QTimer::singleShot(3000, [msgBox]() {
            msgBox->close();
            qDebug() << "Battery is dead. Closing application...";
            QApplication::quit();
        });
    }
}


// Fucntion to populate history page with scan sessions
void MainWindow::populateHistoryPage() {
    ui->historyTable->clear();
    ui->historyTable->setColumnCount(2);
    ui->historyTable->setColumnWidth(0, 200);
    ui->historyTable->setHorizontalHeaderLabels({"Date", "Profile Name"});
    ui->historyTable->setRowCount(0);
    int row = 0;
    for (const auto& session : currentUser->getHistoricalData().getAllScanSessions()) {
        const QString& profileName = std::get<0>(session); // Profile name from tuple
        const QString& date = std::get<1>(session);       // Date from tuple
        ui->historyTable->insertRow(row);
        ui->historyTable->setItem(row, 0, new QTableWidgetItem(date));
        ui->historyTable->setItem(row, 1, new QTableWidgetItem(profileName));
        ++row;
    }
    ui->historyTable->horizontalHeader()->setStretchLastSection(true);
    ui->historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// Function to display previous scan session by profile
void MainWindow::onHistoryTableItemClicked(int row) {
    QString sessionDate = ui->historyTable->item(row, 0)->text();
    QString profileName = ui->historyTable->item(row, 1)->text();
    for(const auto& historicalData : currentUser->getHistoricalData().getAllScanSessions()){
        QString storedName = std::get<0>(historicalData);
        QString storedDate = std::get<1>(historicalData);
        if (storedName == profileName && storedDate == sessionDate){
            currentEditingProfile = currentUser->getProfileByName(profileName);
            currentEditingProfile->setCurrentScanSession(std::get<2>(historicalData));
            goToBodyPage();
            return;
        }
    }
    QMessageBox::warning(this, "Error", "Could not find the selected scan session.");
}

// Fucntions to navigate to the body page
void MainWindow::goToBodyPage()
{
    ui->stackedWidget->setCurrentWidget(ui->bodyPage);
    if(currentEditingProfile && scanCheck){
        currentDiagram = new Diagrams(currentEditingProfile->getCurrentScanSession().getMeridianPoints());
        currentDiagram->calculateBodyChartData(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        std::map<std::string, float> data = currentDiagram->getbodyChartData();
        colorDiagrams(data);
    }
    if (ui->menuFrame->x() >= 0) {
            toggleMenu();
    }
    updateMenu();
}

// Function to navigate to history page
void MainWindow::goToHistoryPage() {
    ui->stackedWidget->setCurrentWidget(ui->historyPage);
    populateHistoryPage();
    if (ui->menuFrame->x() >= 0) {
            toggleMenu();
    }
}


// Function to display side menu
void MainWindow::toggleMenu()
{
    int desiredY = 90;
    if (ui->menuFrame->x() < 0) {
        menuAnimation->setStartValue(ui->menuFrame->pos());
        menuAnimation->setEndValue(QPoint(0, desiredY));
    } else {
        menuAnimation->setStartValue(ui->menuFrame->pos());
        menuAnimation->setEndValue(QPoint(-ui->menuFrame->width(), desiredY));
    }
    menuAnimation->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Function to navigate to the Create Profile page
void MainWindow::goToCreateProfilePage()
{
    ui->stackedWidget->setCurrentWidget(ui->createprofilepage);
    ui->firstInput->clear();
    ui->lastInput->clear();
    ui->femaleRadioButton->setChecked(false);
    ui->maleRadioButton->setChecked(false);
    ui->weightInput->clear();
    ui->heightInput->clear();
    ui->dateOfBirthInput->clear();
    ui->emailInput->clear();
    ui->passwordInput->clear();
    setPlaceHolders();
    updateMenu();

}

// Function to navigate to the Login page
void MainWindow::goToLoginPage()
{
    ui->stackedWidget->setCurrentWidget(ui->loginpage);
    ui->emailLogin->clear();
    ui->passwordLogin->clear();
    ui->emailLogin->setPlaceholderText("Email");
    ui->passwordLogin->setPlaceholderText("Password");
    ui->menuButton->hide();
    updateMenu();
}

// Function to navigate back to the Homepage
void MainWindow::goToHomepage()
{
    ui->stackedWidget->setCurrentWidget(ui->homepage);
    updateMenu();
}

// Function to navigate to measure page
void MainWindow::goToMeasurePage()
{
    ui->stackedWidget->setCurrentWidget(ui->measurePage);
    ui->userName->setText(currentUser->getFirstName() + " " + currentUser->getLastName());
    ui->userName->setStyleSheet("QLabel { font-weight: bold; font-size: 28px; color: white; }");
    ui->userName->setAlignment(Qt::AlignCenter);

    if (ui->menuFrame->x() >= 0) {
            toggleMenu();
    }
    updateMenu();

}

// Function to navigate to the Profile page
void MainWindow::goToProfilePage()
{
    ui->stackedWidget->setCurrentWidget(ui->profilePage);
    displayProfileButtons();
    if (ui->menuFrame->x() >= 0) {
            toggleMenu();
    }
    updateMenu();
    ui->menuButton->setVisible(true);
}

// Function to navigate to the new profile page
void MainWindow::goToNewProfilePage()
{

    // Check if the current user already has 5 profiles
    if (currentUser->getProfiles().size() >= 5) {
        QMessageBox::warning(this, "Profile Limit Reached", "You can only have up to 5 profiles.");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->editProfilePage);
    ui->deleteProfileButton->setVisible(false);
    ui->saveNewProfileButton->setVisible(false);
    ui->addProfileButton->setVisible(true);
    ui->newName->clear();
    ui->newHeightInput->clear();
    ui->newWeightInput->clear();
    ui->ageInput->clear();
    ui->maleRadioButton->setChecked(false);
    ui->femaleRadioButton->setChecked(false);
    updateMenu();
}

// Function to update the menu for the current page
void MainWindow::updateMenu()
{
    // Make the menuButton visible only on homepage and profilePage
    if (ui->stackedWidget->currentWidget() == ui->profilePage ||
        ui->stackedWidget->currentWidget() == ui->measurePage) {
        ui->menuButton->setVisible(true);
    } else {
        ui->menuButton->setVisible(false);
        if (ui->menuFrame->x() >= 0) {
           toggleMenu();
        }
    }
}

// Function to save the user master account
void MainWindow::saveUser()
{
    QString firstName = ui->firstInput->text();
    QString lastName = ui->lastInput->text();
    int weight = ui->weightInput->text().toInt();
    double height = ui->heightInput->text().toDouble();
    QString birthDate = ui->dateOfBirthInput->text();
    QString email = ui->emailInput->text();
    QString password = ui->passwordInput->text();
    QString gender;
    if (ui->maleRadioButton->isChecked()) {
          gender = "Male";
    } else if (ui->femaleRadioButton->isChecked()) {
         gender = "Female";
    }
    int age = ui->age->text().toInt();

    // Validate inputs
       if (firstName.isEmpty() || lastName.isEmpty() || birthDate.isEmpty() ||
           email.isEmpty() || password.isEmpty() ||
           weight <= 0 || height <= 0 || age <= 0 || gender.isEmpty()) {

           QMessageBox::warning(this, "Invalid Input", "All fields must be filled and valid!");
           return;
       }

    currentUser = new User(age, firstName, lastName, gender, weight, height, birthDate, email, password);
    history = new HistoricalData();
    currentUser->setHistoricalData(*history);

    QString profileName = firstName + lastName;
    Profile initialProfile(profileName, gender, weight, age, height);
    initialProfile.calculateConductanceRange();
    currentUser->addProfile(initialProfile);
    goToMeasurePage();
}

// Function display profiles as buttons on the profile page
void MainWindow::displayProfileButtons() {

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->profileLists->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->profileLists);
        ui->profileLists->setLayout(layout);
    }

    // Clear existing buttons from the layout
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Loop through the user's profiles and create buttons for each
    for (const auto& profile : currentUser->getProfiles()) { // Loop through profiles
        QPushButton* profileButton = new QPushButton(profile.getProfileName(), ui->profileLists);
        profileButton->setObjectName(profile.getProfileName());
        profileButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        // Set a stylesheet for rounded borders and white font
        profileButton->setStyleSheet(
            "QPushButton {"
            "    background-color: white;"
            "    color: black;"
            "    border-radius: 15px;"
            "    padding: 10px;"
            "    font-size: 16px;"
            "}"
            "QPushButton:hover {"
            "    background-color: light orange;"
            "}"
            "QPushButton:pressed {"
            "    background-color: grey;"
            "}"
        );

        // Connect the button to the editing function with a reference to the profile
        connect(profileButton, &QPushButton::clicked, [this, &profile]() {onEditProfileClicked(profile);});

        // Add the button to the layout
        layout->addWidget(profileButton);
    }
}

// Function to populate the drop down menu
void MainWindow::populateProfileBox()
{
    ui->profileBox->clear();
    if(currentUser){
        for (const Profile& profile : currentUser->getProfiles()) {
            ui->profileBox->addItem(profile.getProfileName());
        }
    }
}

// UpdateProfile function
void MainWindow::onEditProfileClicked(const Profile& profile) {
    // Switch to the edit profile page
    ui->stackedWidget->setCurrentWidget(ui->editProfilePage);
    ui->deleteProfileButton->setVisible(true);


    currentEditingProfile = const_cast<Profile*>(&profile);

    // Populate fields with the profile's data
    ui->newName->setText(profile.getProfileName());
    ui->newHeightInput->setText(QString::number(profile.getHeight()));
    ui->newWeightInput->setText(QString::number(profile.getWeight()));
    ui->ageInput->setText(QString::number(profile.getAge()));

    // Set the gender radio button
    if (profile.getGender() == "Male") {
        ui->newMaleRadioButton->setChecked(true);
    } else {
        ui->newFemaleRadioButton->setChecked(true);
    }
    ui->saveNewProfileButton->setVisible(true);
    ui->addProfileButton->setVisible(false);

    updateMenu();

}

// Function to save an existing profile
void MainWindow::onSaveProfileClicked() {
    // Read values from UI fields
    QString newName = ui->newName->text();
    double newHeight = ui->newHeightInput->text().toDouble();
    int newWeight = ui->newWeightInput->text().toInt();
    int age = ui->ageInput->text().toInt();
    QString newGender = ui->maleRadioButton->isChecked() ? "Male" : "Female";

    // Update the profile through the User object
    currentUser->updateProfile(currentEditingProfile->getProfileName(), newName, newWeight, newGender, newHeight, age);
    currentUser->getProfileByName(currentEditingProfile->getProfileName())->calculateConductanceRange();
    // Update the UI to reflect changes
    displayProfileButtons();

    // Switch back to the profile list page
   ui->stackedWidget->setCurrentWidget(ui->profilePage);
   ui->menuButton->setVisible(true);
}


// Function to add a new profile
    void MainWindow::addNewProfile()
    {

        QString newName = ui->newName->text();
        double newHeight = ui->newHeightInput->text().toDouble();
        int newWeight = ui->newWeightInput->text().toInt();
        int age = ui->ageInput->text().toInt();
        QString newGender = ui->maleRadioButton->isChecked() ? "Male" : "Female";


        Profile newProfile(newName, newGender, newWeight, age, newHeight);
        newProfile.calculateConductanceRange();
        ui->addProfileButton->setVisible(true);
        currentUser->addProfile(newProfile);
        displayProfileButtons();
        goToProfilePage();
    }


// Function to delete a profile
void MainWindow::deleteProfile()
{
    QString nameToDelete = ui->newName->text();
    currentUser->removeProfile(nameToDelete);
    currentUser->getHistoricalData().removeScanSessions(nameToDelete);
    if( currentUser->getProfiles().size() == 0){
        currentEditingProfile = nullptr;
    }
    goToProfilePage();

}

// Function makes sure that an account exists before login
void MainWindow::verifyUser()
{
    // Check if a user is already created (currentUser is valid)
    if (currentUser == nullptr) {
        // If no user exists, take them to the Create Profile page
        goToCreateProfilePage();
        return;
    }
    goToLoginPage();
}


// Login function
void MainWindow::login()
{
    QString enteredEmail = ui->emailLogin->text();
    QString enteredPassword = ui->passwordLogin->text();

    // Verify email and password
    if (currentUser && currentUser->getEmail() == enteredEmail && currentUser->getPassword() == enteredPassword) {
        // Login successful; navigate to the Measure page
        goToMeasurePage();
        ui->statusLabel->clear();
    } else {
        // Login failed; display an error message
        ui->statusLabel->setText("Invalid email or password. Please try again.");
    }
}

// Function to intialize side menu
void MainWindow::menuSetup()
{
    // Set initial position for the menuFrame
    ui->menuFrame->setParent(ui->centralwidget);
    ui->menuFrame->move(-ui->menuFrame->width(), 90);
    ui->menuFrame->raise();

    // Set up the animation for the menu
    menuAnimation = new QPropertyAnimation(ui->menuFrame, "pos");
    menuAnimation->setDuration(300);
    menuAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    ui->menuButton->setParent(ui->centralwidget);
    ui->menuButton->raise();
    ui->menuButton->move(10, 10);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleMenu);
    ui->menuButton->hide();
}

// Set placeholders for empty input fields
void MainWindow::setPlaceHolders()
{
    ui->firstInput->setPlaceholderText("Firstname");
    ui->lastInput->setPlaceholderText("Lastname");
    ui->weightInput->setPlaceholderText("Your weight");
    ui->heightInput->setPlaceholderText("Your height");
    ui->dateOfBirthInput->setPlaceholderText("DD-MM-YYYY");
    ui->emailInput->setPlaceholderText("Login");
    ui->passwordInput->setPlaceholderText("12345");
    ui->age->setPlaceholderText("Age");
}

// Go to chart page
void MainWindow::goToChartPage()
{
    ui->stackedWidget->setCurrentWidget(ui->chartPage);
    if(currentEditingProfile && scanCheck){
        currentDiagram = new Diagrams(currentEditingProfile->getCurrentScanSession().getMeridianPoints());
        currentDiagram->calculateBodyChartData(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        currentDiagram->calculateBarChartData(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        std::map<std::string, float> data = currentDiagram->getbodyChartData();
        colorDiagrams(data);
        data = currentDiagram->getbarChartData();
        adjustChart(data);
    }
    if (ui->menuFrame->x() >= 0) {
            toggleMenu();
    }
    updateMenu();
}

// Go to indicators page
void MainWindow::goToIndicatorPage()
{
    ui->stackedWidget->setCurrentWidget(ui->indicatorPage);
    if(currentEditingProfile && scanCheck){
        currentIndicator = new Indicators(currentEditingProfile->getCurrentScanSession().getMeridianPoints());
        currentIndicator->calculateEnergyLevel(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        currentIndicator->calculateImmuneSystem(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        currentIndicator->calculateMetabolism(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        currentIndicator->calculatePsychoEmotional(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        currentIndicator->calculateMusculoskeletal(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        currentIndicator->calculateLeftMeridian();
        currentIndicator->calculateRightMeridian();
        currentIndicator->calculateUpperMeridian();
        currentIndicator->calculateLowerMeridian();

        std::map<std::string, float> data = currentIndicator->getProcessedData();
        std::map<std::string, float> data2 = currentIndicator->getStatus();
        adjustStatus(data, data2);
    }
    if (ui->menuFrame->x() >= 0) {
            toggleMenu();
    }
    updateMenu();
}

// Go to recommended page
void MainWindow::goToRecommendPage()
{
    ui->stackedWidget->setCurrentWidget(ui->recommendationPage);
    if(currentEditingProfile && scanCheck){
        currentRecommendation = new Recommendations(currentEditingProfile->getCurrentScanSession().getMeridianPoints());
        currentRecommendation->calculateAbnormities(currentEditingProfile->getConductanceNormMax(), currentEditingProfile->getConductanceNormMin());
        std::vector<std::string> data = currentRecommendation->getRecommendations();

        for (const auto& str : data) {
            ui->recOutput->append(QString::fromStdString(str));
        }
    }
    if (ui->menuFrame->x() >= 0) {
            toggleMenu();
    }
    updateMenu();
}

// Set the diagrams
void MainWindow::colorDiagrams(std::map<std::string, float> data){
    //0 - blue, 1 - red, 2 - green
    switch(int(data["lungL"])){
        case 0: ui->lungLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->lungLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->lungLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->lungLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->lungLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->lungLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->lungLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->lungLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->lungLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->lungLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->lungLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->lungLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["lungR"])){
        case 0: ui->lungRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->lungRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->lungRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->lungRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->lungRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->lungRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->lungRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->lungRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->lungRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->lungRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->lungRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->lungRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["periL"])){
        case 0: ui->immuneLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->immuneLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->immuneLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->immuneLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->immuneLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->immuneLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->immuneLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->immuneLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->immuneLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["periR"])){
        case 0: ui->immuneRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->immuneRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->immuneRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->immuneRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->immuneRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->immuneRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->immuneRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->immuneRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->immuneRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["heartL"])){
        case 0: ui->heartLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->heartLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->heartLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->heartLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->heartLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->heartLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->heartLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->heartLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->heartLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop: 1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->heartLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->heartLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->heartLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["heartR"])){
        case 0: ui->heartRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->heartRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->heartRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->heartRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->heartRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->heartRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->heartRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->heartRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->heartRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->heartRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->heartRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->heartRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["smallInL"])){
        case 0: ui->smallIntestLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->smallIntestLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->smallIntestLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->smallIntestLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->smallIntestLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->smallIntestLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->smallIntestLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->smallIntestLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->smallIntestLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->smallIntestLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->smallIntestLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->smallIntestLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["smallInR"])){
        case 0: ui->smallIntestRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->smallIntestRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->smallIntestRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->smallIntestRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->smallIntestRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->smallIntestRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->smallIntestRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->smallIntestRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->smallIntestRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->smallIntestRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->smallIntestRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->smallIntestRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["lymphL"])){
        case 0: ui->vesselLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:1, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->vesselLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->vesselLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->vesselLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->vesselLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:1, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->vesselLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->vesselLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->vesselLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->vesselLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:1, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->vesselLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->vesselLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->vesselLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["lymphR"])){
        case 0: ui->vesselRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:1, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->vesselRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->vesselRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->vesselRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->vesselRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:1, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->vesselRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->vesselRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->vesselRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->vesselRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:1, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->vesselRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->vesselRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->vesselRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["largeInL"])){
        case 0: ui->largeIntestLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->largeIntestLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->largeIntestLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->largeIntestLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->largeIntestLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->largeIntestLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->largeIntestLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->largeIntestLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->largeIntestLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->largeIntestLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->largeIntestLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->largeIntestLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["largeInR"])){
        case 0: ui->largeIntestRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->largeIntestRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->largeIntestRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->largeIntestRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->largeIntestRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->largeIntestRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->largeIntestRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->largeIntestRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->largeIntestRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->largeIntestRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->largeIntestRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->largeIntestRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["pancL"])){
        case 0: ui->pancLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->pancLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->pancLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->pancLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->pancLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->pancLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->pancLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->pancLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->pancLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->pancLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->pancLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->pancLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["pancR"])){
        case 0: ui->pancRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->pancRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->pancRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->pancRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->pancRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->pancRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->pancRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->pancRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->pancRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->pancRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->pancRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->pancRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["liverL"])){
        case 0: ui->liverLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->liverLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->liverLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->liverLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->liverLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->liverLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->liverLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->liverLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->liverLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->liverLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->liverLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->liverLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["liverR"])){
        case 0: ui->liverRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->liverRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->liverRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->LiverRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->liverRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->liverRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->liverRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->LiverRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->liverRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->liverRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->liverRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->LiverRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["kidneyL"])){
        case 0: ui->kidneyLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->kidneyLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->kidneyLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->kidneyLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->kidneyLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->kidneyLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->kidneyLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->kidneyLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->kidneyLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->kidneyLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->kidneyLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->kidneyLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["kidneyR"])){
        case 0: ui->kidneyRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->kidneyRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->kidneyRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->kidneyRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->kidneyRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->kidneyRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->kidneyRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->kidneyRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->kidneyRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->kidneyRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->kidneyRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->kidneyRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["bladderL"])){
        case 0: ui->bladderLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->bladderLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->bladderLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->bladderLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->bladderLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->bladderLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->bladderLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->bladderLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->bladderLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->bladderLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->bladderLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->bladderLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["bladderR"])){
        case 0: ui->bladderRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->bladderRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->bladderRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->bladderRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->bladderRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->bladderRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->bladderRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->bladderRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->bladderRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->bladderRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->bladderRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->bladderRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["gBladderL"])){
        case 0: ui->gallbladderLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->gallbladderLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->gallbladderLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->gallbladderLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->gallbladderLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->gallbladderLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->gallbladderLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->gallbladderLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->gallbladderLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->gallbladderLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->gallbladderLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->gallbladderLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["gBladderR"])){
        case 0: ui->gallbladderRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->gallbladderRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->gallbladderRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->gallbladderRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->gallbladderRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->gallbladderRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->gallbladderRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->gallbladderRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->gallbladderRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:0, x1:1, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->gallbladderRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:1, y2:0, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->gallbladderRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->gallbladderRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["stomachL"])){
        case 0: ui->stomachLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->stomachLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,150,255,255), stop:0 rgba(0,0,0,0));}");
                ui->stomachLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->stomachLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->stomachLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->stomachLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(255,0,0,255), stop:0 rgba(0,0,0,0));}");
                ui->stomachLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->stomachLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->stomachLeft->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->stomachLeft2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:1 rgba(0,255,0,255), stop:0 rgba(0,0,0,0));}");
                  ui->stomachLeftBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->stomachLeftBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
    switch(int(data["stomachR"])){
        case 0: ui->stomachRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->stomachRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,150,255,255), stop:1 rgba(0,0,0,0));}");
                ui->stomachRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");
                ui->stomachRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,150,255);}");break;
        case 1: ui->stomachRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->stomachRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(255,0,0,255), stop:1 rgba(0,0,0,0));}");
                ui->stomachRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");
                ui->stomachRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(255,0,0);}");break;
        default:  ui->stomachRight->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->stomachRight2->setStyleSheet("QLabel{padding: 0px; background: qlineargradient(spread: pad, x2:0, y2:1, x1:0, y1:0, stop:0 rgba(0,255,0,255), stop:1 rgba(0,0,0,0));}");
                  ui->stomachRightBar1->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");
                  ui->stomachRightBar2->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center; background-color: #ffffff;} QProgressBar::chunk{background-color: rgb(0,255,0);}");break;
    }
}

// Function to adjust chart
void MainWindow::adjustChart(std::map<std::string, float> data){
    if(data["h1Left"] > 100 && data["h1Left"] < 200){ui->lungLeftBar1->setValue(100); ui->lungLeftBar2->setValue(int(data["h1Left"]-100));} else if(data["h1Left"] >= 200 ){ui->lungLeftBar1->setValue(100); ui->lungLeftBar2->setValue(100);} else{ui->lungLeftBar1->setValue(int(data["h1Left"])); ui->lungLeftBar2->setValue(0);}
    if(data["h1Right"] > 100 && data["h1Right"] < 200){ui->lungRightBar1->setValue(100); ui->lungRightBar2->setValue(int(data["h1Right"]-100));} else if(data["h1Right"] >= 200 ){ui->lungRightBar1->setValue(100); ui->lungRightBar2->setValue(100);} else{ui->lungRightBar1->setValue(int(data["h1Right"])); ui->lungRightBar2->setValue(0);}
    if(data["h2Left"] > 100 && data["h2Left"] < 200){ui->immuneLeftBar1->setValue(100); ui->immuneLeftBar2->setValue(int(data["h2Left"]-100));}else if(data["h2Left"] > 200 ){ui->immuneLeftBar1->setValue(100); ui->immuneLeftBar2->setValue(100);} else{ui->immuneLeftBar1->setValue(int(data["h2Left"])); ui->immuneLeftBar2->setValue(0);}
    if(data["h2Right"] > 100 && data["h2Right"] < 200){ui->immuneRightBar1->setValue(100); ui->immuneRightBar2->setValue(int(data["h2Right"]-100));}else if(data["h2Right"] >= 200 ){ui->immuneRightBar1->setValue(100); ui->immuneRightBar2->setValue(100);} else{ui->immuneRightBar1->setValue(int(data["h2Right"])); ui->immuneRightBar2->setValue(0);}
    if(data["h3Left"] > 100 && data["h3Left"] < 200){ui->heartLeftBar1->setValue(100); ui->heartLeftBar2->setValue(int(data["h3Left"]-100));}else if(data["h3Left"] >= 200 ){ui->heartLeftBar1->setValue(100); ui->heartLeftBar2->setValue(100);} else{ui->heartLeftBar1->setValue(int(data["h3Left"])); ui->heartLeftBar2->setValue(0);}
    if(data["h3Right"] > 100 && data["h3Right"] < 200){ui->heartRightBar1->setValue(100); ui->heartRightBar2->setValue(int(data["h3Right"]-100));}else if(data["h3Right"] >= 200 ){ui->heartRightBar1->setValue(100); ui->heartRightBar2->setValue(100);} else{ui->heartRightBar1->setValue(int(data["h3Right"])); ui->heartRightBar2->setValue(0);}
    if(data["h4Left"] > 100 && data["h4eft"] < 200){ui->smallIntestLeftBar1->setValue(100); ui->smallIntestLeftBar2->setValue(int(data["h4Left"]-100));} else if(data["h4Left"] >= 200 ){ui->smallIntestLeftBar1->setValue(100); ui->smallIntestLeftBar2->setValue(100);}else{ui->smallIntestLeftBar1->setValue(int(data["h4Left"])); ui->smallIntestLeftBar2->setValue(0);}
    if(data["h4Right"] > 100 && data["h4Right"] < 200){ui->smallIntestRightBar1->setValue(100); ui->smallIntestRightBar2->setValue(int(data["h4Right"]-100));}else if(data["h4Right"] >= 200 ){ui->smallIntestRightBar1->setValue(100); ui->smallIntestRightBar2->setValue(100);} else{ui->smallIntestRightBar1->setValue(int(data["h4Right"])); ui->smallIntestRightBar2->setValue(0);}
    if(data["h5Left"] > 100 && data["h5Left"] < 200){ui->vesselLeftBar1->setValue(100); ui->vesselLeftBar2->setValue(int(data["h5Left"]-100));} else if(data["h5Left"] >= 200 ){ui->vesselLeftBar1->setValue(100); ui->vesselLeftBar2->setValue(100);}else{ui->vesselLeftBar1->setValue(int(data["h5Left"])); ui->vesselLeftBar2->setValue(0);}
    if(data["h5Right"] > 100 && data["h5Right"] < 200){ui->vesselRightBar1->setValue(100); ui->vesselRightBar2->setValue(int(data["h5Right"]-100));} else if(data["h5Right"] >= 200 ){ui->vesselRightBar1->setValue(100); ui->vesselRightBar2->setValue(100);}else{ui->vesselRightBar1->setValue(int(data["h5Right"])); ui->vesselRightBar2->setValue(0);}
    if(data["h6Left"] > 100 && data["h6Left"] < 200){ui->largeIntestLeftBar1->setValue(100); ui->largeIntestLeftBar2->setValue(int(data["h6Left"]-100));} else if(data["h6Left"] >= 200 ){ui->largeIntestLeftBar1->setValue(100); ui->largeIntestLeftBar2->setValue(100);}else{ui->largeIntestLeftBar1->setValue(int(data["h6Left"])); ui->largeIntestLeftBar2->setValue(0);}
    if(data["h6Right"] > 100 && data["h6Right"] < 200){ui->largeIntestRightBar1->setValue(100); ui->largeIntestRightBar2->setValue(int(data["h6Right"]-100));} else if(data["h6Right"] >= 200 ){ui->largeIntestRightBar1->setValue(100); ui->largeIntestRightBar2->setValue(100);}else{ui->largeIntestRightBar1->setValue(int(data["h6Right"])); ui->largeIntestRightBar2->setValue(0);}
    if(data["f1Left"] > 100 && data["f1Left"] < 200){ui->pancLeftBar1->setValue(100); ui->pancLeftBar2->setValue(int(data["f1Left"]-100));}else if(data["f1Left"] >= 200 ){ui->pancLeftBar1->setValue(100); ui->pancLeftBar2->setValue(100);} else{ui->pancLeftBar1->setValue(int(data["f1Left"])); ui->pancLeftBar2->setValue(0);}
    if(data["f1Right"] > 100 && data["f1Right"]  < 200){ui->pancRightBar1->setValue(100); ui->pancRightBar2->setValue(int(data["f1Right"]-100));} else if(data["f1Right"] >= 200 ){ui->pancRightBar1->setValue(100); ui->pancRightBar2->setValue(100);}else{ui->pancRightBar1->setValue(int(data["f1Right"])); ui->pancRightBar2->setValue(0);}
    if(data["f2Left"] > 100 && data["f2Left"]  < 200){ui->liverLeftBar1->setValue(100); ui->liverLeftBar2->setValue(int(data["f2Left"]-100));} else if(data["f2Left"] >= 200 ){ui->liverLeftBar1->setValue(100); ui->liverLeftBar2->setValue(100);}else{ui->liverLeftBar1->setValue(int(data["f2Left"])); ui->liverLeftBar2->setValue(0);}
    if(data["f2Right"] > 100 && data["f2Right"]  < 200){ui->liverRightBar1->setValue(100); ui->LiverRightBar2->setValue(int(data["f2Right"]-100));} else if(data["f2Right"] >= 200 ){ui->liverRightBar1->setValue(100); ui->LiverRightBar2->setValue(100);}else{ui->liverRightBar1->setValue(int(data["f2Right"])); ui->LiverRightBar2->setValue(0);}
    if(data["f3Left"] > 100 && data["f3Left"]  < 200){ui->kidneyLeftBar1->setValue(100); ui->kidneyLeftBar2->setValue(int(data["f3Left"]-100));} else if(data["f3Left"] >= 200 ){ui->kidneyLeftBar1->setValue(100); ui->kidneyLeftBar2->setValue(100);}else{ui->kidneyLeftBar1->setValue(int(data["f3Left"])); ui->kidneyLeftBar2->setValue(0);}
    if(data["f3Right"] > 100 && data["f3Right"]  < 200){ui->kidneyRightBar1->setValue(100); ui->kidneyRightBar2->setValue(int(data["f3Right"]-100));} else if(data["f3Right"] >= 200 ){ui->kidneyRightBar1->setValue(100); ui->kidneyRightBar2->setValue(100);}else{ui->kidneyRightBar1->setValue(int(data["f3Right"])); ui->kidneyRightBar2->setValue(0);}
    if(data["f4Left"] > 100 && data["f4Left"]  < 200){ui->bladderLeftBar1->setValue(100); ui->bladderLeftBar2->setValue(int(data["f4Left"]-100));} else if(data["f4Left"] >= 200 ){ui->bladderLeftBar1->setValue(100); ui->bladderLeftBar2->setValue(100);}else{ui->bladderLeftBar1->setValue(int(data["f4Left"])); ui->bladderLeftBar2->setValue(0);}
    if(data["f4Right"] > 100 && data["f4Right"]  < 200){ui->bladderRightBar1->setValue(100); ui->bladderRightBar2->setValue(int(data["f4Right"]-100));} else if(data["f4Right"] >= 200 ){ui->bladderRightBar1->setValue(100); ui->bladderRightBar2->setValue(100);}else{ui->bladderRightBar1->setValue(int(data["f4Right"])); ui->bladderRightBar2->setValue(0);}
    if(data["f5Left"] > 100 && data["f5Left"]  < 200){ui->gallbladderLeftBar1->setValue(100); ui->gallbladderLeftBar2->setValue(int(data["f5Left"]-100));} else if(data["f5Left"] >= 200 ){ui->gallbladderLeftBar1->setValue(100); ui->gallbladderLeftBar2->setValue(100);}else{ui->gallbladderLeftBar1->setValue(int(data["f5Left"])); ui->gallbladderLeftBar2->setValue(0);}
    if(data["f5Right"] > 100 && data["f5Right"]  < 200){ui->gallbladderRightBar1->setValue(100); ui->gallbladderRightBar2->setValue(int(data["f5Right"]-100));}else if(data["f5Right"] >= 200 ){ui->gallbladderRightBar1->setValue(100); ui->gallbladderRightBar2->setValue(100);} else{ui->gallbladderRightBar1->setValue(int(data["f5Right"])); ui->gallbladderRightBar2->setValue(0);}
    if(data["f6Left"] > 100 && data["f6Left"]  < 200){ui->stomachLeftBar1->setValue(100); ui->stomachLeftBar2->setValue(int(data["f6Left"]-100));} else if(data["f6Left"] >= 200 ){ui->stomachLeftBar1->setValue(100); ui->stomachLeftBar2->setValue(100);}else{ui->stomachLeftBar1->setValue(int(data["f6Left"])); ui->stomachLeftBar2->setValue(0);}
    if(data["f6Right"] > 100 && data["f6Right"] < 200){ui->stomachRightBar1->setValue(100); ui->stomachRightBar2->setValue(int(data["f6Right"]-100));}else if(data["f6Right"] >= 200 ){ui->stomachRightBar1->setValue(100); ui->stomachRightBar2->setValue(100);} else{ui->stomachRightBar1->setValue(int(data["f6Right"])); ui->stomachRightBar2->setValue(0);}
}

// Set status
void MainWindow::adjustStatus(std::map<std::string, float> data, std::map<std::string, float>data2){
    ui->energyInVal->setText(QString::number(data["energyLevel"], 'f', 2));
    ui->immuneInVal->setText(QString::number(data["immuneSystem"], 'f', 2));
    ui->metabInVal->setText(QString::number(data["metabolism"], 'f', 2));
    ui->psychoInVal->setText(QString::number(data["psychoEmotional"], 'f', 2));
    ui->muscuInVal->setText(QString::number(data["musculoskeletal"], 'f', 2));
    ui->averageInVal->setText(QString::number(data["energyLevel"], 'f', 2));
    ui->leftInVal->setText(QString::number(data["leftMeridian"], 'f', 2));
    ui->rightInVal->setText(QString::number(data["rightMeridian"], 'f', 2));
    ui->upperInVal->setText(QString::number(data["upperMeridian"], 'f', 2));
    ui->lowerInVal->setText(QString::number(data["lowerMeridian"], 'f', 2));

    //0 - blue, 1 - red, 2 - green
    switch(int(data2["energyLevel"])){
        case 0: ui->energyInFrame->setStyleSheet("background: rgb(0,150,255)");
                ui->energyInNorm->setText("Below Norm"); break;
        case 1: ui->energyInFrame->setStyleSheet("background: rgb(255,0,0)");
                ui->energyInNorm->setText("Above Norm"); break;
        default:  ui->energyInFrame->setStyleSheet("background: rgb(0,255,0)");
                  ui->energyInNorm->setText("Normal"); break;
    }
    switch(int(data2["immuneSystem"])){
        case 0: ui->immuneInFrame->setStyleSheet("background: rgb(0,150,255)");
                ui->immuneInNorm->setText("Below Norm"); break;
        case 1: ui->immuneInFrame->setStyleSheet("background: rgb(255,0,0)");
                ui->immuneInNorm->setText("Above Norm"); break;
        default:  ui->immuneInFrame->setStyleSheet("background: rgb(0,255,0)");
                  ui->immuneInNorm->setText("Normal"); break;
    }
    switch(int(data2["metabolism"])){
        case 0: ui->metabInFrame->setStyleSheet("background: rgb(0,150,255)");
                ui->metabInNorm->setText("Below Norm"); break;
        case 1: ui->metabInFrame->setStyleSheet("background: rgb(255,0,0)");
                ui->metabInNorm->setText("Above Norm"); break;
        default:  ui->metabInFrame->setStyleSheet("background: rgb(0,255,0)");
                  ui->metabInNorm->setText("Normal"); break;
    }
    switch(int(data2["psychoEmotional"])){
        case 0: ui->psychoInFrame->setStyleSheet("background: rgb(0,150,255)");
                ui->psychoInNorm->setText("Below Norm"); break;
        case 1: ui->psychoInFrame->setStyleSheet("background: rgb(255,0,0)");
                ui->psychoInNorm->setText("Above Norm"); break;
        default:  ui->psychoInFrame->setStyleSheet("background: rgb(0,255,0)");
                  ui->psychoInNorm->setText("Normal"); break;
    }
    switch(int(data2["musculoskeletal"])){
        case 0: ui->muscuInFrame->setStyleSheet("background: rgb(0,150,255)");
                ui->muscuInNorm->setText("Below Norm"); break;
        case 1: ui->muscuInFrame->setStyleSheet("background: rgb(255,0,0)");
                ui->muscuInNorm->setText("Above Norm"); break;
        default:  ui->muscuInFrame->setStyleSheet("background: rgb(0,255,0)");
                  ui->muscuInNorm->setText("Normal"); break;
    }
}
