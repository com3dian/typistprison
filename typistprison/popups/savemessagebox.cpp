#include "savemessagebox.h"
#include <QApplication>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

SaveMessageBox::SaveMessageBox(QWidget *parent)
    : QDialog(parent)
{
    // Set the dialog parameters
    setWindowTitle("Unsaved Changes");
    setModal(true);
    setFixedWidth(400);

    // Apply a custom stylesheet to change the background color
    QString styleSheet = "QDialog {"
                         "    background-color: #1F2020;"  // dark gray background
                         "}";
    this->setStyleSheet(styleSheet);

    // Create the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Create and add the message label
    QLabel *messageLabel = new QLabel("The document has been modified. Do you want to save your changes?");
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignLeft);
    messageLabel->setStyleSheet("QLabel { color: #FFFFFF; background-color: transparent; }");
    mainLayout->addWidget(messageLabel);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    // Create custom buttons
    saveButton = new QPushButton("  Save  ");
    discardButton = new QPushButton("Discard");
    cancelButton = new QPushButton("Cancel");

    // // set save width for buttons
    // saveButton->setFixedWidth(64);
    // discardButton->setFixedWidth(64);
    // cancelButton->setFixedWidth(64);

    // Style the buttons using style sheets
    QString saveButtonStyle = R"(
                                QPushButton {
                                    background-color: transparent;
                                    border: 1px solid #5A5A5A;
                                    color: #BDBDBD;
                                    border-radius: 4px;
                                    padding: 4px 8px;
                                }
                                QPushButton:hover {
                                    background-color: transparent;
                                    border: 1px solid #80bd96;
                                    color: #84e0a5;
                                    border-radius: 4px;
                                    padding: 4px 8px;
                                })";

    QString discardButtonStyle = R"(
                                 QPushButton {
                                    background-color: transparent;
                                    border: 1px solid #5A5A5A;
                                    color: #BDBDBD;
                                    border-radius: 4px;
                                    padding: 4px 8px;
                                }
                                QPushButton:hover {
                                    background-color: transparent;
                                    border: 1px solid #ba6757;
                                    color: #E0715C;
                                    border-radius: 4px;
                                    padding: 4px 8px;
                                })";

    QString cancelButtonStyle = R"(
                                QPushButton {
                                    background-color: transparent;
                                    border: 1px solid #5A5A5A;
                                    color: #BDBDBD;
                                    border-radius: 4px;
                                    padding: 4px 8px;
                                }
                                QPushButton:hover {
                                    background-color: transparent;
                                    border: 1px solid #3A3A3A;
                                    color: #5A5A5A;
                                    border-radius: 4px;
                                    padding: 4px 8px;
                                })";

    saveButton->setStyleSheet(saveButtonStyle);
    discardButton->setStyleSheet(discardButtonStyle);
    cancelButton->setStyleSheet(cancelButtonStyle);

    // Add buttons to layout with explicit positioning
    buttonLayout->addWidget(discardButton);
    buttonLayout->addStretch(); // Push buttons
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    // Add button layout to main layout
    mainLayout->addLayout(buttonLayout);

    // Connect the custom buttons to the appropriate slots
    connect(saveButton, &QPushButton::clicked, this, &SaveMessageBox::onSaveClicked);
    connect(discardButton, &QPushButton::clicked, this, &SaveMessageBox::onDiscardClicked);
    connect(cancelButton, &QPushButton::clicked, this, &SaveMessageBox::reject);

    // Center the dialog on screen
    centerOnScreen();
}

void SaveMessageBox::onSaveClicked() {
    // Custom save logic here
    result = Save;
    accept();
}

void SaveMessageBox::onDiscardClicked() {
    result = Discard;
    accept();
}

void SaveMessageBox::centerOnScreen() {
    // Get the screen geometry to calculate the center position
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int x = (screenGeometry.width() - this->width()) / 2;   // center horizontally
    int y = (screenGeometry.height() - this->height()) / 2; // center vertically

    // Move the dialog to the calculated center position
    this->move(x, y);
}

SaveMessageBox::ButtonResult SaveMessageBox::getResult() const {
    return result;
}

int SaveMessageBox::exec() {
    // Center the dialog before showing
    centerOnScreen();
    return QDialog::exec();
}