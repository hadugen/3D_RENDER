#include "cube.h"

Cube::Cube() {
    addVerts();
    addEdges();
    addFaces();
}

void Cube::drawEdges(QVector <QVector4D> dots) {
    _painter.begin(_image);
    _painter.setPen(Qt::white);
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
    switch (Lamp::shadingType()) {
    case NO_SHADING:
        drawEdges(dots);
        break;
    case DEFAULT_SHADING:
        drawDefaultShadedFaces(dots);
        break;
    case GOURAUD_SHADING:
        drawGouraudShadedFaces(dots);
        break;
    case PHONG_SHADING:
        drawPhongShadedFaces(dots);
        break;
    }
}

void Cube::drawGouraudShadedFaces(QVector<QVector4D> dots) {
    QVector<QVector3D> normalsFace;
    QVector<QVector3D> normalsVerts(_verts.size(), QVector3D(0.f,0.f,0.f));
    QVector<QVector<QVector4D>> intensVerts;
    for (QVector3D face : _faces) {
        QVector3D absoluteFace[] = {
            _verts[face.x()],
            _verts[face.y()],
            _verts[face.x()]
        };
        QVector3D normal = QVector3D::normal(
                QVector3D(absoluteFace[1].x() - absoluteFace[0].x(), absoluteFace[1].y() - absoluteFace[0].y(), absoluteFace[1].z() - absoluteFace[0].z()),
                QVector3D(absoluteFace[2].x() - absoluteFace[1].x(), absoluteFace[2].y() - absoluteFace[1].y(), absoluteFace[2].z() - absoluteFace[1].z())
        );
        normalsVerts[face.x()] += normal;
        normalsVerts[face.y()] += normal;
        normalsVerts[face.z()] += normal;
        normalsFace.push_back(normal);
    }
    for (int i=0; i < _verts.size(); i++) {
        QVector3D worldPos = _verts[i];
        QVector3D normal = normalsVerts[i];
        QVector <QVector4D> intens = Lamp::getIntensVector(worldPos, normal);
        intensVerts.push_back(intens);
    }
    for (QVector3D face : _faces) {
        QVector3D worldFace[] = {_verts[face.x()], _verts[face.y()], _verts[face.z()]};
        QVector3D camFace[] = {dots[face.x()].toVector3D(), dots[face.y()].toVector3D(), dots[face.z()].toVector3D()};
        QVector <QVector4D> intens[] = {intensVerts[face.x()], intensVerts[face.y()], intensVerts[face.z()]};
        shadeGouraudFaces(worldFace, camFace, intens, _image);
    }
}

void Cube::shadeGouraudFaces(QVector3D * absoluteFace, QVector3D * camFace, QVector<QVector4D> *intens, QImage * image) {
    QVector2D vec1(camFace[1].x() - camFace[0].x(), camFace[1].y() - camFace[0].y());
    QVector2D vec2(camFace[2].x() - camFace[1].x(), camFace[2].y() - camFace[1].y());
    float z = vec1.x() * vec2.y() - vec2.x() * vec1.y();
    if(z > 0) {
        return;
    }

    std::map<int, LineGuroX> lines;
    GuroDot face[] = {GuroDot(absoluteFace[0], camFace[0], intens[0]), GuroDot(absoluteFace[1], camFace[1], intens[1]), GuroDot(absoluteFace[2], camFace[2], intens[2])};
    addBorderLine(face[0], face[1], lines);
    addBorderLine(face[1], face[2], lines);
    addBorderLine(face[2], face[0], lines);

    std::map<int, LineGuroX>::iterator it;;
    for(it = lines.begin(); it != lines.end(); it++) {
        LineGuroX lin = it->second;
        GuroDot beg(lin.absoluteBeg, QVector3D(lin.x1, it->first, lin.z1), lin.begIntens);
        GuroDot end(lin.absoluteEnd, QVector3D(lin.x2, it->first, lin.z2), lin.endIntens);
        shadeLine(beg, end, image);
    }
}


void Cube::drawPhongShadedFaces(QVector <QVector4D> dots) {
    QVector <QVector3D> normalsVerts(_verts.size(), QVector3D(0.0, 0.0, 0.0));
    for (QVector3D face : _faces) {
        QVector3D absoluteFace[] = {
            _verts[face.x()],
            _verts[face.y()],
            _verts[face.z()]
        };
        QVector3D normal = QVector3D::normal(
                QVector3D(absoluteFace[1].x() - absoluteFace[0].x(), absoluteFace[1].y() - absoluteFace[0].y(), absoluteFace[1].z() - absoluteFace[0].z()),
                QVector3D(absoluteFace[2].x() - absoluteFace[1].x(), absoluteFace[2].y() - absoluteFace[1].y(), absoluteFace[2].z() - absoluteFace[1].z())
        );
        normalsVerts[face.x()] += normal * 0.5f;
        normalsVerts[face.y()] += normal;
        normalsVerts[face.z()] += normal * 0.5f;
    }
    for (QVector3D face : _faces) {
        QVector3D worldFace[] = {_verts[face.x()], _verts[face.y()], _verts[face.z()]};
        QVector3D camFace[] = {dots[face.x()].toVector3D(), dots[face.y()].toVector3D(), dots[face.z()].toVector3D()};
        QVector3D normals[] = {normalsVerts[face.x()], normalsVerts[face.y()], normalsVerts[face.z()]};
        shadeFongFace(worldFace, camFace, normals, _image);
    }
}

