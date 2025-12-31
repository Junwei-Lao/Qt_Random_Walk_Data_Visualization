#include "FileListWidget.h"

#include <QDir>
#include <QFileInfoList>

FileListWidget::FileListWidget(QWidget *parent)
    : QListWidget(parent)
{
    dataDir = QDir::currentPath() + "/../data";

    QDir dir(dataDir);
    QFileInfoList files = dir.entryInfoList(QStringList() << "*.txt",
                                             QDir::Files);

    for (const QFileInfo &info : files)
        addItem(info.fileName());

    connect(this, &QListWidget::itemClicked,
            this, &FileListWidget::onItemClicked);
}

void FileListWidget::onItemClicked(QListWidgetItem *item)
{
    emit fileSelected(dataDir + "/" + item->text());
}
