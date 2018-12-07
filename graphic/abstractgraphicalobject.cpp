#include "abstractgraphicalobject.h"

AbstractGraphicalObject::AbstractGraphicalObject()
{
    addVerts();
    addEdges();
    addFaces();
}

AbstractGraphicalObject::~AbstractGraphicalObject() {

}

QVector<QVector3D> AbstractGraphicalObject::verts() {
    return _verts;
}

QVector<QVector2D> AbstractGraphicalObject::edges()
{
    return _edges;
}

QVector<QVector3D> AbstractGraphicalObject::faces()
{
    return _faces;
}

void AbstractGraphicalObject::addVerts() {

    _verts.push_back(QVector3D(-1.0f, -0.5f, -0.5f));
    _verts.push_back(QVector3D(-1.0f, 0.5f, -0.5f));
    _verts.push_back(QVector3D(-1.0f, 0.5f, 0.5f));
    _verts.push_back(QVector3D(-1.0f, -0.5f, 0.5f));
    _verts.push_back(QVector3D(1.0f, -0.5f, -0.5f));
    _verts.push_back(QVector3D(1.0f, 0.5f, -0.5f));
    _verts.push_back(QVector3D(1.0f, 0.5f, 0.5f));
    _verts.push_back(QVector3D(1.0f, -0.5f, 0.5f));

    /*_verts.push_back(QVector3D(0, 0, 1.17557));
    _verts.push_back(QVector3D(1.05146, 0, 0.525731));
    _verts.push_back(QVector3D(0.32492, 1., 0.525731));
    _verts.push_back(QVector3D(-0.850651, 0.618034, 0.525731));
    _verts.push_back(QVector3D(-0.850651, -0.618034, 0.525731));
    _verts.push_back(QVector3D(0.32492, -1., 0.525731));
    _verts.push_back(QVector3D(0.850651, 0.618034, -0.525731));
    _verts.push_back(QVector3D(0.850651, -0.618034, -0.525731));
    _verts.push_back(QVector3D(-0.32492, 1., -0.525731));
    _verts.push_back(QVector3D(-1.05146, 0, -0.525731));
    _verts.push_back(QVector3D(-0.32492, -1., -0.525731));
    _verts.push_back(QVector3D(0, 0, -1.17557));*/

}

void AbstractGraphicalObject::addEdges() {

    _edges.push_back(QVector2D(0, 1));
    _edges.push_back(QVector2D(1, 2));
    _edges.push_back(QVector2D(2, 3));
    _edges.push_back(QVector2D(0, 3));
    _edges.push_back(QVector2D(4, 5));
    _edges.push_back(QVector2D(5, 6));
    _edges.push_back(QVector2D(6, 7));
    _edges.push_back(QVector2D(4, 7));
    _edges.push_back(QVector2D(0, 4));
    _edges.push_back(QVector2D(1, 5));
    _edges.push_back(QVector2D(2, 6));
    _edges.push_back(QVector2D(3, 7));

    /*_edges.push_back(QVector2D(0,1));
    _edges.push_back(QVector2D(0,2));
    _edges.push_back(QVector2D(0,3));
    _edges.push_back(QVector2D(0,4));
    _edges.push_back(QVector2D(0,5));
    _edges.push_back(QVector2D(1,2));
    _edges.push_back(QVector2D(1,5));
    _edges.push_back(QVector2D(1,6));
    _edges.push_back(QVector2D(1,7));
    _edges.push_back(QVector2D(2,3));
    _edges.push_back(QVector2D(2,6));
    _edges.push_back(QVector2D(2,8));
    _edges.push_back(QVector2D(3,4));
    _edges.push_back(QVector2D(3,8));
    _edges.push_back(QVector2D(3,9));
    _edges.push_back(QVector2D(4,5));
    _edges.push_back(QVector2D(4,9));
    _edges.push_back(QVector2D(4,10));
    _edges.push_back(QVector2D(5,7));
    _edges.push_back(QVector2D(5,10));
    _edges.push_back(QVector2D(6,7));
    _edges.push_back(QVector2D(6,8));
    _edges.push_back(QVector2D(6,11));
    _edges.push_back(QVector2D(7,10));
    _edges.push_back(QVector2D(7,11));
    _edges.push_back(QVector2D(8,9));
    _edges.push_back(QVector2D(8,11));
    _edges.push_back(QVector2D(9,10));
    _edges.push_back(QVector2D(9,11));
    _edges.push_back(QVector2D(10,11));*/
}

void AbstractGraphicalObject::addFaces() {

}
