#ifndef FICTIONTEXTEDIT_H
#define FICTIONTEXTEDIT_H

#include <QTextEdit>
#include <QList>
#include <QTextBlock>
#include <string>
#include "searchWidget.h"
#include "fictionhighlighter.h"


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

private:
    void applyBlockFormatting(QTextBlock &block);
    QTextCursor applyCharFormatting(QTextCursor &cursor, bool insertLargeFont = true);
    void updateFocusBlock();
    void changeGlobalTextColor(const QColor &color);
    int getVisibleCenterY();
    std::string checkVisibleCenterBlock(const QTextBlock &block);
    QTextBlock findBlockClosestToCenter();

    void changeFontSize(int delta);
    int globalFontSize;
    QTextBlock previousCenteredBlock;
    QTextBlock newCenteredBlock;
    bool isSniperMode;
    FictionHighlighter* highlighter;
    int matchStringIndex;

};

#endif // FICTIONTEXTEDIT_H
