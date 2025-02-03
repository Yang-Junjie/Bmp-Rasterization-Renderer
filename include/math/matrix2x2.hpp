#ifndef MATRIX2X2_HPP
#define MATRIX2X2_HPP
#include "vector2d.hpp"
#include "common.hpp"

struct Matrix2x2
{
    Matrix2x2() = default;
    Matrix2x2(const Matrix2x2 &mat);
    Matrix2x2(const oeVec2 &col1, const oeVec2 &col2);
    Matrix2x2(const real &row1_x, const real &row1_y, const real &row2_x, const real &row2_y);
    Matrix2x2(Matrix2x2 &&other) = default;

    Matrix2x2 &operator=(const Matrix2x2 &rhs);
    Matrix2x2 &operator+=(const Matrix2x2 &rhs);
    Matrix2x2 &operator-=(const Matrix2x2 &rhs);
    Matrix2x2 &operator*=(const real &factor);
    Matrix2x2 &operator/=(const real &factor);
    Matrix2x2 operator+(const Matrix2x2 &rhs) const;
    Matrix2x2 operator-(const Matrix2x2 &rhs) const;

    oeVec2 row1() const;
    oeVec2 row2() const;

    real &e11();
    real &e12();

    real &e21();
    real &e22();

    real determinant() const;
    Matrix2x2 &transpose();
    Matrix2x2 &invert();
    Matrix2x2 &multiply(const Matrix2x2 &rhs);
    oeVec2 multiply(const oeVec2 &rhs) const;

    Matrix2x2 &clear();
    Matrix2x2 &set(const real &col1_x, const real &col1_y, const real &col2_x, const real &col2_y);
    Matrix2x2 &set(const oeVec2 &col1, const oeVec2 &col2);
    Matrix2x2 &set(const Matrix2x2 &other);

    static Matrix2x2 skewSymmetricMatrix(const oeVec2 &r);
    static Matrix2x2 identityMatrix();
    static oeVec2 multiply(const Matrix2x2 &lhs, const oeVec2 &rhs);
    static Matrix2x2 multiply(const Matrix2x2 &lhs, const Matrix2x2 &rhs);
    static real determinant(const Matrix2x2 &mat);
    static bool invert(Matrix2x2 &mat);

    oeVec2 column1;
    oeVec2 column2;
};

#endif