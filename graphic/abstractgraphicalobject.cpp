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
