#ifndef LAMP_H
#define LAMP_H

#include <abstractgraphicalobject.h>
#include <utils/utils.h>
#include <Cube/cube.h>

class Lamp : public AbstractGraphicalObject
{
public:
    Lamp(QVector3D position, double zoom, QColor color);
    ~Lamp() override;
    static QVector <Lamp*> lamps;

    void moveTo(int x, int y);
    virtual void renderOnImage(Matrix4x4 viewProjection, QImage *image) override;

    bool itsMe(int x, int y);

    QVector3D position();

    static QColor calcSummaryLight(QVector3D worldPos, QVector3D normal);
    QVector4D calcLightOnPoint(QVector3D worldPos, QVector3D normal);

private:
    QVector3D _absolutePosition;
    QVector3D _screenPosition;
    QVector3D _newScreenPosition;

    bool _isMoved = false;
    double _radius;
    QColor _color;

    QVector2D _intensity;
    QVector2D _attenuation;

    void addVerts() override;
    void addEdges() override;
    void addFaces() override;
};

#endif // LAMP_H
