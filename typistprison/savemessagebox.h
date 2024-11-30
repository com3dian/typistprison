#ifndef SAVEMESSAGEBOX_H
#define SAVEMESSAGEBOX_H

#include <QMessageBox>
#include <QPushButton>
#include <QWidget>
#include <QShowEvent>

class SaveMessageBox : public QMessageBox {
    Q_OBJECT // Necessary macro for signals and slots in Qt

public:
    explicit SaveMessageBox(QWidget *parent = nullptr);
    // void showEvent(QShowEvent *event) override;
    int exec() override;

private slots:
    void onSaveClicked();
    void onDiscardClicked();
};

#endif // CUSTOMMESSAGEBOX_H