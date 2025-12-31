#include "AnimationWidget.h"
#include <QFile>
#include <QTextStream>
#include <QWheelEvent>
#include <QPainter>
#include <cmath>

#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>



AnimationWidget::AnimationWidget(QWidget *parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this))
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    // IMPORTANT for grid redraw + smooth zooming
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setCacheMode(QGraphicsView::CacheNone);

    // Grid pens
    gridPenMinor = QPen(QColor(220, 220, 220));
    gridPenMinor.setCosmetic(true);

    gridPenMajor = QPen(QColor(190, 190, 190));
    gridPenMajor.setWidth(2);
    gridPenMajor.setCosmetic(true);

    // drag mode
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setCursor(Qt::OpenHandCursor);


    connect(&timer, &QTimer::timeout,
            this, &AnimationWidget::animateStep);
}

void AnimationWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);

    const double s = gridSpacing;
    if (s <= 0.0) {
        painter->restore();
        return;
    }

    const double left   = std::floor(rect.left()   / s) * s;
    const double right  = std::ceil (rect.right()  / s) * s;
    const double top    = std::floor(rect.top()    / s) * s;
    const double bottom = std::ceil (rect.bottom() / s) * s;

    // Minor grid
    painter->setPen(gridPenMinor);
    for (double x = left; x <= right; x += s) painter->drawLine(QLineF(x, top, x, bottom));
    for (double y = top;  y <= bottom; y += s) painter->drawLine(QLineF(left, y, right, y));

    // Major grid every 5
    painter->setPen(gridPenMajor);
    const double ms = s * 5.0;

    const double mleft   = std::floor(rect.left()   / ms) * ms;
    const double mright  = std::ceil (rect.right()  / ms) * ms;
    const double mtop    = std::floor(rect.top()    / ms) * ms;
    const double mbottom = std::ceil (rect.bottom() / ms) * ms;

    for (double x = mleft; x <= mright; x += ms) painter->drawLine(QLineF(x, top, x, bottom));
    for (double y = mtop;  y <= mbottom; y += ms) painter->drawLine(QLineF(left, y, right, y));

    painter->restore();
}

void AnimationWidget::loadFile(const QString &path)
{
    timer.stop();
    currentDot = nullptr;

    steps.clear();
    boundaryPolygon.clear();
    hasCircle = false;
    currentStep = 0;

    scene->clear();

    // Give a valid default rect so background paints immediately
    scene->setSceneRect(QRectF(-100, -100, 200, 200));

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    // ---- Read first three lines ----
    if (in.atEnd()) return;
    int shapeType = in.readLine().trimmed().toInt();

    if (in.atEnd()) return;
    QString lineX = in.readLine().trimmed();

    if (in.atEnd()) return;
    QString lineY = in.readLine().trimmed();

    // ---- Parse shape ----
    if (shapeType == 1) {
        bool ok1 = false, ok2 = false;
        double cx = lineX.toDouble(&ok1);
        double cy = lineY.toDouble(&ok2);
        if (ok1 && ok2) {
            hasCircle = true;
            circleCenter = QPointF(cx, -cy);   // flip Y once
            circleRadius = std::abs(cx);       // your chosen convention
        }
    } else {
        QStringList xs = lineX.split(' ', Qt::SkipEmptyParts);
        QStringList ys = lineY.split(' ', Qt::SkipEmptyParts);

        int n = std::min(xs.size(), ys.size());
        for (int i = 0; i < n; ++i) {
            bool ok1 = false, ok2 = false;
            double x = xs[i].toDouble(&ok1);
            double y = ys[i].toDouble(&ok2);
            if (ok1 && ok2)
                boundaryPolygon << QPointF(x, -y); // flip Y once
        }
    }

    // ---- Draw boundary behind the walk ----
    QPen boundaryPen(Qt::darkGreen);
    boundaryPen.setWidth(2);
    boundaryPen.setCosmetic(true);

    if (hasCircle) {
        auto *circleItem = scene->addEllipse(circleCenter.x() - circleRadius,
                                             circleCenter.y() - circleRadius,
                                             2 * circleRadius,
                                             2 * circleRadius,
                                             boundaryPen);
        circleItem->setZValue(1);
    } else if (boundaryPolygon.size() >= 2) {
        if (boundaryPolygon.first() != boundaryPolygon.last())
            boundaryPolygon << boundaryPolygon.first();

        auto *polyItem = scene->addPolygon(boundaryPolygon, boundaryPen);
        polyItem->setZValue(1);
    }

    // ---- Parse random walk ----
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 2) continue;

        bool ok1 = false, ok2 = false;
        double x = parts[0].toDouble(&ok1);
        double y = parts[1].toDouble(&ok2);
        if (ok1 && ok2)
            steps.emplace_back(x, -y); // flip Y once
    }

    // ---- Set view bounds and start ----
    if (!steps.empty()) {
        QRectF bounds;

        if (hasCircle) {
            bounds = QRectF(circleCenter - QPointF(circleRadius, circleRadius),
                            QSizeF(2 * circleRadius, 2 * circleRadius));
        } else if (!boundaryPolygon.isEmpty()) {
            bounds = boundaryPolygon.boundingRect();
        }

        for (const auto &p : steps)
            bounds |= QRectF(p, QSizeF(1, 1));

        bounds.adjust(-20, -20, 20, 20);
        scene->setSceneRect(bounds);
        fitInView(bounds, Qt::KeepAspectRatio);

        timer.start(50);
    }
}

void AnimationWidget::animateStep()
{
    if (currentStep + 1 >= (int)steps.size()) {
        timer.stop();
        return;
    }

    QPointF p1 = steps[currentStep];
    QPointF p2 = steps[currentStep + 1];

    // Draw path segment
    auto *line = scene->addLine(p1.x(), p1.y(),
                                p2.x(), p2.y(),
                                QPen(Qt::black));
    line->setZValue(2);

    // Turn previous blue dot red
    if (currentDot && currentDot->scene() == scene) {
        QPen redPen(Qt::red);
        redPen.setCosmetic(true);
        currentDot->setPen(redPen);
        currentDot->setBrush(QBrush(Qt::red));
    }

    // Draw new blue dot
    const double r = 0.25;   // your chosen radius
    QPen bluePen(Qt::blue);
    bluePen.setCosmetic(true);

    currentDot = scene->addEllipse(
        p2.x() - r, p2.y() - r,
        2*r, 2*r,
        bluePen,
        QBrush(Qt::blue));

    currentDot->setZValue(3);

    currentStep++;
}


void AnimationWidget::setSpeed(int ms)
{
    timer.setInterval(ms);
}

void AnimationWidget::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}
