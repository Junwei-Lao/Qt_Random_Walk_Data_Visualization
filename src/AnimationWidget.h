#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QPointF>
#include <QPolygonF>
#include <QPen>
#include <vector>

#include <QGraphicsEllipseItem>


class AnimationWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit AnimationWidget(QWidget *parent = nullptr);
    void loadFile(const QString &path);
    void setSpeed(int ms);

private slots:
    void animateStep();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    QGraphicsScene *scene;
    QTimer timer;

    std::vector<QPointF> steps;
    int currentStep = 0;

    QPolygonF boundaryPolygon;
    bool hasCircle = false;
    QPointF circleCenter;
    double circleRadius = 0.0;

    // Grid settings
    double gridSpacing = 1.0;
    QPen gridPenMinor;
    QPen gridPenMajor;

    QGraphicsEllipseItem *currentDot = nullptr;

};
