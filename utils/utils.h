#ifndef UTILS_H
#define UTILS_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QColor>
#include <QImage>
#include <QList>

#include "matrix4x4.h"

struct LineX {
    LineX(){}
    LineX(int x1, int x2, float z1, float z2, QVector3D absoluteBeg, QVector3D absoluteEnd) {
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

struct Dot3D {
    Dot3D(QVector3D absolute, QVector3D cam) {
        this->absolute = absolute;
        this->cam = cam;
    }
    QVector3D absolute;
    QVector3D cam;
};

class Utils
{
public:
    Utils();

    static void addBorderLine(Dot3D beg, Dot3D end, std::map<int, LineX> &lines);
    static void shadeLambFong(std::map<int, LineX> &lines, QVector3D *absoluteFace, QImage *image);
    static void shadeLine(Dot3D beg, Dot3D end, QVector3D normal, QImage *image);
    static void addBorderPixel(Dot3D point, std::map<int, LineX> &lines);
    static QList<Dot3D> getDotsForLine(Dot3D beg, Dot3D end);
    static QVector3D worldToScreen(QVector3D point, Matrix4x4 vp, QSize size);
    static QVector3D screenToWorld(QVector3D point, Matrix4x4 vp, QSize size);
    static void addBorderPixel(QVector3D point, std::map<int, LineX> &lines);
    static void bresCircle(QVector3D point, int x, int y, std::map<int, LineX> &lines);
    static void addBorderCircle(QVector3D center, int radius, std::map<int, LineX> &lines);
};

#endif // UTILS_H
