#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QGraphicsView (parent),
    _deltaCameraPos(0.0f, 0.0f),
    _currentCameraPos(0.0f, 0.0f, 0.0f) {

    _scene = new QGraphicsScene(0, 0, 750, 750);
    setScene(_scene);
    setFixedSize(755, 755);
    setCursor(Qt::OpenHandCursor);

    _objects.append(new Cube);

    _renderOneFrameTimer = new QTimer;
    connect(_renderOneFrameTimer, SIGNAL(timeout()), this, SLOT(start()));
    _renderOneFrameTimer->setTimerType(Qt::PreciseTimer);
    _renderOneFrameTimer->start(1000 / _frameRate);
}

MainWindow::~MainWindow() {

}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    QPointF pos = mapToScene(event->pos());
    _deltaCameraPos += QVector2D((pos - _lastClickPos) / 8);
    _lastClickPos = pos;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    _lastClickPos = mapToScene(event->pos());
    setCursor(Qt::ClosedHandCursor);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    setCursor(Qt::OpenHandCursor);
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    QGraphicsView::wheelEvent(event);
    _zAxisRotation += event->delta() / 50.0f;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    switch (event->nativeVirtualKey()) {
    case 107: /* + */
        _frameRate = (_frameRate == 250) ? 250 : _frameRate + 10;
        _renderOneFrameTimer->setInterval(1000 / _frameRate);
        break;
    case 109: /* - */
        _frameRate = (_frameRate == 10) ? 10 : _frameRate - 10;
        _renderOneFrameTimer->setInterval(1000 / _frameRate);
        break;
    case Qt::Key_1:
        break;
    case Qt::Key_2:
        break;
    default:
        break;
    }
}

void MainWindow::drawFPS(int frameTime) {
    QString text = QString::number((int)(1000.0 / frameTime));
    QFont font;
    font.setPixelSize(20);
    font.setBold(false);
    font.setFamily("Consolas");
    QPainterPath path;
    path.addText(0, 20, font,  text);
    _scene->addPath(path, QPen(QBrush(Qt::red), 1), QBrush(Qt::red));
}

void MainWindow::render(Matrix4x4 matrix, QVector<AbstractGraphicalObject*> objects) {
    _scene->clear();
    drawFPS(_lastFrameWasAt.elapsed());
    _lastFrameWasAt = QTime::currentTime();
    _frame = QImage(750, 750, QImage::Format_RGBA8888);
    for(AbstractGraphicalObject *obj : objects) {
        obj->renderOnImage(matrix, &_frame);
    }
    _scene->addPixmap(QPixmap::fromImage(_frame));
}

void MainWindow::start() {
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    Matrix4x4 matView;
    Matrix4x4 matProjection;
    QSizeF size = sceneRect().size();

    _xAxisRotation = getXAxisRotation();
    _yAxisRotation = getYAxisRotation();
    double zoom = 0.02 * _zAxisRotation + 3.0;

    double cameraXCoord = zoom * cos(_xAxisRotation) * cos(_yAxisRotation);
    double cameraYCoord = zoom * sin(_xAxisRotation);
    double cameraZCoord = -zoom * cos(_xAxisRotation) * sin(_yAxisRotation);
    _currentCameraPos = QVector3D(cameraXCoord, cameraYCoord, cameraZCoord);
    QVector3D objectPosition(0.0f, 0.0f, 0.0f);
    matView.lookAt(_currentCameraPos, objectPosition, cameraUp);
    double aspectRatio = size.height() / size.width();
    matProjection.toProjectionMatrix(getNormalForVector(_currentCameraPos - objectPosition), aspectRatio);
    Matrix4x4 viewProjection = matProjection * matView;
    render(viewProjection, _objects);
    _deltaCameraPos = QVector2D(0, 0);
}

double MainWindow::getNormalForVector(const QVector3D &vec) {
    return std::sqrt(vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z());
}

double MainWindow::getXAxisRotation() {
    double newXAxisRotation = _xAxisRotation + _deltaCameraPos.y() / 30.0;
    if (newXAxisRotation <= -M_PI_2) {
        return -M_PI_2 + 0.01;
    } else if(newXAxisRotation >= M_PI_2 ) {
        return M_PI_2 - 0.01;
    } else {
        return newXAxisRotation;
    }
}

double MainWindow::getYAxisRotation() {
    double newYAxisRotation = _yAxisRotation + _deltaCameraPos.x() / 30.0;
    if(newYAxisRotation > 2*M_PI) {
        newYAxisRotation -= 2*M_PI;
    } else if(newYAxisRotation < 0) {
        newYAxisRotation += 2*M_PI;
    }
    return newYAxisRotation;
}

