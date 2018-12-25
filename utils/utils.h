#ifndef UTILS_H
#define UTILS_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QColor>
#include <QImage>
#include <QList>

#include "matrix4x4.h"

struct Line {
    Line(){}
    Line(int x1, int x2, float z1, float z2, QVector3D absoluteBeg, QVector3D absoluteEnd) {
        this->x1 = x1;
        this->x2 = x2;
        this->z1 = z1;
        this->z2 = z2;
        this->absoluteBeg = absoluteBeg;
        this->absoluteEnd = absoluteEnd;
    }
    int x1, x2;
    float z1, z2;
    QVector3D absoluteBeg;
    QVector3D absoluteEnd;
};


struct LineGouraud {
    LineGouraud() {}
    LineGouraud(int x1, int x2, float z1, float z2, QVector3D absoluteBeg, QVector3D absoluteEnd) {
        this->x1 = x1;
        this->x2 = x2;
        this->z1 = z1;
        this->z2 = z2;
        this->absoluteBeg = absoluteBeg;
        this->absoluteEnd = absoluteEnd;
    }
    int x1, x2;
    float z1, z2;
    QVector3D absoluteBeg;
    QVector3D absoluteEnd;
    QVector <QVector4D> begIntens;
    QVector <QVector4D> endIntens;
};

struct LinePhong {
    LinePhong() {}
    LinePhong( int x1, int x2, float z1, float z2, QVector3D absoluteBeg, QVector3D absoluteEnd) {
        this->x1 = x1;
        this->x2 = x2;
        this->z1 = z1;
        this->z2 = z2;
        this->absoluteBeg = absoluteBeg;
        this->absoluteEnd = absoluteEnd;
    }
    int x1, x2;
    float z1, z2;
    QVector3D absoluteBeg;
    QVector3D absoluteEnd;
    QVector3D begNormal;
    QVector3D endNormal;
};

struct Dot {
    Dot(QVector3D absolute, QVector3D cam) {
        this->absolute = absolute;
        this->cam = cam;
    }
    QVector3D absolute;
    QVector3D cam;
};

struct GouraudDot {
    GouraudDot(QVector3D absolute, QVector3D cam, QVector<QVector4D> intens) {
        this->absolute = absolute;
        this->cam = cam;
        this->intens = intens;
    }
    QVector3D absolute;
    QVector3D cam;
    QVector <QVector4D> intens;
};

struct PhongDot {
    PhongDot(QVector3D absolute, QVector3D cam, QVector3D normal) {
        this->absolute = absolute;
        this->cam = cam;
        this->normal = normal;
    }
    QVector3D absolute;
    QVector3D cam;
    QVector3D normal;
};

class Utils
{
public:
    static QList<Dot> getDotsForLine(Dot beg, Dot end);
    static QList<GouraudDot> getDotsForLine(GouraudDot beg, GouraudDot end);
    static QList<PhongDot> getDotsForLine(PhongDot beg, PhongDot end);
    static QVector3D worldToScreen(QVector3D point, Matrix4x4 vp, QSize size);
    static QVector3D screenToWorld(QVector3D point, Matrix4x4 vp, QSize size);
    static void bresCircle(QVector3D point, int x, int y, std::map<int, Line> &lines);
    static void addBorderCircle(QVector3D center, int radius, std::map<int, Line> &lines);
    static bool IsLit(QVector3D normal, QVector3D point0, QVector3D point);
    static QVector3D reflect(QVector3D incident, QVector3D normal);
private:
    Utils();
};

#endif // UTILS_H
