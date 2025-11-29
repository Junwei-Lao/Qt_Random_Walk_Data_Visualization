// src/MainWindow.cpp
#include "MainWindow.h"
#include "widgets/ChartWidget.h"

#include <QSplitter>
#include <QTimer>
#include <QResizeEvent>
#include <QFileSystemModel>
#include <QTreeView>
#include <QDir>
#include <QFileInfo>
#include <QModelIndex>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_dataRoot(QStringLiteral("../data")),
      m_fsModel(new QFileSystemModel(this)),
      m_treeView(new QTreeView(this)),
      m_chartWidget(new ChartWidget(this))
{
    // Set up file system model (left pane)
    m_fsModel->setRootPath(m_dataRoot);
    m_fsModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
    m_fsModel->setNameFilters(QStringList() << "*.csv");
    m_fsModel->setNameFilterDisables(false); // hide non-csv files

    QModelIndex rootIndex = m_fsModel->index(m_dataRoot);

    m_treeView->setModel(m_fsModel);
    m_treeView->setRootIndex(rootIndex);
    // Show header so the user sees column names; you can hide it again
    // if you prefer a cleaner list view (use setHeaderHidden(true)).
    m_treeView->setHeaderHidden(false);
    m_treeView->setAnimated(true);

    connect(m_treeView, &QTreeView::clicked,
            this, &MainWindow::onFileClicked);

    // Programmatically control which metadata columns are visible.
    // Columns: 0 = Name, 1 = Size, 2 = Type, 3 = Date Modified
    constexpr bool kShowSize = true;
    constexpr bool kShowType = false; // set to true if Type should be visible
    constexpr bool kShowDate = false; // set to true if Date Modified should be visible
    m_treeView->setColumnHidden(1, !kShowSize);
    m_treeView->setColumnHidden(2, !kShowType);
    m_treeView->setColumnHidden(3, !kShowDate);

    // Adjust header resize modes and initial column widths.
    // Make the 'Name' column stretch to fill available space so it doesn't
    // get squeezed, and set a small initial width for the Size column.
    // Other columns can be set to ResizeToContents so they remain compact.
    QHeaderView *header = m_treeView->header();
    header->setSectionResizeMode(0, QHeaderView::Stretch);          // Name
    header->setSectionResizeMode(1, QHeaderView::Interactive);      // Size
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Type
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Date Modified
    // Set a modest initial width for the Size column (pixels).
    m_treeView->setColumnWidth(1, 80);

    // No View menu by design: metadata columns are set programmatically
    // and the user cannot toggle them at runtime. This allows the app to
    // present a consistent set of metadata columns by default.

    // Splitter: 30% left (tree), 70% right (chart)
    m_splitter = new QSplitter(this);
    m_splitter->addWidget(m_treeView);
    m_splitter->addWidget(m_chartWidget);
    m_splitter->setStretchFactor(0, 2);
    m_splitter->setStretchFactor(1, 3);

    setCentralWidget(m_splitter);

    // Ensure the splitter initially uses a 30/70 ratio. Using a single-shot timer
    // ensures the widget geometry is established before computing sizes.
    QTimer::singleShot(0, this, [this]
                       {
        if (!m_splitter) return;
        QList<int> sizes = m_splitter->sizes();
        int total = 0;
        for (int s : sizes) total += s;
        if (total <= 0) {
            // fall back to stretch factors if there are no sizes yet
            sizes = QList<int>{300, 700};
            total = 1000;
        }
        int left = total * 30 / 100;
        int right = total - left;
        m_splitter->setSizes(QList<int>{left, right}); });
    setWindowTitle("Random Walk Visualizer");
}

void MainWindow::onFileClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QFileInfo info = m_fsModel->fileInfo(index);
    if (!info.isFile())
        return;
    if (info.suffix().toLower() != QLatin1String("csv"))
        return;

    const QString path = info.absoluteFilePath();
    m_chartWidget->showFile(path);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (!m_splitter)
        return;
    // Maintain 30/70 split on window resize
    int width = m_splitter->size().width();
    if (width <= 0)
        return;
    int left = width * 30 / 100;
    int right = width - left;
    m_splitter->setSizes(QList<int>{left, right});
}
