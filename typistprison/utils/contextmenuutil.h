#ifndef CONTEXTMENUUTIL_H
#define CONTEXTMENUUTIL_H

#include <QPoint>
#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QMenu>
#include <QWidgetAction>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <functional>
#include "../functionbar/menubutton.h"


/*
Method for nice looking context menu (menu at right click).
Used in FictionViewTab, MarkdownViewTab, PlaintextViewTab.
*/
class ContextMenuUtil {
public:
    // Static utility function that works with both QTextEdit and QPlainTextEdit
    template<typename TextEditType>
    static void showContextMenu(TextEditType* textEdit, const QPoint& pos) {
        // Create the menu
        QMenu* menu = new QMenu(textEdit);
        menu->setAttribute(Qt::WA_TranslucentBackground);
        menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint);
        
        // Add drop shadow effect
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(menu);
        shadow->setBlurRadius(20);
        shadow->setColor(QColor(0, 0, 0, 80));
        shadow->setOffset(0, 0);
        menu->setGraphicsEffect(shadow);
        
        // Helper function to create MenuButton action
        auto createMenuButtonAction = [&](const QString &text, const QString &shortcut = QString(), bool enabled = true, std::function<void()> callback = nullptr) -> QWidgetAction* {
            MenuButton* button = new MenuButton(text, shortcut, menu);
            button->setEnabled(enabled);
            button->setFixedHeight(28);
            button->setFixedWidth(128);
            
            QWidgetAction* action = new QWidgetAction(menu);
            action->setDefaultWidget(button);
            
            // Mac-specific fix: Force initial repaint to ensure proper text rendering
            #ifdef Q_OS_MAC
            QTimer::singleShot(0, button, [button]() {
                button->update();
                button->repaint();
            });
            #endif
            
            if (callback) {
                QObject::connect(button, &MenuButton::mouseClick, [menu, callback]() {
                    menu->close();
                    callback();
                });
            }
            
            return action;
        };
        
        // Get document and cursor state (works for both QTextEdit and QPlainTextEdit)
        bool hasUndo = textEdit->document()->isUndoAvailable();
        bool hasRedo = textEdit->document()->isRedoAvailable();
        bool hasSelection = textEdit->textCursor().hasSelection();
        
        // Create menu items using MenuButton
        menu->addAction(createMenuButtonAction("Undo", "Ctrl+Z", hasUndo, [textEdit]() { textEdit->undo(); }));
        menu->addAction(createMenuButtonAction("Redo", "Ctrl+Y", hasRedo, [textEdit]() { textEdit->redo(); }));
        
        menu->addSeparator();
        
        menu->addAction(createMenuButtonAction("Cut", "Ctrl+X", hasSelection, [textEdit]() { textEdit->cut(); }));
        menu->addAction(createMenuButtonAction("Copy", "Ctrl+C", hasSelection, [textEdit]() { textEdit->copy(); }));
        menu->addAction(createMenuButtonAction("Paste", "Ctrl+V", true, [textEdit]() { textEdit->paste(); }));
        
        menu->addSeparator();
        
        menu->addAction(createMenuButtonAction("Select All", "Ctrl+A", true, [textEdit]() { textEdit->selectAll(); }));
        
        // Style the menu
        menu->setStyleSheet(
            "QMenu {"
            "    background-color: #1F2020;"
            "    border: 1px solid #1F2020;"
            "    border-radius: 5px;"
            "    padding-left: 12px;"
            "    padding-right: 12px;"
            "    padding-top: 4px;"
            "    padding-bottom: 4px;"
            "}"
            "QMenu::separator {"
            "    height: 1px;"
            "    background-color: #303030;"
            "    margin: 2px 0px 0px 0px;"
            "}"
        );
        
        // Show the menu at the cursor position
        menu->exec(textEdit->mapToGlobal(pos));
        
        // Clean up
        menu->deleteLater();
    }
};

#endif // CONTEXTMENUUTIL_H 