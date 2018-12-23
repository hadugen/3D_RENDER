#include "utils.h"
#include <QDebug>
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
    QVector3D absolutePoint = beg.absolute;
    for (int x = int(beg.cam.x() + 0.5); x <= int(end.cam.x() + 0.5); x++, z += zstep, absolutePoint += delta)
    {
        QVector3D camPoint(steep ? y : x, steep ? x : y, z);
        result.push_back(Dot3D(absolutePoint ,camPoint));
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
    return result;
}

QList <GuroDot> Utils::getDotsForLine(GuroDot beg, GuroDot end) {
    QList <GuroDot> result;
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
    float fdx = (end.cam.x() - beg.cam.x());
    int dx = static_cast<int>(fdx);
    int dy = abs(end.cam.y() - beg.cam.y());
    int error = dx / 2;
    int ystep = (beg.cam.y() < end.cam.y()) ? 1 : -1;
    int y = int(beg.cam.y() + 0.5);
    float z = beg.cam.z();
    float zstep = (end.cam.z() - beg.cam.z()) / (end.cam.x() - beg.cam.x());
    QVector3D delta = (end.absolute - beg.absolute) / fdx;
    QVector3D absolutePoint = beg.absolute;
    QVector <QVector4D> curIntens = beg.intens;
    QVector <QVector4D> deltaIntens = end.intens;
    for(int i = 0; i < beg.intens.size(); i++) {
        QVector4D dlt = deltaIntens[i];
        QVector4D begInt = beg.intens[i];
        deltaIntens[i] = (dlt - begInt);
        deltaIntens[i] = deltaIntens[i] / fdx;
    }
    for (int x = int(beg.cam.x() + 0.5); x <= int(end.cam.x() + 0.5); x++, z += zstep, absolutePoint += delta) {
        QVector3D camPoint(steep ? y : x, steep ? x : y, z);
        result.push_back(GuroDot(absolutePoint ,camPoint, curIntens));
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
        for(int i=0; i < beg.intens.size(); i++) {
            curIntens[i] = curIntens[i] + deltaIntens[i];
        }
    }
    return result;
}

QList <FongDot> Utils::getDotsForLine(FongDot beg, FongDot end) {
    QList <FongDot> result;
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
    int dx = end.cam.x() - beg.cam.x();
    int dy = abs(end.cam.y() - beg.cam.y());
    int error = dx / 2;
    int ystep = (beg.cam.y() < end.cam.y()) ? 1 : -1;
    int y = int(beg.cam.y() + 0.5);
    float z = beg.cam.z();
    float zstep = (end.cam.z() - beg.cam.z()) / (end.cam.x() - beg.cam.x());
    QVector3D delta = (end.absolute - beg.absolute) / (end.cam.x() - beg.cam.x());
    QVector3D absolutePoint = beg.absolute;
    QVector3D deltaNormal = (end.normal - beg.normal) / (end.cam.x() - beg.cam.x());
    QVector3D curNormal = beg.normal;
    for (int x = int(beg.cam.x() + 0.5); x <= int(end.cam.x() + 0.5); x++, z += zstep, absolutePoint += delta)
    {
        QVector3D camPoint(steep ? y : x, steep ? x : y, z);
        result.push_back(FongDot(absolutePoint, camPoint, curNormal));
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
        curNormal += deltaNormal;
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
QVector3D Utils::reflect(QVector3D incident, QVector3D normal) {
    return incident - (normal * 2.f * QVector3D::dotProduct(normal, incident));
}
