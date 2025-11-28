#include "SidebarWidget.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QFont>

SidebarWidget::SidebarWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    treeView = new QTreeView(this);
    model = new QFileSystemModel(this);

    QString rootPath = QCoreApplication::applicationDirPath() + "/../data";

    // Show all dirs + files, but only let CSV actually appear as files
    model->setRootPath(rootPath);
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    // Only CSV files appear as files (folders always appear)
    model->setNameFilters(QStringList() << "*.csv");
    model->setNameFilterDisables(false);

    treeView->setModel(model);
    treeView->setRootIndex(model->index(rootPath));

    // OPTIONAL: make it look nicer
    treeView->setHeaderHidden(true);
    treeView->setSortingEnabled(true);

    // 🔹 Change font size of displayed names here:
    QFont f = treeView->font();
    f.setPointSize(12);              // adjust this number
    treeView->setFont(f);

    layout->addWidget(treeView);

    connect(treeView, &QTreeView::doubleClicked,
            this, &SidebarWidget::onItemActivated);
}

void SidebarWidget::onItemActivated(const QModelIndex &index)
{
    if (!index.isValid()) return;

    // Only emit for files (not folders)
    if (!model->isDir(index)) {
        QString path = model->filePath(index);
        emit fileSelected(path);
    }
}
