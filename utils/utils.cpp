#include "utils.h"

Utils::Utils() {

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

bool Utils::IsLit(QVector3D normal, QVector3D point0, QVector3D point) {
    float value = normal.x() * (point.x() - point0.x()) + normal.y() * (point.y() - point0.y()) + normal.z() * (point.z() - point0.z());
    if(value > 0) {
        return true;
    } else {
        return false;
    }
}
