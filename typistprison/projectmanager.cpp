#include "projectmanager.h"
#include <QDebug>

// Constructor
ProjectManager::ProjectManager()
    : isLoadedProject(false)
    , haveBannedWordsFile(false)
    , maxiumBannedWordLength(0)

{
    // Initialization code, if needed
}

void ProjectManager::open(const QString selectedProjectRoot) {
    readBannedWords(selectedProjectRoot);

    isLoadedProject = true;

    return;
}

void ProjectManager::readBannedWords(const QString selectedProjectRoot) {
    maxiumBannedWordLength = 0;

    // Step 1: Locate all .txt files
    QDir directory(selectedProjectRoot);
    QStringList txtFiles = directory.entryList(QStringList() << "*.txt", QDir::Files);

    qDebug() << "1. selectedProjectRoot" << selectedProjectRoot;

    // Step 2: Identify the file with the longest sequence of '*'
    QString bestFile;
    int maxStars = 0;

    for (const QString& fileName : txtFiles) {
        int starCount = fileName.count('*');
        if (starCount > maxStars) {
            maxStars = starCount;
            bestFile = fileName;
        }
    }

    qDebug() << "2. best file banned" << bestFile;

    // Step 3: Read the contents of the selected file
    if (!bestFile.isEmpty()) {
        QFile file(directory.filePath(bestFile));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open file:" << bestFile;
            return;
        }

        QTextStream in(&file);
        std::vector<std::string> lines;
        while (!in.atEnd()) {
            QString line = in.readLine();
            lines.push_back(line.toStdString());
        }
        file.close();

        bannedWordsLines = lines;

        QString bannedWordsQstringList;
        for (const auto& str : bannedWordsLines) {
            bannedWordsQstringList.append(QString::fromUtf8(str.c_str()));
            QString banneWordQSR = QString::fromUtf8(str.c_str());
            maxiumBannedWordLength = std::max(static_cast<int>(banneWordQSR.length()), maxiumBannedWordLength);
        }

        qDebug() << "3. bannedWordsLines" << bannedWordsQstringList;
        qDebug() << "4. maxiumBannedWordLength" << maxiumBannedWordLength;

        AhoCorasick trie;
        for (size_t i = 0; i < lines.size(); ++i) {
            trie.insert(lines[i], i);
        }
        trie.buildFailureLinks();

        bannedWordsTrie = trie;
        haveBannedWordsFile = true;
    } else {
        haveBannedWordsFile = false;
    }
}

int ProjectManager::getMaxiumBannedWordLength() {
    return maxiumBannedWordLength;
}

QString ProjectManager::matchBannedWords(QString text) {
    // If no banned words file is found, return original text
    if (!haveBannedWordsFile) {
        return text;
    }

    std::string stdStringText = text.toStdString();
    std::vector<std::pair<int, int>> matches = bannedWordsTrie.search(stdStringText);

    int backspace = 0;

    for (const auto& match : matches) {
        int patternIndex = match.first;
        int utf8BannedWordLength = bannedWordsLines[patternIndex].size();
        int end = match.second - backspace;
        int start = end - utf8BannedWordLength + 1;
        qDebug() << "start position" << start;

        // Replace the range [start, end] with proper number of asterisks
        stdStringText.erase(start, utf8BannedWordLength);

        // get the length of banned word
        int actualBannedWordLength = QString::fromUtf8(bannedWordsLines[patternIndex].c_str()).length();
        std::string replacementAsterisks(actualBannedWordLength, '*');
        stdStringText.insert(start, replacementAsterisks);
        
        // update backspace
        backspace += utf8BannedWordLength - actualBannedWordLength;
    }
    QString filteredText = QString::fromStdString(stdStringText); 

    return filteredText;
}


void ProjectManager::readWikiFiles(const QString& selectedProjectRoot) {
    QDir projectDir(selectedProjectRoot);
    QDir wikiDir(projectDir.filePath("wiki"));

    QStringList mdFiles;

    // Check if the "wiki" directory exists
    if (wikiDir.exists()) {
        mdFiles = wikiDir.entryList(QStringList() << "*.md", QDir::Files);
    }

    // If no .md files in "wiki", check for wiki.md in root
    if (mdFiles.isEmpty()) {
        QFile wikiFile(projectDir.filePath("wiki.md"));
        if (wikiFile.exists()) {
            mdFiles.append("wiki.md"); // Add wiki.md manually
        }
    }

    // If still no .md files, do nothing
    if (mdFiles.isEmpty()) {
        qDebug() << "No wiki files found.";
        haveWiki = false;
        return;
    }

    qDebug() << "Found wiki files:" << mdFiles;

    // Read and store content (if needed)
    for (const QString& mdFile : mdFiles) {
        QFile file(wikiDir.exists() ? wikiDir.filePath(mdFile) : projectDir.filePath(mdFile));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            file.close();
            qDebug() << "Contents of" << mdFile << ":" << content.left(100) << "..."; // Print only first 100 chars
        } else {
            qWarning() << "Could not open file:" << mdFile;
        }
    }
}

/* 
TODO:
When the banned words are updated, also reload the banned words

*/
// void ProjectManager::refreshBannedWords() {

// }
// for wiki
// void ProjectManager::refreshBannedWords() {

// }