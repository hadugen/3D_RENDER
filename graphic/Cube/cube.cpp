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
    addBorderLine(face[0], face[1], lines);
    addBorderLine(face[1], face[2], lines);
    addBorderLine(face[2], face[0], lines);
    shadeLambFong(lines, absoluteFace);
}

void Cube::addBorderLine(Dot3D beg, Dot3D end, std::map<int, LineX> &lines) {
    QList <Dot3D> dotList = getDotsForLine(beg, end);
    for(Dot3D dot : dotList) {
        addBorderPixel(Dot3D(dot.absolute, dot.cam), lines);
    }
    QVector3D lastCamPoint(int(end.cam.x() + 0.5), int(end.cam.y() + 0.5), end.cam.z());
    addBorderPixel(Dot3D(end.absolute, lastCamPoint), lines);
}

void Cube::shadeLambFong(std::map<int, LineX> &lines, QVector3D *absoluteFace) {

    QVector3D vec1(
            absoluteFace[1].x() - absoluteFace[0].x(),
            absoluteFace[1].y() - absoluteFace[0].y(),
            absoluteFace[1].z() - absoluteFace[0].z()
    );
    QVector3D vec2(
            absoluteFace[2].x() - absoluteFace[1].x(),
            absoluteFace[2].y() - absoluteFace[1].y(),
            absoluteFace[2].z() - absoluteFace[1].z()
    );
    QVector3D normal = QVector3D::normal(vec1, vec2);
    std::map<int, LineX>::iterator it;
    QList <Dot3D> dots3d;
    for(it = lines.begin(); it != lines.end(); it++) {
        LineX lin = it->second;
        Dot3D beg(lin.absoluteBeg, QVector3D(lin.x1, it->first, lin.z1));
        Dot3D end(lin.absoluteEnd, QVector3D(lin.x2, it->first, lin.z2));
        shadeLine(beg, end, normal);
    }
}

void Cube::shadeLine(Dot3D beg, Dot3D end, QVector3D normal) {
    QList <Dot3D> dotList = getDotsForLine(beg, end);
    QColor color(Qt::blue);
    for(Dot3D dot : dotList) {
        _image->setPixelColor(dot.cam.x(), dot.cam.y(), color);
    }
}

void Cube::addBorderPixel(Dot3D point, std::map<int, LineX>& lines) {
    int x = static_cast<int>(point.cam.x() + 0.5);
    int y = static_cast<int>(point.cam.y() + 0.5);
    float z = point.cam.z();
    std::map<int, LineX>::iterator it;
    it = lines.find(y);
    if(it != lines.end()) {
        LineX line = it->second;
        if(x < line.x1) {
            line.x1 = x;
            line.z1 = z;
            line.absoluteBeg = point.absolute;
        } else if( x > line.x2) {
            line.x2 = x;
            line.z2 = z;
            line.absoluteEnd = point.absolute;
        }
        lines[y] = line;
    } else {
        LineX line(x, x, z, z, point.absolute, point.absolute);
        lines[y] = line;
    }
}

QList <Dot3D> Cube::getDotsForLine(Dot3D beg, Dot3D end) {
    QList <Dot3D> result;
    bool steep = abs(end.cam.y() - beg.cam.y()) > abs(end.cam.x() - beg.cam.x());
    if (steep) {
        float temp = beg.cam.x();
        beg.cam.setX(beg.cam.y());
        beg.cam.setY(temp);

        temp = end.cam.x();
        end.cam.setX(end.cam.y());
        end.cam.setY(temp);
    }
    if (beg.cam.x() > end.cam.x()) {
        std::swap(beg, end);
    }
    float dx = end.cam.x() - beg.cam.x();
    float dy = std::abs(end.cam.y() - beg.cam.y());
    float error = dx / 2;
    int ystep = (beg.cam.y() < end.cam.y()) ? 1 : -1;
    int y = int(beg.cam.y() + 0.5);
    float z = beg.cam.z();
    float zstep = (end.cam.z() - beg.cam.z()) / (end.cam.x() - beg.cam.x());
    QVector3D delta = (end.absolute - beg.absolute) / (end.cam.x() - beg.cam.x());
    QVector3D worldPoint = beg.absolute;
    for (int x = int(beg.cam.x() + 0.5); x <= int(end.cam.x() + 0.5); x++, z += zstep, worldPoint += delta)
    {
        QVector3D camPoint(steep ? y : x, steep ? x : y, z);
        result.push_back(Dot3D(worldPoint ,camPoint));
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
    return result;
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
