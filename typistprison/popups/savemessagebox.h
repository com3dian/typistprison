#ifndef SAVEMESSAGEBOX_H
#define SAVEMESSAGEBOX_H

#include <QDialog>
#include <QPushButton>
#include <QWidget>

class SaveMessageBox : public QDialog {
    Q_OBJECT // Necessary macro for signals and slots in Qt

public:
    enum ButtonResult {
        Save,
        Discard,
        Cancel
    };

    explicit SaveMessageBox(QWidget *parent = nullptr);
    int exec() override;
    ButtonResult getResult() const;

private slots:
    void onSaveClicked();
    void onDiscardClicked();

private:
    void centerOnScreen();
    
    QPushButton *saveButton;
    QPushButton *discardButton;
    QPushButton *cancelButton;
    ButtonResult result = Cancel;
};

#endif // SAVEMESSAGEBOX_H