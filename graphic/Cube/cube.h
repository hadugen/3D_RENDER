#ifndef CUBE_H
#define CUBE_H

#include <abstractgraphicalobject.h>
#include <Lamp/lamp.h>

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
    void drawDefaultShadedFaces(QVector <QVector4D> dots);
    void drawFace(const QVector<QVector4D> &dots, QVector3D *absoluteFace, QVector3D *camRelativeFace);
    void drawGouraudShadedFaces(QVector <QVector4D> dots);
    void drawPhongShadedFaces(QVector <QVector4D> dots);
    void shadeFongFace(QVector3D *absoluteFace, QVector3D *camFace, QVector3D *normals, QImage *image);
    void shadeFong(std::map<int, LineFongX>& lines, QVector3D* absoluteFace, QImage * image);
    void addBorderLine(Dot3D beg, Dot3D end, std::map<int, LineX> &lines);
    void defaultShade(std::map<int, LineX> &lines, QVector3D *absoluteFace, QImage *image);
    void shadeLine(Dot3D beg, Dot3D end, QVector3D normal, QImage *image);
    void addBorderLine(GuroDot beg, GuroDot end, std::map<int, LineGuroX> &lines);
    void addBorderLine(FongDot beg, FongDot end, std::map<int, LineFongX> &lines);
    void shadeLine(GuroDot beg, GuroDot end, QImage *image);
    void shadeLine(FongDot beg, FongDot end, QImage *image);
    void shadeGouraudFaces(QVector3D *absoluteFace, QVector3D *camFace, QVector<QVector4D> *intens, QImage *image);
};

#endif // CUBE_H
