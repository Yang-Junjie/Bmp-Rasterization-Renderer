#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include "common.hpp"
#include "vector3d.hpp"
#include "vector2d.hpp"

struct  Matrix3x3
{
    struct ByRows {};

    Matrix3x3() = default;
    Matrix3x3(const Matrix3x3& mat);
    Matrix3x3(const oeVec3& col1, const oeVec3& col2, const oeVec3& col3);
    Matrix3x3(ByRows,const oeVec3& row1, const oeVec3& row2, const oeVec3& row3);
    Matrix3x3(const real& row1_x, const real& row1_y, const real& row1_z,
                const real& row2_x, const real& row2_y, const real& row2_z,
                const real& row3_x, const real& row3_y, const real& row3_z);
    Matrix3x3(Matrix3x3&& other) = default;

    Matrix3x3& operator= (const Matrix3x3& rhs);
    Matrix3x3& operator+=(const Matrix3x3& rhs);
    Matrix3x3& operator-=(const Matrix3x3& rhs);
    Matrix3x3& operator*=(const real& factor);
    Matrix3x3& operator/=(const real& factor);

    oeVec3 row1()const;
    oeVec3 row2()const;
    oeVec3 row3()const;

    real& e11();
    real& e12();
    real& e13();

    real& e21();
    real& e22();
    real& e23();

    real& e31();
    real& e32();
    real& e33();

    Matrix3x3& set(const real& col1_x, const real& col1_y, const real& col1_z,
        const real& col2_x, const real& col2_y, const real& col2_z,
        const real& col3_x, const real& col3_y, const real& col3_z);
    Matrix3x3& set(const oeVec3& col1, const oeVec3& col2, const oeVec3& col3);
    Matrix3x3& set(const Matrix3x3& other);
    Matrix3x3& clear();

    oeVec3 multiply(const oeVec3& rhs)const;
    Matrix3x3& multiply(const Matrix3x3& rhs);
    real determinant()const;
    Matrix3x3& transpose();
    Matrix3x3& invert();

    static Matrix3x3 skewSymmetricMatrix(const oeVec3& v);
    static Matrix3x3 identityMatrix();
    static Matrix3x3 multiply(const Matrix3x3& lhs, const Matrix3x3& rhs);
    static oeVec3 multiply(const Matrix3x3& lhs, const oeVec3& rhs);
    static real determinant(const Matrix3x3& mat);
    static bool invert(Matrix3x3& mat);

    oeVec3 column1;
    oeVec3 column2;
    oeVec3 column3;
};

#endif