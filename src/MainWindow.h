// src/MainWindow.h
#pragma once

#include <QMainWindow>
#include <QString>

class QSplitter;
class QResizeEvent;

class QFileSystemModel;
class QTreeView;
class QModelIndex;
class ChartWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onFileClicked(const QModelIndex &index);

private:
    QString m_dataRoot;
    QFileSystemModel *m_fsModel;
    QTreeView *m_treeView;
    ChartWidget *m_chartWidget;
    QSplitter *m_splitter;

protected:
    void resizeEvent(QResizeEvent *event) override;
};
