#pragma once
#include <QListWidget>

class FileListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit FileListWidget(QWidget *parent = nullptr);

signals:
    void fileSelected(const QString &path);

private slots:
    void onItemClicked(QListWidgetItem *item);

private:
    QString dataDir;
};
