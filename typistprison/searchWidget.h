#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);

signals:
    void onSearch(const QString &text);
    void onClear();

public slots:
    void handleSearch(const QString &text);
    void handleClear();
    void handleReSearch();
    void updateBottomLine();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLineEdit *lineEdit;
    QPushButton *button;
    QWidget *bottomLine;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    bool isHovered;
    bool isOnSearch;
};

#endif // SEARCHWIDGET_H
