#include "lamp.h"

QVector <Lamp*> Lamp::lamps;

Lamp::Lamp(QVector3D position, double zoom, QColor color) {
    _absolutePosition = position;
    _radius = zoom;
    _screenPosition = QVector3D(0, 0, 0);
    _color = color;
    _intensity = QVector2D(0.4f, 0.5f);
    _attenuation = QVector2D(0.1f, 0.1f);
    lamps.append(this);
}

Lamp::~Lamp() {
    lamps.removeOne(this);
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
        _newScreenPosition.setZ(2.5);
        _absolutePosition = Utils::screenToWorld(_newScreenPosition, viewProjection, image->size());
        _isMoved = false;
    }
    _screenPosition = Utils::worldToScreen(_absolutePosition, viewProjection, image->size());
    Utils::addBorderCircle(_screenPosition, _radius, lines);
    std::map<int, LineX>::iterator it;;
    for(it = lines.begin(); it != lines.end(); it++) {
        int y = it->first;
        LineX line = it->second;
        painter.drawLine(line.x1, y, line.x2, y);
    }
}

QVector3D Lamp::position() {
    return _absolutePosition;
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

QVector4D Lamp::calcLightOnPoint(QVector3D worldPos, QVector3D normal) {
    QVector3D lightDirection = worldPos - _absolutePosition;
    float dist2 = lightDirection.lengthSquared();
    if (dist2 > _radius * _radius) {
        return QVector4D();
    }
    float dist = sqrt(dist2);
    lightDirection.normalize();
    QVector4D ambientColor = QVector4D(_color.red(), _color.green(), _color.blue(), 1.f) * _intensity.x();
    float diffuseFactor = QVector3D::dotProduct(normal, -lightDirection);
    QVector4D diffuseColor = QVector4D(_color.red(), _color.green(), _color.blue(), 1.0) * _intensity.y() * 2.f * diffuseFactor;
    float attenuation = 1.0 + _attenuation.x() * dist + _attenuation.y() * dist2;
    attenuation = std::max(1.f, attenuation);
    return diffuseColor / attenuation;
}

QColor Lamp::calcSummaryLight(QVector3D worldPos, QVector3D normal) {
    QVector4D colorVector;
    for(Lamp *lamp : lamps) {
        if(Utils::IsLit(normal, worldPos, lamp->position())) {
            colorVector = colorVector + lamp->calcLightOnPoint(worldPos, normal);
        }
    }
    return QColor(std::min(colorVector.x(), 255.f), std::min(colorVector.y(), 255.f), std::min(colorVector.z(), 255.f));
}
