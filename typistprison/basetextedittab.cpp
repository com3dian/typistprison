#include "basetextedittab.h"

BaseTextEditTab::BaseTextEditTab(const QString &content, const QString &filePath, QWidget *parent)
    : QWidget(parent)
    , currentFilePath(filePath)
    , content(content)
{
}

BaseTextEditTab::~BaseTextEditTab()
{
}

QString BaseTextEditTab::getCurrentFilePath() const
{
    return currentFilePath;
}

void BaseTextEditTab::setFilePath(const QString &path)
{
    currentFilePath = path;
}

bool BaseTextEditTab::saveContent()
{
    if (currentFilePath.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");
        if (fileName.isEmpty()) {
            return false;
        }
        currentFilePath = fileName;
        emit onChangeFileType(fileName);
    }

    QFile file(currentFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save Error", "Unable to open file for writing.");
        return false;
    }

    QTextStream out(&file);
    out << getTextContent();
    file.close();

    emit onChangeTabName(QFileInfo(currentFilePath).fileName());
    return true;
}