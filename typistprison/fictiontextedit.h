#ifndef FICTIONTEXTEDIT_H
#define FICTIONTEXTEDIT_H


#include "searchWidget.h"
#include "fontmanager.h"
#include "fictionhighlighter.h"

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
    FictionTextEdit(QWidget *parent = nullptr);

    void load(const QString& text);
    void setTopMargin(int margin);
    void activateSniperMode();
    void deactivateSniperMode();
    void search(const QString &searchString);
    void searchPrev(const QString &searchString);
    void clearSearch();

signals:
    void onFictionEditSearch(const QString &text);
    void focusGained();
    void onSave();
    void keyboardInput();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void insertFromMimeData(const QMimeData *source) override;
    void focusInEvent(QFocusEvent *e) override;
    // void paintEvent(QPaintEvent *event) override;
    // void focusOutEvent(QFocusEvent *event) override;

private:
    void applyBlockFormatting(QTextBlock &block);
    QTextCursor applyCharFormatting(QTextCursor &cursor, bool insertLargeFont = true);
    QTextCursor applyCharFormatting4NextBlock(QTextCursor &cursor);
    void updateFocusBlock();
    void changeGlobalTextColor(const QColor &color);
    int getVisibleCenterY();
    int checkVisibleCenterBlock(const QTextBlock &block);
    QTextBlock findBlockClosestToCenter();
    // void toggleCursorVisibility();

    void changeFontSize(int delta);
    int globalFontSize;
    QTextBlock previousCenteredBlock;
    QTextBlock newCenteredBlock;
    bool isSniperMode;
    FictionHighlighter* highlighter;
    int matchStringIndex;
    bool isPrisoner;

};

#endif // FICTIONTEXTEDIT_H
