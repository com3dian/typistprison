#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "foldertreeviewwidget.h"
#include "utils/ahocorasick.h"
#include <QDir>
#include <QFile>
#include <QObject>
#include <QTimer>

class ProjectManager : public QObject {
    Q_OBJECT  // Required macro for QObject subclasses

public:
    // Constructor
    explicit ProjectManager(QObject* parent = nullptr);

    // Member Functions
    void open(const QString selectedProjectRoot);
    QString matchBannedWords(QString text);
    int getMaxiumBannedWordLength();

    // Variables
    bool haveBannedWordsFile;
    bool haveWiki;
    bool isLoadedProject;
    
private:
    // Member Variables
    std::vector<std::string> bannedWordsLines;
    AhoCorasick bannedWordsTrie;
    int maxiumBannedWordLength;
    QTimer* bannedWordsTimer;
    QString currentProjectRoot;

    void readBannedWords(const QString selectedProjectRoot);
    void readWikiFiles(const QString& selectedProjectRoot);
    
private slots:
    void checkBannedWordsChanges();
};

#endif // PROJECTMANAGER_H
