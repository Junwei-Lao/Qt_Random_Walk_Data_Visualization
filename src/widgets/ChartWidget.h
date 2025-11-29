#pragma once

#include <QWidget>
#include <QString>
#include <QVector>
#include <QPointF>

class QLabel;

class QChart;
class QChartView;
class QLineSeries;
class QBarSeries;
class QBarSet;
class QValueAxis;

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);
    void showFile(const QString &filePath);

private:
    void showWalk(const QString &filePath);
    void showHistogram(const QString &filePath);

    QVector<QPointF> loadWalkCsv(const QString &filePath);
    QVector<double> loadSingleColumnCsv(const QString &filePath);
    QVector<QPointF> downsample(const QVector<QPointF> &pts, int maxPoints);

    QLabel *m_titleLabel;
    QChart *m_chart;
    QChartView *m_chartView;
};
