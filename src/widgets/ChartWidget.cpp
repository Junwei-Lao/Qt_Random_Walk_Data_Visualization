#include "ChartWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent),
      m_titleLabel(new QLabel(this)),
      m_chart(new QChart()),
      m_chartView(new QChartView(m_chart, this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_chartView);

    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_chart->setTitle("Chart");
    m_titleLabel->setText("Select a CSV file.");
}

void ChartWidget::showFile(const QString &filePath) {
    QFileInfo info(filePath);
    QString name = info.fileName();

    if (name.contains("SingleWalk", Qt::CaseInsensitive)) {
        showWalk(filePath);
    } else {
        showHistogram(filePath);
    }
}

void ChartWidget::showWalk(const QString &filePath) {
    m_chart->removeAllSeries();

    QVector<QPointF> pts = loadWalkCsv(filePath);
    pts = downsample(pts, 1000);

    QLineSeries *series = new QLineSeries();
    for (const QPointF &p : pts) {
        series->append(p);
    }

    m_chart->addSeries(series);
    m_chart->createDefaultAxes();
    m_chart->setTitle("Random Walk Path");

    m_titleLabel->setText(
        QString("File: %1  |  Points: %2")
        .arg(QFileInfo(filePath).fileName())
        .arg(pts.size())
    );
}

void ChartWidget::showHistogram(const QString &filePath) {
    m_chart->removeAllSeries();

    QVector<double> data = loadSingleColumnCsv(filePath);
    if (data.isEmpty()) {
        m_titleLabel->setText("Unable to parse CSV.");
        return;
    }

    double minV = data[0];
    double maxV = data[0];
    for (double v : data) {
        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
    }
    if (minV == maxV) {
        maxV = minV + 1.0;
    }

    int bins = 30;
    QVector<int> counts(bins, 0);
    for (double v : data) {
        double t = (v - minV) / (maxV - minV);
        int idx = int(t * bins);
        if (idx < 0) idx = 0;
        if (idx >= bins) idx = bins - 1;
        counts[idx]++;
    }

    QBarSet *barSet = new QBarSet("count");
    for (int c : counts) {
        *barSet << c;
    }

    QBarSeries *series = new QBarSeries();
    series->append(barSet);
    m_chart->addSeries(series);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Bin");
    axisX->setRange(0, bins);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Count");
    axisY->setRange(0, *std::max_element(counts.begin(), counts.end()));

    m_chart->setAxisX(axisX, series);
    m_chart->setAxisY(axisY, series);

    m_chart->setTitle("Histogram");

    m_titleLabel->setText(
        QString("File: %1  |  N = %2")
        .arg(QFileInfo(filePath).fileName())
        .arg(data.size())
    );
}

QVector<QPointF> ChartWidget::loadWalkCsv(const QString &filePath) {
    QVector<QPointF> pts;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return pts;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(',');
        if (parts.size() < 2) continue;

        bool ok1 = false, ok2 = false;
        double x = parts[0].toDouble(&ok1);
        double y = parts[1].toDouble(&ok2);

        if (ok1 && ok2) {
            pts.append(QPointF(x, y));
        }
    }

    return pts;
}

QVector<double> ChartWidget::loadSingleColumnCsv(const QString &filePath) {
    QVector<double> v;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return v;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(',');
        bool ok = false;
        double value = parts[0].toDouble(&ok);
        if (ok) {
            v.append(value);
        }
    }

    return v;
}

QVector<QPointF> ChartWidget::downsample(const QVector<QPointF> &pts, int maxPoints) {
    if (pts.size() <= maxPoints) return pts;

    QVector<QPointF> out;
    out.reserve(maxPoints);

    double step = double(pts.size() - 1) / double(maxPoints - 1);
    for (int i = 0; i < maxPoints; ++i) {
        int idx = int(i * step);
        if (idx < pts.size()) {
            out.append(pts[idx]);
        }
    }

    return out;
}
