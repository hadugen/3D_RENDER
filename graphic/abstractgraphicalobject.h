#ifndef ABSTRACTGRAPHICALOBJECT_H
#define ABSTRACTGRAPHICALOBJECT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

class AbstractGraphicalObject
{
public:

    AbstractGraphicalObject();
    virtual ~AbstractGraphicalObject();

    QVector <QVector3D> verts();
    QVector <QVector2D> edges();
    QVector <QVector3D> faces();

private:

    void addVerts();
    void addEdges();
    void addFaces();

protected:

    QVector <QVector3D> _verts;
    QVector <QVector2D> _edges;
    QVector <QVector3D> _faces;
};

#endif // ABSTRACTGRAPHICALOBJECT_H
