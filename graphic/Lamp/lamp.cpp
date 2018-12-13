#include "lamp.h"

Lamp::Lamp(QVector3D position, double zoom, QColor color) {
    _absolutePosition = position;
    _radius = zoom;
    _screenPosition = QVector3D(0, 0, 0);
    _color = color;
}

void Lamp::moveTo(int x, int y) {
    _newScreenPosition = QVector3D(x, y, _screenPosition.z());
    _isMoved = true;
}

void Lamp::renderOnImage(Matrix4x4 viewProjection, QImage *image) {
    std::map<int, LineX> lines;
    QPainter painter(image);
    painter.setPen(_color);
    painter.setBrush(_color);
    if(_isMoved) {
        _newScreenPosition.setZ(3);
        _absolutePosition = Utils::screenToWorld(_newScreenPosition, viewProjection, image->size());
        _isMoved = false;
    }
    _screenPosition = Utils::worldToScreen(_absolutePosition, viewProjection, image->size());
    QVector4D pos(_absolutePosition, 1);
    pos = viewProjection * pos;
    Utils::addBorderCircle(_screenPosition, _radius, lines);
    std::map<int, LineX>::iterator it;;
    for(it = lines.begin(); it != lines.end(); it++) {
        int y = it->first;
        LineX line = it->second;
        painter.drawLine(line.x1, y, line.x2, y);
    }
}

void Lamp::addVerts() {

}

void Lamp::addEdges() {

}

void Lamp::addFaces() {

}

bool Lamp::itsMe(int x, int y) {
    QVector2D vec = QVector2D(_screenPosition.x(), _screenPosition.y()) - QVector2D(x, y);
    float dist = vec.x() * vec.x() + vec.y() * vec.y();
    if(dist < _radius * _radius) {
        return true;
    } else {
        return false;
    }
}
