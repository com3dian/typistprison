#ifndef SAVEMESSAGEBOX_H
#define SAVEMESSAGEBOX_H

#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

class SaveMessageBox : public QMessageBox {
    Q_OBJECT // Necessary macro for signals and slots in Qt

public:
    explicit SaveMessageBox(QWidget *parent = nullptr);

private slots:
    void onSaveClicked();
    void onDiscardClicked();
};

#endif // CUSTOMMESSAGEBOX_H