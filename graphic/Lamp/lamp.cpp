#include "lamp.h"

QVector <Lamp*> Lamp::_lamps;
ShadingType Lamp::_shadingType = NO_SHADING;
QVector3D Lamp::camPos;

void Lamp::setShadingType(ShadingType type) {
    _shadingType = type;
}

ShadingType Lamp::shadingType() {
    return _shadingType;
}

Lamp::Lamp(QVector3D position, double radius, QColor color) {
    _absolutePosition = position;
    _radius = radius;
    _squaredRadius = radius * radius;
    _screenPosition = QVector3D(0, 0, 0);
    _color = color;
    _intensity = QVector2D(0.4f, 0.5f);
    _attenuation = QVector2D(0.1f, 0.1f);
    _lamps.append(this);
}

Lamp::~Lamp() {
    _lamps.removeOne(this);
}

void Lamp::bresCircle(QVector3D point, int x, int y, std::map<int, LineX>& lines)
{
    int xc = static_cast<int>(point.x());
    int yc = static_cast<int>(point.y());
    float zc = point.z();
    addBorderPixel(QVector3D(xc + x, yc + y, zc), lines);
    addBorderPixel(QVector3D(xc - x, yc + y, zc), lines);
    addBorderPixel(QVector3D(xc + x, yc - y, zc), lines);
    addBorderPixel(QVector3D(xc - x, yc - y, zc), lines);
    addBorderPixel(QVector3D(xc + y, yc + x, zc), lines);
    addBorderPixel(QVector3D(xc - y, yc + x, zc), lines);
    addBorderPixel(QVector3D(xc + y, yc - x, zc), lines);
    addBorderPixel(QVector3D(xc - y, yc - x, zc), lines);
}

void Lamp::addBorderCircle(QVector3D center, int radius, std::map<int, LineX>& lines)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    while (y >= x) {
        bresCircle(center, x, y, lines);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else {
            d = d + 4 * x + 6;
        }
    }
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
    addBorderCircle(_screenPosition, _radius, lines);
    std::map<int, LineX>::iterator it;
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

Lamp * Lamp::findLampByCoords(double x, double y) {
    for(Lamp *lamp : _lamps) {
        QVector2D vec = QVector2D(lamp->_screenPosition.x(), lamp->_screenPosition.y()) - QVector2D(x, y);
        double dist = vec.x() * vec.x() + vec.y() * vec.y();
        if(dist < lamp->_squaredRadius) {
            return lamp;
        }
    }
    return nullptr;
}

QVector4D Lamp::calcPhongLightOnPoint(QVector3D worldPos, QVector3D normal) {
   QVector3D lightDirection = worldPos - _absolutePosition;
    float dist2 = lightDirection.lengthSquared();
    if (dist2 > _squaredRadius) {
        return QVector4D();
    }
    float dist = sqrt(dist2);
    lightDirection.normalize();
    QVector4D ambientColor = QVector4D(_color.red(), _color.green(), _color.blue(), 1.f) * _intensity.x();
    float diffuseFactor = QVector3D::dotProduct(normal, -lightDirection);
    QVector4D diffuseColor;
    QVector4D specularColor;
    if (diffuseFactor > 0) {
        diffuseColor = QVector4D(_color.red(), _color.green(), _color.blue(), 1.0) * _intensity.y() * diffuseFactor;
        QVector3D vertexToEye = (worldPos - camPos).normalized();
        QVector3D lightReflect = (Utils::reflect(lightDirection, normal)).normalized();
        float specularFactor = QVector3D::dotProduct(vertexToEye, lightReflect);
        specularFactor = powf(specularFactor, 0.3f);
        if (specularFactor > 0.0) {
            specularColor = QVector4D(_color.red(), _color.green(), _color.blue(), 1.0) * (0.2f * specularFactor);
        }
    }
    QVector4D color = ambientColor + diffuseColor;
    float attenuation = 1.0 + this->_attenuation.x() * dist + this->_attenuation.y() * dist2;
    attenuation = std::max(1.f, attenuation);
    return color / attenuation;
}

QVector4D Lamp::calcDefaultLightOnPoint(QVector3D worldPos, QVector3D normal) {
    QVector3D lightDirection = worldPos - _absolutePosition;
    float dist2 = lightDirection.lengthSquared();
    if (dist2 > _squaredRadius) {
        return QVector4D();
    }
    float dist = sqrt(dist2);
    lightDirection.normalize();
    float diffuseFactor = QVector3D::dotProduct(normal, -lightDirection);
    QVector4D diffuseColor = QVector4D(_color.red(), _color.green(), _color.blue(), 1.0) * _intensity.y() * 2.f * diffuseFactor;
    float attenuation = 1.0 + this->_attenuation.x() * dist + this->_attenuation.y() * dist2;
    attenuation = std::max(1.f, attenuation);
    return diffuseColor / attenuation;
}

QVector4D Lamp::calcLightOnPoint(QVector3D worldPos, QVector3D normal) {
    switch (Lamp::_shadingType) {
    case DEFAULT_SHADING:   return calcDefaultLightOnPoint(worldPos, normal);
    case PHONG_SHADING:     return calcDefaultLightOnPoint(worldPos, normal);
    case GOURAUD_SHADING:   return calcPhongLightOnPoint(worldPos, normal);
    default:                return calcPhongLightOnPoint(worldPos, normal);
    }
}

QColor Lamp::calcSummaryLight(QVector3D worldPos, QVector3D normal) {
    QVector4D colorVector;
    for(Lamp *lamp : _lamps) {
        if(Utils::IsLit(normal, worldPos, lamp->position())) {
            colorVector = colorVector + lamp->calcLightOnPoint(worldPos, normal);
        }
    }
    return QColor(std::min(colorVector.x(), 255.f), std::min(colorVector.y(), 255.f), std::min(colorVector.z(), 255.f));
}

QVector <QVector4D> Lamp::getIntensVector(QVector3D worldPos, QVector3D normal) {
    QVector <QVector4D> intens;
    for(Lamp *lamp : _lamps) {
        intens.push_back(lamp->calcLightOnPoint(worldPos, normal));
    }
    return intens;
}
