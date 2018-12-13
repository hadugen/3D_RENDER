#include "cube.h"

Cube::Cube() {
    addVerts();
    addEdges();
    addFaces();
}

void Cube::renderOnImage(Matrix4x4 viewProjection, QImage *image) {
    this->_image = image;

    QVector <QVector4D> dots;
    Matrix4x4 matViewPort;
    matViewPort.toViewPortMatrix(
                0,
                0,
                image->size().height(),
                image->size().width()
    );

    for(QVector3D vert : _verts) {
        QVector4D vec4(vert, 1);
        QVector4D dot = viewProjection * vec4;
        double z = dot.z();
        dot = matViewPort * dot;
        dot = dot / dot.w();
        dot.setZ(z);
        dots.push_back(dot);
    }
    //drawEdges(dots);
    drawFaces(dots);
}

void Cube::drawEdges(QVector <QVector4D> dots) {
    _painter.begin(_image);
    for(QVector2D edge : _edges) {
        _painter.drawLine(
                dots[edge.x()].x(),
                dots[edge.x()].y(),
                dots[edge.y()].x(),
                dots[edge.y()].y()
        );
    }
    _painter.end();
}

void Cube::drawFaces(QVector <QVector4D> dots) {
    for(QVector3D face : _faces) {
        QVector3D absoluteFace[] = {
            _verts[face.x()],
            _verts[face.y()],
            _verts[face.z()]
        };
        QVector3D camRelativeFace[] = {
            dots[face.x()].toVector3D(),
            dots[face.y()].toVector3D(),
            dots[face.z()].toVector3D()
        };
        drawFace(dots, absoluteFace, camRelativeFace);
    }
}

void Cube::drawFace(const QVector<QVector4D> &dots, QVector3D *absoluteFace, QVector3D *camRelativeFace) {
    QVector2D vec1(camRelativeFace[1].x() - camRelativeFace[0].x(), camRelativeFace[1].y() - camRelativeFace[0].y());
    QVector2D vec2(camRelativeFace[2].x() - camRelativeFace[1].x(), camRelativeFace[2].y() - camRelativeFace[1].y());
    float z = vec1.x() * vec2.y() - vec2.x() * vec1.y();
    if(z > 0) {
        return;
    }

    std::map<int, LineX> lines;
    Dot3D face[] = {
        Dot3D(absoluteFace[0], camRelativeFace[0]),
        Dot3D(absoluteFace[1], camRelativeFace[1]),
        Dot3D(absoluteFace[2], camRelativeFace[2])
    };
    Utils::addBorderLine(face[0], face[1], lines);
    Utils::addBorderLine(face[1], face[2], lines);
    Utils::addBorderLine(face[2], face[0], lines);
    Utils::shadeLambFong(lines, absoluteFace, _image);
}



void Cube::addEdges() {
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
}

void Cube::addFaces() {
    _faces.push_back(QVector3D(2, 1, 0));
    _faces.push_back(QVector3D(0, 3, 2));
    _faces.push_back(QVector3D(4, 5, 6));
    _faces.push_back(QVector3D(6, 7, 4));
    _faces.push_back(QVector3D(4, 0, 1));
    _faces.push_back(QVector3D(1, 5, 4));
    _faces.push_back(QVector3D(5, 1, 2));
    _faces.push_back(QVector3D(2, 6, 5));
    _faces.push_back(QVector3D(6, 2, 3));
    _faces.push_back(QVector3D(3, 7, 6));
    _faces.push_back(QVector3D(3, 0, 4));
    _faces.push_back(QVector3D(4, 7, 3));
}

void Cube::addVerts() {
    _verts.push_back(QVector3D(-0.5f, -0.5f, -0.5f));
    _verts.push_back(QVector3D(-0.5f, 0.5f, -0.5f));
    _verts.push_back(QVector3D(-0.5f, 0.5f, 0.5f));
    _verts.push_back(QVector3D(-0.5f, -0.5f, 0.5f));
    _verts.push_back(QVector3D(0.5f, -0.5f, -0.5f));
    _verts.push_back(QVector3D(0.5f, 0.5f, -0.5f));
    _verts.push_back(QVector3D(0.5f, 0.5f, 0.5f));
    _verts.push_back(QVector3D(0.5f, -0.5f, 0.5f));
}
