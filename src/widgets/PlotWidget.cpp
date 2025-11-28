#include "PlotWidget.h"
#include <QVBoxLayout>
#include <QLabel>

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    label = new QLabel("Select a CSV file to plot.", this);
    layout->addWidget(label);
}

void PlotWidget::loadCsv(QString filepath)
{
    label->setText("Loaded CSV: " + filepath);
    // Later: parse CSV and plot here
}
