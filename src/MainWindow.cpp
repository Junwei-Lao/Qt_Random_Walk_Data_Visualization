#include "MainWindow.h"
#include "FileListWidget.h"
#include "AnimationWidget.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QSplitter>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ---- Central container ----
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // ---- Splitter ----
    QSplitter *splitter = new QSplitter(Qt::Horizontal, central);

    fileList = new FileListWidget(splitter);
    animation = new AnimationWidget(splitter);

    splitter->addWidget(fileList);
    splitter->addWidget(animation);

    // Initial 30% / 70%
    splitter->setSizes({200, 800});
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 8);

    // ---- Speed slider ----
    QSlider *speedSlider = new QSlider(Qt::Horizontal, central);
    speedSlider->setRange(1, 50);   // milliseconds per step
    speedSlider->setValue(25);        // default speed

    connect(speedSlider, &QSlider::valueChanged,
            animation, &AnimationWidget::setSpeed);

    // ---- Layout assembly ----
    layout->addWidget(splitter, 1);   // takes most space
    layout->addWidget(speedSlider, 0);

    setCentralWidget(central);

    // ---- File selection hookup ----
    connect(fileList, &FileListWidget::fileSelected,
            animation, &AnimationWidget::loadFile);


}

