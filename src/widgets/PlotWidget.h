#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

class QLabel;

class PlotWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = nullptr);

public slots:
    void loadCsv(QString filepath);

private:
    QLabel *label;
};

#endif // PLOTWIDGET_H
