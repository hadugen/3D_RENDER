#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <matrix4x4.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QTime>
#include <QPainterPath>
#include <QDebug>
#include <QTimer>
#include <QtConcurrent>
#include <Cube/cube.h>

struct ShadePackage;

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void render(Matrix4x4 matrix, QVector<AbstractGraphicalObject *> objects);
    void start();

private:

    QGraphicsScene *_scene;
    QVector <AbstractGraphicalObject*> _objects;

    QVector2D _deltaCameraPos;
    QVector3D _currentCameraPos;

    double _xAxisRotation = 0.5;
    double _yAxisRotation = 0.5;
    double _zAxisRotation = 0.5;

    QImage _frame;
    QPainter _painter;

    QPointF _lastClickPos;

    QTime _lastFrameWasAt;
    uint _frameRate = 60;

    QTimer *_renderOneFrameTimer;

    void drawFPS(int frameTime);

    double getNormalForVector(const QVector3D &vec);

    double getYAxisRotation();
    double getXAxisRotation();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // MAINWINDOW_H
