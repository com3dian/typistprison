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