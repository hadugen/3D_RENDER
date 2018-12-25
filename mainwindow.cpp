#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget (parent),
    _deltaCameraPos(0.0f, 0.0f),
    _currentCameraPos(0.0f, 0.0f, 0.0f) {

    setFixedSize(755, 755);
    setCursor(Qt::OpenHandCursor);

    Lamp::camPos = _currentCameraPos;

    _objects.append(new Cube);
    _objects.append(new Lamp(QVector3D(0.f, 0.0f, 1.5f), 10.0, Qt::red));
    _objects.append(new Lamp(QVector3D(-1.5f, 0.f, 0.f), 10.0, Qt::green));
    _objects.append(new Lamp(QVector3D(0.f, -1.5f, 0.f), 10.0, Qt::blue));

    _renderOneFrameTimer = new QTimer;
    connect(_renderOneFrameTimer, SIGNAL(timeout()), this, SLOT(update()));
    _renderOneFrameTimer->setTimerType(Qt::PreciseTimer);
    _renderOneFrameTimer->start(1000 / _frameRate);                         // таймер обновления экрана (_famerate == 60)
}

MainWindow::~MainWindow() {

}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QOpenGLWidget::mouseMoveEvent(event);
    switch (_lastButtonPressed) {
    case Qt::RightButton: {
        QPointF pos = event->pos();
        _deltaCameraPos += QVector2D((pos - _lastClickPos) / 8);
        _lastClickPos = pos;
        break;
    }
    case Qt::LeftButton: {
        if(_currentLamp != nullptr) {
            _currentLamp->moveTo(event->pos().x(), event->pos().y());
        }
        break;
    }
    default: {
        break;
    }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QOpenGLWidget::mousePressEvent(event);
    setCursor(Qt::ClosedHandCursor);
    _lastButtonPressed = event->button();
    switch (event->button()) {
    case Qt::RightButton:                                   // правая кнопка - вращение
        _lastClickPos = event->pos();
        break;
    case Qt::LeftButton:
        _currentLamp = Lamp::findLampByCoords(
                    event->x(),
                    event->y()
        );
        break;
    default:
        break;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QOpenGLWidget::mouseReleaseEvent(event);
    setCursor(Qt::OpenHandCursor);
    switch (event->button()) {
    case Qt::RightButton:
        _lastClickPos = event->pos();
        break;
    case Qt::LeftButton:
        _currentLamp = nullptr;
        break;
    default:
        break;
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    QOpenGLWidget::wheelEvent(event);
    _zAxisRotation += event->delta() / 50.0f;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QOpenGLWidget::keyPressEvent(event);
    switch (event->nativeVirtualKey()) {
    case 107: /* + */
        _frameRate = (_frameRate == 250) ? 250 : _frameRate + 10;
        _renderOneFrameTimer->setInterval(1000 / _frameRate);
        break;
    case 109: /* - */
        _frameRate = (_frameRate == 10) ? 10 : _frameRate - 10;
        _renderOneFrameTimer->setInterval(1000 / _frameRate);
        break;
    case Qt::Key_N:
        Lamp::setShadingType(NO_SHADING);
        break;
    case Qt::Key_D:
        Lamp::setShadingType(DEFAULT_SHADING);
        break;
    case Qt::Key_G:
        Lamp::setShadingType(GOURAUD_SHADING);
        break;
    case Qt::Key_P:
        Lamp::setShadingType(PHONG_SHADING);
        break;
    default:
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {           // paintEvent вызывается каждый раз, когда вызывается update()
    prepareFrame();
    _painter.begin(this);
    _painter.fillRect(event->rect(), Qt::black);
    _painter.drawImage(event->rect(), _frame);
    _painter.end();
}

void MainWindow::drawFPS(int frameTime) {
    QString text = QString::number((int)(1000.0 / frameTime));
    QFont font;
    font.setPixelSize(20);
    font.setBold(false);
    font.setFamily("Consolas");
    QPainterPath path;
    path.addText(0, 20, font,  text);
    _painter.begin(&_frame);
    _painter.setPen(Qt::red);
    _painter.setBrush(Qt::red);
    _painter.drawPath(path);
    _painter.end();
}

void MainWindow::renderObjectsOnFrame(Matrix4x4 matrix, QVector<AbstractGraphicalObject*> objects) {
    _frame = QImage(750, 750, QImage::Format_RGBA8888);
    drawFPS(_lastFrameWasAt.elapsed());
    _lastFrameWasAt = QTime::currentTime();
    for(AbstractGraphicalObject *obj : objects) {
        obj->renderOnImage(matrix, &_frame);
    }
}

void MainWindow::prepareFrame() {
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    Matrix4x4 matView;
    Matrix4x4 matProjection;
    QSizeF size = this->size();

    _xAxisRotation = getNewXAxisRotation();
    _yAxisRotation = getNewYAxisRotation();
    double zoom = 0.02 * _zAxisRotation + 3.0;

    double cameraXCoord = zoom * cos(_xAxisRotation) * cos(_yAxisRotation);
    double cameraYCoord = zoom * sin(_xAxisRotation);
    double cameraZCoord = -zoom * cos(_xAxisRotation) * sin(_yAxisRotation);
    _currentCameraPos = QVector3D(cameraXCoord, cameraYCoord, cameraZCoord);
    Lamp::camPos = _currentCameraPos;
    QVector3D objectPosition(0.0f, 0.0f, 0.0f);
    matView.lookAt(_currentCameraPos, objectPosition, cameraUp);
    double aspectRatio = size.height() / size.width();
    matProjection.toProjectionMatrix((_currentCameraPos - objectPosition).length(), aspectRatio);
    Matrix4x4 viewProjection = matProjection * matView;
    renderObjectsOnFrame(viewProjection, _objects);
    _deltaCameraPos = QVector2D(0, 0);
}

double MainWindow::getNewXAxisRotation() {
    double newXAxisRotation = _xAxisRotation + _deltaCameraPos.y() / 30.0;
    if (newXAxisRotation <= -M_PI_2) {
        return -M_PI_2 + 0.01;
    } else if(newXAxisRotation >= M_PI_2 ) {
        return M_PI_2 - 0.01;
    } else {
        return newXAxisRotation;
    }
}

double MainWindow::getNewYAxisRotation() {
    double newYAxisRotation = _yAxisRotation + _deltaCameraPos.x() / 30.0;
    if(newYAxisRotation > 2*M_PI) {
        newYAxisRotation -= 2*M_PI;
    } else if(newYAxisRotation < 0) {
        newYAxisRotation += 2*M_PI;
    }
    return newYAxisRotation;
}

