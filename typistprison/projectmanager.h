#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "foldertreeviewwidget.h"
#include "utils/ahocorasick.h"
#include <QDir>
#include <QFile>

class ProjectManager {
public:
    // Constructor
    ProjectManager();

    // Member Functions
    void open(const QString selectedProjectRoot);
    QString matchBannedWords(QString text);
    int getMaxiumBannedWordLength();

    // Variables
    bool haveBannedWordsFile;
    bool isLoadedProject;
    
private:
    // Member Variables
    
    std::vector<std::string> bannedWordsLines;
    AhoCorasick bannedWordsTrie;
    int maxiumBannedWordLength;

    void readBannedWords(const QString selectedProjectRoot);
};

#endif // PROJECTMANAGER_H
