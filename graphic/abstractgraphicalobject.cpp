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

void AbstractGraphicalObject::addPixel(Dot dot, std::map<int, Line>& lines) {
    int x = static_cast<int>(dot.cam.x() + 0.5);
    int y = static_cast<int>(dot.cam.y() + 0.5);
    float z = dot.cam.z();
    std::map<int, Line>::iterator it;
    it = lines.find(y);
    if(it != lines.end()) {
        Line line = it->second;
        if(x < line.x1) {
            line.x1 = x;
            line.z1 = z;
            line.absoluteBeg = dot.absolute;
        } else if( x > line.x2) {
            line.x2 = x;
            line.z2 = z;
            line.absoluteEnd = dot.absolute;
        }
        lines[y] = line;
    } else {
        Line line(x, x, z, z, dot.absolute, dot.absolute);
        lines[y] = line;
    }
}

void AbstractGraphicalObject::addPixel(GouraudDot dot, std::map<int, LineGouraud>& lines) {
    int x = static_cast<int>(dot.cam.x() + 0.5);
    int y = static_cast<int>(dot.cam.y() + 0.5);
    float z = dot.cam.z();
    std::map<int, LineGouraud>::iterator it;
    it = lines.find(y);
    if(it != lines.end()) {
        LineGouraud line = it->second;
        if(x < line.x1) {
            line.x1 = x;
            line.z1 = z;
            line.absoluteBeg = dot.absolute;
            line.begIntens = dot.intens;
        } else if( x > line.x2) {
            line.x2 = x;
            line.z2 = z;
            line.absoluteEnd = dot.absolute;
            line.endIntens = dot.intens;
        }
        lines[y] = line;
    } else {
        LineGouraud line(x, x, z, z, dot.absolute, dot.absolute);
        line.begIntens = dot.intens;
        line.endIntens = dot.intens;
        lines[y] = line;
    }
}

void AbstractGraphicalObject::addPixel(PhongDot dot, std::map<int, LinePhong>& lines) {
    int x = static_cast<int>(dot.cam.x() + 0.5);
    int y = static_cast<int>(dot.cam.y() + 0.5);
    float z = dot.cam.z();
    std::map<int, LinePhong>::iterator it;
    it = lines.find(y);
    if(it != lines.end()) {
        LinePhong line = it->second;
        if(x < line.x1) {
            line.x1 = x;
            line.z1 = z;
            line.absoluteBeg = dot.absolute;
            line.begNormal = dot.normal;
        } else if(x > line.x2) {
            line.x2 = x;
            line.z2 = z;
            line.absoluteEnd = dot.absolute;
            line.endNormal = dot.normal;
        }
        lines[y] = line;
    } else {
        LinePhong line(x, x, z, z, dot.absolute, dot.absolute);
        line.begNormal = dot.normal;
        line.endNormal = dot.normal;
        lines[y] = line;
    }
}

void AbstractGraphicalObject::addPixel(QVector3D point, std::map<int, Line>& lines) {
    Dot dot(QVector3D(0.f, 0.f, 0.f), point);
    addPixel(dot, lines);
}
