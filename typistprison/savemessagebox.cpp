#include "savemessagebox.h"
#include <QApplication>
#include <QDebug>
#include <QApplication>
#include <QScreen>

SaveMessageBox::SaveMessageBox(QWidget *parent)
    : QMessageBox(parent)
{
    // Set the standard message box parameters
    setWindowTitle("Unsaved Changes");
    setText("The document has been modified. Do you want to save your changes?");

    // Remove the default buttons
    setStandardButtons(QMessageBox::NoButton);

    // Apply a custom stylesheet to change the background color
    QString styleSheet = "QMessageBox {"
                         "    background-color: #262a33;"  // Light gray background
                         "}";

    this->setStyleSheet(styleSheet);

    // Create custom buttons
    QPushButton *saveButton = new QPushButton("Save");
    QPushButton *discardButton = new QPushButton("Discard");
    QPushButton *cancelButton = new QPushButton("Cancel");

    // Style the buttons using style sheets
    QString saveButtonStyle = "QPushButton {"
                              "   padding: 8px 16px;"
                              "   font-size: 14px;"
                              "   border: 0px;"
                              "   border-radius: 0px;"
                              "   background-color: transparent;"
                              "}"
                              "QPushButton:hover {"
                              "   background-color: #84e0a5;"
                              "   color: #1b1e24;"
                              "}"
                              "QPushButton:pressed {"
                              "   background-color: #84e0a5;"
                              "   color: transparent;"
                              "}";

    QString discardButtonStyle = "QPushButton {"
                                 "   padding: 10px 20px;"
                                 "   font-size: 14px;"
                                 "   border: 0px;"
                                 "   border-radius: 0px;"
                                 "   background-color: transparent;"
                                 "}"
                                 "QPushButton:hover {"
                                 "   background-color: #fc7258;"
                                 "   color: #1b1e24;"
                                 "}"
                                 "QPushButton:pressed {"
                                 "   background-color: #fc7258;"
                                 "   color: transparent;"
                                 "}";

    QString cancelButtonStyle = "QPushButton {"
                                "   padding: 10px 20px;"
                                "   font-size: 14px;"
                                "   border: 0px;"
                                "   border-radius: 0px;"
                                "   background-color: transparent;"
                                "}"
                                "QPushButton:hover {"
                                "   background-color: #454F61;"
                                "   color: #1b1e24;"
                                "}"
                                "QPushButton:pressed {"
                                "   background-color: #454F61;"
                                "   color: transparent;"
                                "}";

    saveButton->setStyleSheet(saveButtonStyle);
    discardButton->setStyleSheet(discardButtonStyle);
    cancelButton->setStyleSheet(cancelButtonStyle);

    // Add custom buttons to the message box
    addButton(saveButton, QMessageBox::AcceptRole);
    addButton(discardButton, QMessageBox::DestructiveRole);
    addButton(cancelButton, QMessageBox::RejectRole);

    // Connect the custom buttons to the appropriate slots
    connect(saveButton, &QPushButton::clicked, this, &SaveMessageBox::onSaveClicked);
    connect(discardButton, &QPushButton::clicked, this, &SaveMessageBox::onDiscardClicked);
    connect(cancelButton, &QPushButton::clicked, this, &SaveMessageBox::reject);
}

// void SaveMessageBox::showEvent(QShowEvent *event) {
//     // Call base class method
//     QMessageBox::showEvent(event);

//     // Set custom position for the message box
//     // For example, center it on the screen or place it at a specific point
//     // Here, centering it on the primary screen
//     QWidget *parentWnd = parentWidget();  // Use the parent widget for relative positioning if needed

//     // Use QScreen to get the dimensions of the current screen
//     QScreen *screen = QApplication::primaryScreen();
//     QRect screenGeometry = screen->geometry();

//     int x = (screenGeometry.width() - this->width()) / 2;  // center horizontally
//     int y = (screenGeometry.height() - this->height()) / 2;  // center vertically

//     move(x, y);  // Move the message box to the new position
// }

void SaveMessageBox::onSaveClicked() {
    // Custom save logic here
    setResult(QMessageBox::Save);  // Set the result to QMessageBox::Save
    done(QMessageBox::Save);
}

void SaveMessageBox::onDiscardClicked() {
    setResult(QMessageBox::Discard);  // Set the result to QMessageBox::Save
    done(QMessageBox::Discard);
}

int SaveMessageBox::exec() {
    // Show the message box first
    int result = QMessageBox::exec();

    // Get the screen geometry to calculate the center position
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int x = (screenGeometry.width() - this->width()) / 2;   // center horizontally
    int y = (screenGeometry.height() - this->height()) / 2; // center vertically

    // Move the message box to the calculated center position
    this->move(x, y);

    return result;
}