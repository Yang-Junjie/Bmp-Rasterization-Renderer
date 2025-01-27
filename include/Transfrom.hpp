#pragma once
#include "oe_math.hpp"

namespace Transform {

    // 2D变换函数
    void CoordChange(oeVec2& v, const real& width, const real& height) {
        v.y = -v.y + height / 2.0;
        v.x = v.x + width / 2.0;
    }

    void ApplyMatrixToVertices(oeVec2 vertex[], const size_t& vertices_num, const Matrix2x2& mat) {
        for (size_t i = 0; i < vertices_num; ++i) {
            vertex[i] = mat.multiply(vertex[i]);
        }
    }

    Matrix2x2 Scale(const real& factor1, const real &factor2) {
        return Matrix2x2(factor1, 0, 0, factor2);
    }

    Matrix2x2 Shear(const real& factor1, const real &factor2) {
        return Matrix2x2(1, factor1, factor2, 1);
    }

    Matrix2x2 Rotation(const real& angle) {
        const real radian = angle * Constant::Pi / 180;
        return Matrix2x2(std::cos(radian), std::sin(radian), -std::sin(radian), std::cos(radian));
    }

    // 3D变换函数
    void ApplyMatrixToVertices(oeVec3 vertex[], const size_t& vertices_num, const Matrix3x3& mat) {
        for (size_t i = 0; i < vertices_num; ++i) {
            vertex[i] = mat.multiply(vertex[i]);
        }
    }

    Matrix3x3 Scale(const real& factor1, const real& factor2, const real& factor3) {
        return Matrix3x3(factor1, 0, 0,
                         0, factor2, 0,
                         0, 0, factor3);
    }

    Matrix3x3 Shear(const real& factorXY, const real& factorXZ, const real& factorYX, const real& factorYZ, const real& factorZX, const real& factorZY) {
        return Matrix3x3(1, factorXY, factorXZ,
                         factorYX, 1, factorYZ,
                         factorZX, factorZY, 1);
    }

    Matrix3x3 Rotation(real angleX, real angleY, real angleZ) {
        const real radianX = angleX * Constant::Pi / 180;
        const real radianY = angleY * Constant::Pi / 180;
        const real radianZ = angleZ * Constant::Pi / 180;

        Matrix3x3 rotationX(1, 0, 0,
                            0, std::cos(radianX), std::sin(radianX),
                            0, -std::sin(radianX), std::cos(radianX));

        Matrix3x3 rotationY(std::cos(radianY), 0, -std::sin(radianY),
                            0, 1, 0,
                            std::sin(radianY), 0, std::cos(radianY));

        Matrix3x3 rotationZ(std::cos(radianZ), std::sin(radianZ), 0,
                            -std::sin(radianZ), std::cos(radianZ), 0,
                            0, 0, 1);

        return rotationX.multiply(rotationY).multiply(rotationZ);
    }

    // 4D变换函数
    void ApplyMatrixToVertices(oeVec4 vertex[], const size_t& vertices_num, const Matrix4x4& mat) {
        for (size_t i = 0; i < vertices_num; ++i) {
            vertex[i] = mat.multiply(vertex[i]);
        }
    }

    Matrix4x4 Scale4x4(const real& factor1, const real& factor2, const real& factor3) {
        return Matrix4x4(factor1, 0, 0, 0,
                         0, factor2, 0, 0,
                         0, 0, factor3, 0,
                         0, 0, 0, 1);
    }

    Matrix4x4 Translation(const real& dx, const real& dy, const real& dz) {
        return Matrix4x4(1, 0, 0, dx,
                         0, 1, 0, dy,
                         0, 0, 1, dz,
                         0, 0, 0, 1);
    }

    Matrix4x4 Rotation4x4(real angleX, real angleY, real angleZ) {
        Matrix3x3 rotMat = Rotation(angleX, angleY, angleZ);
        return Matrix4x4(rotMat.e11(), rotMat.e12(), rotMat.e13(), 0,
                         rotMat.e21(), rotMat.e22(), rotMat.e23(), 0,
                         rotMat.e31(), rotMat.e32(), rotMat.e33(), 0,
                         0, 0, 0, 1);
    }

    
    // Matrix4x4 ViewTransformation(const oeVec3& e, const oeVec3& g, const oeVec3& t) {
    //     g.normalize();
    //     t.normalize();
    //     oeVec3 s = g.cross(t);
    //     return Matrix4x4(s.x,t.x,-g.x,0,
    //                      s.x,t.x,-g.x,0,
    //                      s.x,t.x,-g.x,0,
    //                        0,  0,   0,1).multiply(Matrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,-e.x,-e.y,-e.z,1));
    // }

   Matrix4x4 PerspectiveProjection(real eye_fov, real aspect_ratio,
                                      real zNear, real zFar) {
    Matrix4x4 proj, ortho;
    Matrix4x4 projection = {1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,0,1};
    proj={zNear, 0, 0, 0,
            0, zNear, 0, 0,
            0, 0, zNear + zFar, -zNear * zFar,
            0, 0, 1, 0};

    double w, h, z;
    h = zNear * tan(eye_fov / 2) * 2;
    w = h * aspect_ratio;
    z = zFar - zNear;

    ortho={ 2 / w, 0, 0, 0,
             0, 2 / h, 0, 0,
             0, 0, 2 / z, -(zFar+zNear) / 2,
             0, 0, 0, 1};
             				
    projection = (ortho.transpose().multiply(proj.transpose())).multiply(projection);

    return projection;

    }
   
}