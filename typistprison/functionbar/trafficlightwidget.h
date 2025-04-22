#ifndef TRAFFICLIGHTWIDGET_H
#define TRAFFICLIGHTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>
#include <QSize>

class TrafficLightWidget : public QWidget {
    Q_OBJECT

public:
    explicit TrafficLightWidget(QWidget *parent = nullptr);

private:
    QPushButton *minimalButton;
    QPushButton *maximalButton;
    QPushButton *closeButton;

private slots:
    void greyButtons();

// Add these methods to the class declaration in the header
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void minimalButtonClicked();
    void maximalButtonClicked();
    void closeButtonClicked();
};

#endif // TRAFFICLIGHTWIDGET_H