#ifndef CUBE_H
#define CUBE_H

#include <abstractgraphicalobject.h>
#include <utils/utils.h>

class Cube : public AbstractGraphicalObject
{
public:
    Cube();
    void renderOnImage(Matrix4x4 viewProjection, QImage *image) override;

private:

    void addVerts() override;
    void addEdges() override;
    void addFaces() override;

    void drawEdges(QVector <QVector4D> dots);
    void drawFaces(QVector <QVector4D> dots);
    void drawFace(const QVector<QVector4D> &dots, QVector3D *absoluteFace, QVector3D *camRelativeFace);

};

#endif // CUBE_H
