#include "customtabbarWidget.h"

CustomTabBarWidget::CustomTabBarWidget(QWidget *parent, CustomTabWidget *syncedTabWidget)
    : QWidget(parent), syncedTabWidget(syncedTabWidget), isExpanded(false) {
    // Main container layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Setup the toggle button
    setupToggleButton();

    // Setup the menu bar
    setupMenuBar();

    // Setup the tab bar
    setupTabBar();

    // Add toggle button, menu bar, and tab bar to the main layout
    mainLayout->addWidget(toggleButton); // Toggle button on the left
    mainLayout->addWidget(menuBar);     // Menu bar in the middle
    mainLayout->addWidget(tabBar);      // Tab bar on the right
}

CustomTabBarWidget::~CustomTabBarWidget() {
    // Cleanup if necessary
}

void CustomTabBarWidget::setupToggleButton() {
    // Create the toggle button
    toggleButton = new QPushButton("☰", this);
    toggleButton->setFixedSize(40, 40);
    toggleButton->setStyleSheet("background-color: #888; color: white; border: none;");

    // Connect the toggle button click to toggleMenuBar()
    connect(toggleButton, &QPushButton::clicked, this, &CustomTabBarWidget::toggleMenuBar);
}

void CustomTabBarWidget::onTabGainedAttention(int index) {
    if (syncedTabWidget) {
        syncedTabWidget->blockSignals(true);

        syncedTabWidget->setCurrentIndex(index);

        syncedTabWidget->blockSignals(false);
    }
}

void CustomTabBarWidget::onTabInserted(int index, const QString &label) {
    if (syncedTabWidget) {
        syncedTabWidget->blockSignals(true);

        QPushButton *closeButton = new QPushButton(this);
        closeButton->setFixedSize(16, 16); // Small size for a close button
        closeButton->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "padding: 5px;"
            "border-image: url(:/icons/tab_close.png) 0 0 0 0 stretch stretch;"
            "}"
        );

        tabBar->insertTab(index, label); // insert tab
        tabBar->setTabButton(index, QTabBar::RightSide, closeButton);

        // Connect the close button to remove the tab
        connect(closeButton, &QPushButton::clicked, this, [this, index]() {
            onTabRemoved(index);
        });

        syncedTabWidget->blockSignals(false);
    }
}

void CustomTabBarWidget::onTabClosedFromSyncedWidget(int index) {
    if (syncedTabWidget) {
        syncedTabWidget->blockSignals(true);

        tabBar->setTabButton(index, QTabBar::RightSide, nullptr);  // Remove the close button
        tabBar->removeTab(index);                                  // Remove the tab

        syncedTabWidget->blockSignals(false);
    }
}

void CustomTabBarWidget::onTabRemoved(int index) {
    if (syncedTabWidget) {

        syncedTabWidget->closeWindowIfNoTabs(index);
    }
}

void CustomTabBarWidget::onTabMoved(int from, int to) {
    if (syncedTabWidget) {
        syncedTabWidget->blockSignals(true);

        // Move the corresponding tab in QTabWidget
        QWidget *widget = syncedTabWidget->widget(from);
        QString label = syncedTabWidget->tabText(from);
        syncedTabWidget->removeTab(from);
        syncedTabWidget->insertTab(to, widget, label);

        syncedTabWidget->blockSignals(false);
    }
}

void CustomTabBarWidget::onTabTitleUpdated(int index, QString newTitle) {
    tabBar->setTabText(index, newTitle);
}

void CustomTabBarWidget::setupTabBar() {
    // Create the tab bar
    tabBar = new QTabBar(this);
    tabBar->setStyleSheet(
        "QTabBar::tab {"
        "    padding-right: 30px; /* Add space for the close button */"
        "    padding-left: 10px;  /* Adjust left padding if needed */"
        "}"
        "QTabBar::tab:selected {"
        "    background-color: #154543;"
        "}"
    );

    // Optional: Customize tab behavior or appearance
    tabBar->setExpanding(true);  // Tabs expand to fill available space
    tabBar->setMovable(true);

    connect(syncedTabWidget, &CustomTabWidget::tabInsertedSignal, this, &CustomTabBarWidget::onTabInserted);
    connect(syncedTabWidget, &CustomTabWidget::tabClosedFromSyncedTabWidgetSignal, this, &CustomTabBarWidget::onTabClosedFromSyncedWidget);
    connect(syncedTabWidget, &CustomTabWidget::updatedTabTitleSignal, this, &CustomTabBarWidget::onTabTitleUpdated);

    connect(tabBar, &QTabBar::tabBarClicked, this, &CustomTabBarWidget::onTabGainedAttention);
    connect(tabBar, &QTabBar::tabCloseRequested, this, &CustomTabBarWidget::onTabRemoved);
    connect(tabBar, &QTabBar::tabMoved, this, &CustomTabBarWidget::onTabMoved);

    return;
}

void CustomTabBarWidget::setupMenuBar() {
    menuBar = new QFrame(this);
    menuBar->setStyleSheet("background-color: #729485; border: 0px solid #cccccc;");
    menuBar->setFixedHeight(40);  // 高度固定为 40
    menuBar->setMaximumWidth(0);  // 初始宽度
    menuBar->setMinimumWidth(0);  // 折叠时的最小宽度

    QHBoxLayout *menuLayout = new QHBoxLayout(menuBar);
    menuLayout->setContentsMargins(5, 0, 0, 0);
    menuLayout->setSpacing(10);

    // 添加一些 PushButton
    button1 = new QPushButton("Button 1", this);
    button1->setVisible(false); // 默认折叠时隐藏

    button2 = new QPushButton("Button 1", this);
    button2->setVisible(false); // 默认折叠时隐藏

    menuLayout->addWidget(button1);
    menuLayout->addWidget(button2);

    menuBar->setLayout(menuLayout);
}

void CustomTabBarWidget::showMenu() {
}

void CustomTabBarWidget::toggleMenuBar() {
    // Animate the expansion of the menu bar
    QPropertyAnimation *animation = new QPropertyAnimation(menuBar, "minimumWidth", this);
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    if (isExpanded) {
        // Collapse menu bar
        animation->setStartValue(500);  // Expanded width
        animation->setEndValue(0);      // Collapsed width
        button1->setVisible(false);     // Hide button when collapsed
        button2->setVisible(false);     // Hide button when collapsed

    } else {
        // Expand menu bar
        animation->setStartValue(0);    // Collapsed width
        animation->setEndValue(500);    // Expanded width
        button1->setVisible(true);      // Show button when expanded
        button2->setVisible(true);      // Show button when expanded

    }

    animation->start();
    isExpanded = !isExpanded;  // Toggle the expanded state
}
