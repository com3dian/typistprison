#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSysInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    // Warning: ‘Qt::AA_EnableHighDpiScaling’ is deprecated: High-DPI scaling is always enabled. 
    // This attribute no longer has any effect.

    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "typistprison_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    #if defined(Q_OS_WIN)
        qDebug() << "Running on Windows";
        QString styleSheet = "QWidget { font-family: 'Noto Sans CJK SC Light';}";
        
    #elif defined(Q_OS_MAC)
        qDebug() << "Running on macOS";
    #elif defined(Q_OS_LINUX)
        QString styleSheet = "QWidget { font-family: 'Noto Sans CJK SC Light';}"
                             "PlaintextEdit { font-family: 'Noto Mono'; }"
                             "FictionTextEdit { font-family: 'Noto Sans CJK SC'; }" ;
        
    #else
        qDebug() << "Unknown Operating System";
    #endif

    // Set the font as the default application font
    app.setStyleSheet(styleSheet);

    w.show();
    return app.exec();
}
