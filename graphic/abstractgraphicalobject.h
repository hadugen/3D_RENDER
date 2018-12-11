#ifndef ABSTRACTGRAPHICALOBJECT_H
#define ABSTRACTGRAPHICALOBJECT_H

#include <QVector>
#include <QImage>
#include <QPainter>
#include <matrix4x4.h>

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

class AbstractGraphicalObject
{
public:

    AbstractGraphicalObject();
    virtual ~AbstractGraphicalObject();

    QVector <QVector3D> verts() const;
    QVector <QVector2D> edges() const;
    QVector <QVector3D> faces() const;

    virtual void renderOnImage(Matrix4x4 viewProjection, QImage *image) = 0;

private:

protected:

    QPainter _painter;

    QImage *_image;

    QVector <QVector3D> _verts;
    QVector <QVector2D> _edges;
    QVector <QVector3D> _faces;

    virtual void addVerts() = 0;
    virtual void addEdges() = 0;
    virtual void addFaces() = 0;
};

#endif // ABSTRACTGRAPHICALOBJECT_H