void Cube::shadeFongFace(QVector3D* absoluteFace, QVector3D* camFace, QVector3D* normals, QImage * image) {
    QVector2D vec1(camFace[1].x() - camFace[0].x(), camFace[1].y() - camFace[0].y());
    QVector2D vec2(camFace[2].x() - camFace[1].x(), camFace[2].y() - camFace[1].y());
    float z = vec1.x() * vec2.y() - vec2.x() * vec1.y();
    if(z > 0) {
        return;
    }
    std::map<int, LineFongX> lines;
    FongDot face[] = {FongDot(absoluteFace[0], camFace[0], normals[0]), FongDot(absoluteFace[1], camFace[1], normals[1]), FongDot(absoluteFace[2], camFace[2], normals[2])};
    addBorderLine(face[0], face[1], lines);
    addBorderLine(face[1], face[2], lines);
    addBorderLine(face[2], face[0], lines);
    shadeFong(lines, absoluteFace, image);
}

void Cube::shadeFong(std::map<int, LineFongX> &lines, QVector3D *absoluteFace, QImage *image) {
    std::map<int, LineFongX>::iterator it;
    for(it = lines.begin(); it != lines.end(); it++) {
        LineFongX lin = it->second;
        FongDot beg(lin.absoluteBeg, QVector3D(lin.x1, it->first, lin.z1), lin.begNormal);
        FongDot end(lin.absoluteEnd, QVector3D(lin.x2, it->first, lin.z2), lin.endNormal);
        shadeLine(beg, end, image);
    }
}

void Cube::drawDefaultShadedFaces(QVector <QVector4D> dots) {
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
    defaultShade(lines, absoluteFace, _image);
}

void Cube::addBorderLine(Dot3D beg, Dot3D end, std::map<int, LineX> &lines) {
    QList <Dot3D> dotList = Utils::getDotsForLine(beg, end);
    for(Dot3D dot : dotList) {
        addBorderPixel(Dot3D(dot.absolute, dot.cam), lines);
    }
    QVector3D lastCamPoint(int(end.cam.x() + 0.5), int(end.cam.y() + 0.5), end.cam.z());
    addBorderPixel(Dot3D(end.absolute, lastCamPoint), lines);
}

void Cube::addBorderLine(GuroDot beg, GuroDot end, std::map<int, LineGuroX>& lines) {
    QList <GuroDot> dotList = Utils::getDotsForLine(beg, end);
    for(GuroDot dot : dotList) {
        addBorderPixel(dot, lines);
    }
    QVector3D lastCamPoint(int(end.cam.x() + 0.5), int(end.cam.y() + 0.5), end.cam.z());
    addBorderPixel(end, lines);
}

void Cube::addBorderLine(FongDot beg, FongDot end, std::map<int, LineFongX>& lines) {
    QList <FongDot> dotList = Utils::getDotsForLine(beg, end);
    for(FongDot dot : dotList) {
        addBorderPixel(dot, lines);
    }
    QVector3D lastCamPoint(int(end.cam.x() + 0.5), int(end.cam.y() + 0.5), end.cam.z());
    addBorderPixel(end, lines);
}

void Cube::defaultShade(std::map<int, LineX> &lines, QVector3D *absoluteFace, QImage *image) {
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
    for(it = lines.begin(); it != lines.end(); it++) {
        LineX lin = it->second;
        Dot3D beg(lin.absoluteBeg, QVector3D(lin.x1, it->first, lin.z1));
        Dot3D end(lin.absoluteEnd, QVector3D(lin.x2, it->first, lin.z2));
        shadeLine(beg, end, normal, image);
    }
}

void Cube::shadeLine(Dot3D beg, Dot3D end, QVector3D normal, QImage *image) {
    QList <Dot3D> dotList = Utils::getDotsForLine(beg, end);
    QColor color(Qt::white);
    for(Dot3D dot : dotList) {
        color = Lamp::calcSummaryLight(dot.absolute, normal);
        image->setPixelColor(dot.cam.x(), dot.cam.y(), color);
    }
}

void Cube::shadeLine(GuroDot beg, GuroDot end, QImage * image) {
    QList <GuroDot> dotList = Utils::getDotsForLine(beg, end);
    for(GuroDot dot : dotList) {
        QVector <QVector4D> intens = dot.intens;
        QVector4D colorVec(0.f, 0.f, 0.f, 1.f);
        for(int i=0; i < intens.size(); i++) {
            colorVec = colorVec + intens[i];
        }
        QColor color(std::min(colorVec.x(), 255.f), std::min(colorVec.y(), 255.f), std::min(colorVec.z(), 255.f));
        image->setPixelColor(dot.cam.x(), dot.cam.y(), color);
    }
}

void Cube::shadeLine(FongDot beg, FongDot end, QImage * image) {
    QList <FongDot> dotList = Utils::getDotsForLine(beg, end);
    for(FongDot dot : dotList) {
        QVector3D normal = dot.normal;
        QColor color = Lamp::calcSummaryLight(dot.absolute, normal);
        image->setPixelColor(dot.cam.x(), dot.cam.y(), color);
    }
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
