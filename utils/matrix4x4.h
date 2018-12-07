#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <abstractgraphicalobject.h>
#include <QDebug>

class Matrix4x4
{
public:
    Matrix4x4();
    ~Matrix4x4();
    Matrix4x4 & toIdentityMatrix();
    Matrix4x4 & lookAt(QVector3D cameraPosition, QVector3D target, QVector3D cameraUp);
    Matrix4x4 & toProjectionMatrix(double d, double aspectRatio);
    Matrix4x4 & toViewPortMatrix(int x, int y, int w, int h);

    bool operator==(const Matrix4x4 & second);
    Matrix4x4 & operator= (const Matrix4x4 & second);
    Matrix4x4 operator* (const Matrix4x4 & second);
    QVector4D operator* (const QVector4D & second);

    QString toString();

private:
    double _matrix[16];

};

#endif // MATRIX4X4_H
