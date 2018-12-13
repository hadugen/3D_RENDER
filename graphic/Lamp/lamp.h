#ifndef LAMP_H
#define LAMP_H

#include <abstractgraphicalobject.h>
#include <utils/utils.h>
#include <Cube/cube.h>

class Lamp : public AbstractGraphicalObject
{
public:
    Lamp(QVector3D position, double zoom, QColor color);

    void moveTo(int x, int y);
    virtual void renderOnImage(Matrix4x4 viewProjection, QImage *image) override;

    bool itsMe(int x, int y);

private:
    QVector3D _absolutePosition;
    QVector3D _screenPosition;
    QVector3D _newScreenPosition;

    bool _isMoved = false;
    double _radius;
    QColor _color;

    void addVerts() override;
    void addEdges() override;
    void addFaces() override;
};

#endif // LAMP_H
