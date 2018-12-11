#ifndef CUBE_H
#define CUBE_H

#include <abstractgraphicalobject.h>

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
    void addBorderLine(Dot3D beg, Dot3D end, std::map<int, LineX> &lines);
    void shadeLambFong(std::map<int, LineX> &lines, QVector3D *absoluteFace);
    void shadeLine(Dot3D beg, Dot3D end, QVector3D normal);
    void addBorderPixel(Dot3D point, std::map<int, LineX> &lines);
    QList<Dot3D> getDotsForLine(Dot3D beg, Dot3D end);
};

#endif // CUBE_H