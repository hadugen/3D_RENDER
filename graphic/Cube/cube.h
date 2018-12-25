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
    void shadePhongFace(QVector3D *absoluteFace, QVector3D *camFace, QVector3D *normals, QImage *image);
    void shadePhong(std::map<int, LinePhong>& lines, QVector3D* absoluteFace, QImage * image);
    void addLine(Dot beg, Dot end, std::map<int, Line> &lines);
    void defaultShade(std::map<int, Line> &lines, QVector3D *absoluteFace, QImage *image);
    void shadeLine(Dot beg, Dot end, QVector3D normal, QImage *image);
    void addLine(GouraudDot beg, GouraudDot end, std::map<int, LineGouraud> &lines);
    void addLine(PhongDot beg, PhongDot end, std::map<int, LinePhong> &lines);
    void shadeLine(GouraudDot beg, GouraudDot end, QImage *image);
    void shadeLine(PhongDot beg, PhongDot end, QImage *image);
    void shadeGouraudFaces(QVector3D *absoluteFace, QVector3D *camFace, QVector<QVector4D> *intens, QImage *image);
};

#endif // CUBE_H
