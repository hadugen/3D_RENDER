#ifndef ABSTRACTGRAPHICALOBJECT_H
#define ABSTRACTGRAPHICALOBJECT_H

#include <QVector>
#include <QImage>
#include <QPainter>
#include <matrix4x4.h>
#include <utils.h>

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

    void addBorderPixel(Dot3D point, std::map<int, LineX> &lines);
    void addBorderPixel(GuroDot point, std::map<int, LineGuroX> &lines);
    void addBorderPixel(QVector3D point, std::map<int, LineX> &lines);
    void addBorderPixel(FongDot point, std::map<int, LineFongX> &lines);
};

#endif // ABSTRACTGRAPHICALOBJECT_H
