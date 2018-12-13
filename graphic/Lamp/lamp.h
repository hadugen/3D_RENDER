#ifndef LAMP_H
#define LAMP_H

#include <abstractgraphicalobject.h>

class Lamp : public AbstractGraphicalObject
{
public:

    Lamp(QVector3D position, double radius, QColor color);
    ~Lamp() override;
    static Lamp * findLampByCoords(double x, double y);
    void moveTo(int x, int y);
    QVector3D position();
    static QColor calcSummaryLight(QVector3D worldPos, QVector3D normal);
    QVector4D calcLightOnPoint(QVector3D worldPos, QVector3D normal);
    virtual void renderOnImage(Matrix4x4 viewProjection, QImage *image) override;

private:
    QVector3D _absolutePosition;
    QVector3D _screenPosition;
    QVector3D _newScreenPosition;

    bool _isMoved = false;
    double _radius;
    double _squaredRadius;
    QColor _color;
    static QVector <Lamp*> _lamps;

    QVector2D _intensity;
    QVector2D _attenuation;

    void addVerts() override;
    void addEdges() override;
    void addFaces() override;
    void bresCircle(QVector3D point, int x, int y, std::map<int, LineX> &lines);
    void addBorderCircle(QVector3D center, int radius, std::map<int, LineX> &lines);
};

#endif // LAMP_H
