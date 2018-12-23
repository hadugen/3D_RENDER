#include "abstractgraphicalobject.h"

AbstractGraphicalObject::AbstractGraphicalObject() {

}

AbstractGraphicalObject::~AbstractGraphicalObject() {

}

QVector<QVector3D> AbstractGraphicalObject::verts() const {
    return _verts;
}

QVector<QVector2D> AbstractGraphicalObject::edges() const {
    return _edges;
}

QVector<QVector3D> AbstractGraphicalObject::faces() const {
    return _faces;
}

void AbstractGraphicalObject::addBorderPixel(Dot3D point, std::map<int, LineX>& lines) {
    int x = static_cast<int>(point.cam.x() + 0.5);
    int y = static_cast<int>(point.cam.y() + 0.5);
    float z = point.cam.z();
    std::map<int, LineX>::iterator it;
    it = lines.find(y);
    if(it != lines.end()) {
        LineX line = it->second;
        if(x < line.x1) {
            line.x1 = x;
            line.z1 = z;
            line.absoluteBeg = point.absolute;
        } else if( x > line.x2) {
            line.x2 = x;
            line.z2 = z;
            line.absoluteEnd = point.absolute;
        }
        lines[y] = line;
    } else {
        LineX line(x, x, z, z, point.absolute, point.absolute);
        lines[y] = line;
    }
}

void AbstractGraphicalObject::addBorderPixel(GuroDot point, std::map<int, LineGuroX>& lines) {
    int x = static_cast<int>(point.cam.x() + 0.5);
    int y = static_cast<int>(point.cam.y() + 0.5);
    float z = point.cam.z();
    std::map<int, LineGuroX>::iterator it;
    it = lines.find(y);
    if(it != lines.end()) {
        LineGuroX line = it->second;
        if(x < line.x1) {
            line.x1 = x;
            line.z1 = z;
            line.absoluteBeg = point.absolute;
            line.begIntens = point.intens;
        } else if( x > line.x2) {
            line.x2 = x;
            line.z2 = z;
            line.absoluteEnd = point.absolute;
            line.endIntens = point.intens;
        }
        lines[y] = line;
    } else {
        LineGuroX line(x, x, z, z, point.absolute, point.absolute);
        line.begIntens = point.intens;
        line.endIntens = point.intens;
        lines[y] = line;
    }
}

void AbstractGraphicalObject::addBorderPixel(FongDot point, std::map<int, LineFongX>& lines) {
    int x = static_cast<int>(point.cam.x() + 0.5);
    int y = static_cast<int>(point.cam.y() + 0.5);
    float z = point.cam.z();
    std::map<int, LineFongX>::iterator it;
    it = lines.find(y);
    if(it != lines.end()) {
        LineFongX line = it->second;
        if(x < line.x1) {
            line.x1 = x;
            line.z1 = z;
            line.absoluteBeg = point.absolute;
            line.begNormal = point.normal;
        } else if(x > line.x2) {
            line.x2 = x;
            line.z2 = z;
            line.absoluteEnd = point.absolute;
            line.endNormal = point.normal;
        }
        lines[y] = line;
    } else {
        LineFongX line(x, x, z, z, point.absolute, point.absolute);
        line.begNormal = point.normal;
        line.endNormal = point.normal;
        lines[y] = line;
    }
}

void AbstractGraphicalObject::addBorderPixel(QVector3D point, std::map<int, LineX>& lines) {
    Dot3D dot(QVector3D(0.f, 0.f, 0.f), point);
    addBorderPixel(dot, lines);
}
