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
#include <QPaintEvent>
#include <QDebug>
#include <QTimer>
#include <Cube/cube.h>
#include <QOpenGLWidget>

struct ShadePackage;

class MainWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void renderObjOnFrame(Matrix4x4 matrix, QVector<AbstractGraphicalObject *> objects);
    void prepareFrame();

private:

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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // MAINWINDOW_H
