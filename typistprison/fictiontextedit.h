#ifndef FICTIONTEXTEDIT_H
#define FICTIONTEXTEDIT_H


#include "searchWidget.h"
#include "fontmanager.h"
#include "fictionhighlighter.h"
#include "projectmanager.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDebug>
#include <QFontMetricsF>
#include <QKeyEvent>
#include <QList>
#include <QMimeData>
#include <QPainter>
#include <QPalette>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextBlock>
#include <QTimer>
#include <QScrollBar>
#include <QString>
#include <QStringMatcher>


class FictionTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    FictionTextEdit(QWidget *parent = nullptr, ProjectManager *projectManager = nullptr);

    bool isSniperMode;

    void load(const QString& text, bool keepCursorPlace = false);
    void setTopMargin(int margin);
    virtual void activateSniperMode();
    virtual void deactivateSniperMode();
    void search(const QString &searchString);
    void searchPrev(const QString &searchString);
    void clearSearch();
    void changeGlobalTextColor(const QColor &color);
    void updateFocusBlock();
    void changeFontSize(int delta);
    void applyBlockFormatting(QTextBlock &block);
    QTextCursor applyCharFormatting(QTextCursor &cursor, bool insertLargeFont = true);
    QTextCursor applyCharFormatting4NextBlock(QTextCursor &cursor);

signals:
    void onFictionEditSearch(const QString &text);
    void focusGained();
    void onSave();
    void keyboardInput();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void insertFromMimeData(const QMimeData *source) override;
    void focusInEvent(QFocusEvent *e) override;

private:
    int getVisibleCenterY();
    int checkVisibleCenterBlock(const QTextBlock &block);
    QTextBlock findBlockClosestToCenter();
    void refresh();
    // void toggleCursorVisibility();

    int globalFontSize;
    QTextBlock previousCenteredBlock;
    QTextBlock newCenteredBlock;
    
    FictionHighlighter* highlighter;
    int matchStringIndex;
    ProjectManager *projectManager;
    QString previousText;
    int previousDocumentLength;
    int previousCursorPosition;
    QString previousDocumentText;
    

};

#endif // FICTIONTEXTEDIT_H
