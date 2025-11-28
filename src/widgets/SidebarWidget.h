#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>

class QTreeView;
class QFileSystemModel;

class SidebarWidget : public QWidget {
    Q_OBJECT
    public:
        explicit SidebarWidget(QWidget *parent = nullptr);

    signals:
        void fileSelected(QString path);

    private slots:
        void onItemActivated(const QModelIndex &index);

    private:
        QTreeView *treeView;
        QFileSystemModel *model;
};

#endif // SIDEBARWIDGET_H
