#include "MainWindow.h"
#include "widgets/SidebarWidget.h"
#include "widgets/PlotWidget.h"

#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Central widget and main horizontal layout
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *layout = new QHBoxLayout(central);

    // Create left and right widgets
    auto sidebar = new SidebarWidget(central);
    auto plotArea = new PlotWidget(central);

    // Add to layout with stretch factors
    layout->addWidget(sidebar, 35);  // 35% width
    layout->addWidget(plotArea, 65); // 65% width

    // Connect sidebar to plot widget
    connect(sidebar, &SidebarWidget::fileSelected,
            plotArea, &PlotWidget::loadCsv);


    setWindowTitle("Qt Data Visualization App");
    resize(1000, 600);
}
