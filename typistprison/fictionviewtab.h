#ifndef FICTIONVIEWTAB_H
#define FICTIONVIEWTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollBar>
#include <QString>
#include <QFile>
#include <QPushButton>
#include <QLineEdit>
#include <QMargins>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>

#include "fictiontextedit.h"
#include "searchWidget.h"
#include "prisonerfictiontextedit.h"
#include "projectmanager.h"
#include "basetextedittab.h"
#include "utils/hoverbutton.h"
#include "popups/prisonerdialog.h"


class FictionViewTab : public BaseTextEditTab {
    Q_OBJECT

public:
    explicit FictionViewTab(const QString &content, const QString &filePath, QWidget *parent = nullptr,
                          bool isPrisoner = false, ProjectManager *projectManager = nullptr);
    QString getTextContent() const override;
    HoverButton *prisonerButton;

private:
    FictionTextEdit *textEdit;
    QScrollBar *vScrollBar;
    HoverButton *sniperButton;
    QHBoxLayout *globalLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QHBoxLayout *bottomLeftLayout;
    QLabel *wordCountLabel;
    // Add to private member variables
    QWidget *wordCountSpacerRight;  // Change from QSpacerItem* to QWidget*
    QString oldTextContent;
    bool isPrisoner;
    QFrame* prisonerFrame = nullptr;  // instead of QDialog* prisonerDialog
    FictionViewTab *prisonerFictionViewTab;
    PrisonerFictionTextEdit* fullScreenPrisonerTextEdit;
    ProjectManager *projectManager;

    void setupTextEdit(const QString &content);
    void setupScrollBar();
    void syncScrollBar();
    void activateSniperMode();
    void deactivateSniperMode();
    void editContent();
    bool saveContent() override;
    void updateWordcount();
    void activatePrisonerMode();
    void deactivatePrisonerMode();
    void showWikiFunc(const QString &wikiContent, QPoint lastMousePos);
    void hideWikiFunc();

signals:
    void showWikiAt(const QString &wikiContent, QPoint lastMousePos);
    void hideWiki();

// In the protected or public section of your class declaration:
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // FICTIONVIEWTAB_H
