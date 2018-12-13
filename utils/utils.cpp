#include "utils.h"

Utils::Utils() {

}

void Utils::addBorderLine(Dot3D beg, Dot3D end, std::map<int, LineX> &lines) {
    QList <Dot3D> dotList = getDotsForLine(beg, end);
    for(Dot3D dot : dotList) {
        addBorderPixel(Dot3D(dot.absolute, dot.cam), lines);
    }
    QVector3D lastCamPoint(int(end.cam.x() + 0.5), int(end.cam.y() + 0.5), end.cam.z());
    addBorderPixel(Dot3D(end.absolute, lastCamPoint), lines);
}

void Utils::shadeLambFong(std::map<int, LineX> &lines, QVector3D *absoluteFace, QImage *image) {

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

void Utils::shadeLine(Dot3D beg, Dot3D end, QVector3D normal, QImage *image) {
    QList <Dot3D> dotList = getDotsForLine(beg, end);
    QColor color(Qt::white);
    for(Dot3D dot : dotList) {
        image->setPixelColor(dot.cam.x(), dot.cam.y(), color);
    }
}

void Utils::addBorderPixel(Dot3D point, std::map<int, LineX>& lines) {
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

QList <Dot3D> Utils::getDotsForLine(Dot3D beg, Dot3D end) {
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

QVector3D Utils::worldToScreen(QVector3D point, Matrix4x4 vp, QSize size) {
    QVector4D pos(point, 1);
    pos = vp * pos;
    float z = pos.z();
    pos = pos / pos.w();
    pos.setX((((pos.x() + 1.0f) * 0.5f) * size.width()));
    pos.setY((((pos.y() + 1.0f) * 0.5f) * size.height()));
    pos.setZ(z);
    return pos.toVector3D();
}

QVector3D Utils::screenToWorld(QVector3D point, Matrix4x4 vp, QSize size) {
    QVector4D pos(point, 1);
    vp.inverse();
    pos.setX(((pos.x() / ((float) size.width())) * 2.0f) - 1.0f);
    pos.setY((((pos.y() / ((float) size.height())) * 2.0f) - 1.0f));
    pos = vp * pos;
    pos = pos / pos.w();
    return pos.toVector3D();
}

void Utils::addBorderPixel(QVector3D point, std::map<int, LineX>& lines) {
    Dot3D dot(QVector3D(0.f, 0.f, 0.f), point);
    addBorderPixel(dot, lines);
}

void Utils::bresCircle(QVector3D point, int x, int y, std::map<int, LineX>& lines)
{
    int xc = static_cast<int>(point.x());
    int yc = static_cast<int>(point.y());
    float zc = point.z();
    addBorderPixel(QVector3D(xc + x, yc + y, zc), lines);
    addBorderPixel(QVector3D(xc - x, yc + y, zc), lines);
    addBorderPixel(QVector3D(xc + x, yc - y, zc), lines);
    addBorderPixel(QVector3D(xc - x, yc - y, zc), lines);
    addBorderPixel(QVector3D(xc + y, yc + x, zc), lines);
    addBorderPixel(QVector3D(xc - y, yc + x, zc), lines);
    addBorderPixel(QVector3D(xc + y, yc - x, zc), lines);
    addBorderPixel(QVector3D(xc - y, yc - x, zc), lines);
}

void Utils::addBorderCircle(QVector3D center, int radius, std::map<int, LineX>& lines)
{
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    while (y >= x) {
        bresCircle(center, x, y, lines);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
    }
}
