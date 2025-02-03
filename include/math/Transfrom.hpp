#pragma once
#include "oe_math.hpp"

namespace Transform
{

    void ApplyMatrixToVertices(oeVec2 vertex[], const size_t &vertices_num, const Matrix2x2 &mat)
    {
        for (size_t i = 0; i < vertices_num; ++i)
        {
            vertex[i] = mat.multiply(vertex[i]);
        }
    }

    Matrix2x2 Scale(const real &factor1, const real &factor2)
    {
        return Matrix2x2(factor1, 0, 0, factor2);
    }

    Matrix2x2 Shear(const real &factor1, const real &factor2)
    {
        return Matrix2x2(1, factor1, factor2, 1);
    }

    Matrix2x2 Rotation(const real &angle)
    {
        const real radian = angle * Constant::Pi / 180;
        return Matrix2x2(std::cos(radian), std::sin(radian), -std::sin(radian), std::cos(radian));
    }

    // 3D变换函数
    void ApplyMatrixToVertices(oeVec3 vertex[], const size_t &vertices_num, const Matrix3x3 &mat)
    {
        for (size_t i = 0; i < vertices_num; ++i)
        {
            vertex[i] = mat.multiply(vertex[i]);
        }
    }

    Matrix3x3 Scale(const real &factor1, const real &factor2, const real &factor3)
    {
        return Matrix3x3(factor1, 0, 0,
                         0, factor2, 0,
                         0, 0, factor3);
    }

    Matrix3x3 Shear(const real &factorXY, const real &factorXZ, const real &factorYX, const real &factorYZ, const real &factorZX, const real &factorZY)
    {
        return Matrix3x3(1, factorXY, factorXZ,
                         factorYX, 1, factorYZ,
                         factorZX, factorZY, 1);
    }

    Matrix3x3 Rotation(real angleX, real angleY, real angleZ)
    {
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
    void ApplyMatrixToVertices(oeVec4 vertex[], const size_t &vertices_num, const Matrix4x4 &mat)
    {
        for (size_t i = 0; i < vertices_num; ++i)
        {
            vertex[i] = mat.multiply(vertex[i]);
        }
    }

    Matrix4x4 Scale4x4(const real &factor1, const real &factor2, const real &factor3)
    {
        return Matrix4x4(factor1, 0, 0, 0,
                         0, factor2, 0, 0,
                         0, 0, factor3, 0,
                         0, 0, 0, 1);
    }

    Matrix4x4 Translation(const real &dx, const real &dy, const real &dz)
    {
        return Matrix4x4(1, 0, 0, dx,
                         0, 1, 0, dy,
                         0, 0, 1, dz,
                         0, 0, 0, 1);
    }

    Matrix4x4 Rotation4x4(real angleX, real angleY, real angleZ)
    {
        Matrix3x3 rotMat = Rotation(angleX, angleY, angleZ);
        return Matrix4x4(rotMat.e11(), rotMat.e12(), rotMat.e13(), 0,
                         rotMat.e21(), rotMat.e22(), rotMat.e23(), 0,
                         rotMat.e31(), rotMat.e32(), rotMat.e33(), 0,
                         0, 0, 0, 1);
    }

    static Matrix4x4 lookAt(const oeVec3 &eye, const oeVec3 &target, const oeVec3 &up)
    {
        oeVec3 f = (target - eye).normalize();
        oeVec3 r = oeVec3::cross(f, up).normalize();
        oeVec3 u = oeVec3::cross(r, f);

        Matrix4x4 mat;
        mat.column1 = oeVec4(r.x, u.x, -f.x, 0);
        mat.column2 = oeVec4(r.y, u.y, -f.y, 0);
        mat.column3 = oeVec4(r.z, u.z, -f.z, 0);
        mat.column4 = oeVec4(-r.dot(eye), -u.dot(eye), f.dot(eye), 1);
        return mat;
    }

    Matrix4x4 perspective(real fov, real aspect, real near, real far)
    {
        real tanHalfFov = tan(fov / 2);
        Matrix4x4 mat = Matrix4x4::identityMatrix();
        mat = {1 / (aspect * tanHalfFov), 0, 0, 0,
               0, 1 / tanHalfFov, 0, 0,
               0, 0, -(far + near) / (far - near), -1,
               0, 0, -2 * far * near / (far - near), 0};
        return mat;
    }

    Matrix4x4 get_projection_matrix(real eye_fov, real aspect_ratio,
                                    real zNear, real zFar) // 投影变换矩阵
    {
        Matrix4x4 projection = Matrix4x4::identityMatrix();

        Matrix4x4 proj, ortho;

        proj = {zNear, 0, 0, 0,
                0, zNear, 0, 0,
                0, 0, zNear + zFar, -zNear * zFar,
                0, 0, -1, 0};

        double w, h, z;
        h = zNear * tan(eye_fov / 2) * 2;
        w = h * aspect_ratio;
        z = zFar - zNear;

        ortho = {2 / w, 0, 0, 0,
                 0, 2 / h, 0, 0,
                 0, 0, 2 / z, -(zFar + zNear) / 2,
                 0, 0, 0, 1};

        projection = ortho.multiply(proj.multiply(projection));

        return projection;
    }

}