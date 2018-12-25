#include "matrix4x4.h"

Matrix4x4::Matrix4x4() {
    memset(_matrix, 0.0f, 16 * sizeof(double));
}

Matrix4x4::~Matrix4x4() {

}

Matrix4x4 & Matrix4x4::toIdentityMatrix() {
    memset(_matrix, 0.0f, 16 * sizeof(double));
    _matrix[0] = 1.0;
    _matrix[5] = 1.0;
    _matrix[10] = 1.0;
    _matrix[15] = 1.0;
    return *this;
}

Matrix4x4 & Matrix4x4::toViewPortMatrix(int x, int y, int w, int h) {
    toIdentityMatrix();
    _matrix[3] = x + w / 2.0;
    _matrix[7] = y + h / 2.0;
    _matrix[11] = 1.0;
    _matrix[0] = w / 2.0;
    _matrix[5] = h / 2.0;
    _matrix[10] = 0;
    return *this;
}

Matrix4x4 & Matrix4x4::toProjectionMatrix(double z, double aspectRatio){
    toIdentityMatrix();
    double scaleCoef = 1.0 / z;
    _matrix[0] = scaleCoef;
    _matrix[5] = aspectRatio * scaleCoef;
    _matrix[14] = -scaleCoef;
    return *this;
}

Matrix4x4 & Matrix4x4::lookAt(QVector3D cameraPosition, QVector3D target, QVector3D cameraUp) {
    QVector3D zAxis = (cameraPosition - target).normalized();
    QVector3D xAxis = QVector3D::crossProduct(cameraUp, zAxis).normalized();
    QVector3D yAxis = QVector3D::crossProduct(zAxis, xAxis).normalized();
    _matrix[0] = xAxis.x();
    _matrix[1] = xAxis.y();
    _matrix[2] = xAxis.z();
    _matrix[3] = -QVector3D::dotProduct(xAxis, target);
    _matrix[4] = yAxis.x();
    _matrix[5] = yAxis.y();
    _matrix[6] = yAxis.z();
    _matrix[7] = -QVector3D::dotProduct(yAxis, target);
    _matrix[8] = -zAxis.x();
    _matrix[9] = -zAxis.y();
    _matrix[10] = -zAxis.z();
    _matrix[11] = -QVector3D::dotProduct(zAxis, target);
    _matrix[12] = 0;
    _matrix[13] = 0;
    _matrix[14] = 0;
    _matrix[15] = 1;
    return *this;
}

bool Matrix4x4::operator==(const Matrix4x4 & second) {
    for(int i = 0; i < 16; i++) {
        if(this->_matrix[i] != second._matrix[i]) {
            return false;
        }
    }
    return true;
}

Matrix4x4 & Matrix4x4::operator=(const Matrix4x4 & second) {
    if(this == &second || *this == second) {
        return *this;
    }
    memcpy(this->_matrix, second._matrix, 16 * sizeof(double));
    return *this;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & second) {
    Matrix4x4 resultMatrix;
    for(int i = 0; i < 16; i++){
        resultMatrix._matrix[i] = 0;
        int x = i % 4,
            y = i / 4;
        for(int j = 0; j < 4; j++){
            resultMatrix._matrix[i] += this->_matrix[y * 4 + j] * second._matrix[j * 4 + x];
        }
    }
    return resultMatrix;
}

QVector4D Matrix4x4::operator*(const QVector4D &vec) {
    return QVector4D(
            this->_matrix[0]  * vec.x() + this->_matrix[1]  * vec.y() + this->_matrix[2]  * vec.z() + this->_matrix[3]  * vec.w(),
            this->_matrix[4]  * vec.x() + this->_matrix[5]  * vec.y() + this->_matrix[6]  * vec.z() + this->_matrix[7]  * vec.w(),
            this->_matrix[8]  * vec.x() + this->_matrix[9]  * vec.y() + this->_matrix[10] * vec.z() + this->_matrix[11] * vec.w(),
            this->_matrix[12] * vec.x() + this->_matrix[13] * vec.y() + this->_matrix[14] * vec.z() + this->_matrix[15] * vec.w()
    );
}

Matrix4x4 & Matrix4x4::inverse() {
    Matrix4x4 AT;
    double * inv = AT._matrix, *m = _matrix;
    double determinant;

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
              m[4]  * m[11] * m[14] +
              m[8]  * m[6]  * m[15] -
              m[8]  * m[7]  * m[14] -
              m[12] * m[6]  * m[11] +
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] -
               m[8]  * m[6] * m[13] -
               m[12] * m[5] * m[10] +
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
              m[1]  * m[11] * m[14] +
              m[9]  * m[2] * m[15] -
              m[9]  * m[3] * m[14] -
              m[13] * m[2] * m[11] +
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
              m[0]  * m[11] * m[13] +
              m[8]  * m[1] * m[15] -
              m[8]  * m[3] * m[13] -
              m[12] * m[1] * m[11] +
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
              m[0]  * m[7] * m[14] +
              m[4]  * m[2] * m[15] -
              m[4]  * m[3] * m[14] -
              m[12] * m[2] * m[7] +
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
               m[0]  * m[6] * m[13] +
               m[4]  * m[1] * m[14] -
               m[4]  * m[2] * m[13] -
               m[12] * m[1] * m[6] +
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
              m[1] * m[7] * m[10] +
              m[5] * m[2] * m[11] -
              m[5] * m[3] * m[10] -
              m[9] * m[2] * m[7] +
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
               m[0] * m[7] * m[9] +
               m[4] * m[1] * m[11] -
               m[4] * m[3] * m[9] -
               m[8] * m[1] * m[7] +
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    determinant = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (determinant != 0) {
        determinant = 1.0 / determinant;
        for (int i = 0; i < 16; i++) {
            inv[i] = inv[i] * determinant;
        }
        *this = AT;
    }
    return *this;
}
