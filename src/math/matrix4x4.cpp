#include "matrix4x4.hpp"

#include "matrix4x4.hpp"


Matrix4x4::Matrix4x4(const oeVec4& col1, const oeVec4& col2, const oeVec4& col3, const oeVec4& col4)
    :column1(col1), column2(col2), column3(col3), column4(col4)
{
}
Matrix4x4::Matrix4x4(const real& col1_x, const real& col1_y, const real& col1_z, const real& col1_w, const real& col2_x, const real& col2_y, const real& col2_z, const real& col2_w, const real& col3_x, const real& col3_y, const real& col3_z, const real& col3_w, const real& col4_x, const real& col4_y, const real& col4_z, const real& col4_w)
    : column1(col1_x, col1_y, col1_z, col1_w),
    column2(col2_x, col2_y, col2_z, col2_w),
    column3(col3_x, col3_y, col3_z, col3_w),
    column4(col4_x, col4_y, col4_z, col4_w)
{
}
Matrix4x4::Matrix4x4(ByRows, const oeVec4& row1, const oeVec4& row2, const oeVec4& row3, const oeVec4& row4)
    : column1(row1.x, row2.x, row3.x, row4.x),
      column2(row1.y, row2.y, row3.y, row4.y),
      column3(row1.z, row2.z, row3.z, row4.z),
      column4(row1.t, row2.t, row3.t, row4.t)
{}
Matrix4x4::Matrix4x4(const Matrix3x3& mat)
    :column1(mat.column1.x, mat.column1.y, mat.column1.z, 0),
    column2(mat.column2.x, mat.column2.y, mat.column2.z, 0),
    column3(mat.column3.x, mat.column3.y, mat.column3.z, 0)
{
}

Matrix4x4& Matrix4x4::operator=(const Matrix3x3& rhs)
{
    column1 = rhs.column1;
    column2 = rhs.column2;
    column3 = rhs.column3;
    column4.clear();
    return *this;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
{
    column1 = rhs.column1;
    column2 = rhs.column2;
    column3 = rhs.column3;
    column4 = rhs.column4;
    return *this;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs)
{
    column1 += rhs.column1;
    column2 += rhs.column2;
    column3 += rhs.column3;
    column4 += rhs.column4;
    return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs)
{
    column1 -= rhs.column1;
    column2 -= rhs.column2;
    column3 -= rhs.column3;
    column4 -= rhs.column4;
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(const real& factor)
{
    column1 *= factor;
    column2 *= factor;
    column3 *= factor;
    column4 *= factor;
    return *this;
}

Matrix4x4& Matrix4x4::operator/=(const real& factor)
{
    column1 /= factor;
    column2 /= factor;
    column3 /= factor;
    column4 /= factor;
    return *this;
}

oeVec4 Matrix4x4::row1() const
{
    return oeVec4(column1.x, column2.x,
        column3.x, column4.x);
}

oeVec4 Matrix4x4::row2() const
{
    return oeVec4(column1.y, column2.y,
        column3.y, column4.y);
}

oeVec4 Matrix4x4::row3() const
{
    return oeVec4(column1.z, column2.z,
        column3.z, column4.z);
}

oeVec4 Matrix4x4::row4() const
{
    return oeVec4(column1.t, column2.t,
        column3.t, column4.t);
}

real& Matrix4x4::e11()
{
    return column1.x;
}

real& Matrix4x4::e12()
{
    return column1.y;
}

real& Matrix4x4::e13()
{
    return column1.z;
}

real& Matrix4x4::e14()
{
    return column1.t;
}

real& Matrix4x4::e21()
{
    return column2.x;
}

real& Matrix4x4::e22()
{
    return column2.y;
}

real& Matrix4x4::e23()
{
    return column2.z;
}

real& Matrix4x4::e24()
{
    return column2.t;
}

real& Matrix4x4::e31()
{
    return column3.x;
}

real& Matrix4x4::e32()
{
    return column3.y;
}

real& Matrix4x4::e33()
{
    return column3.z;
}

real& Matrix4x4::e34()
{
    return column3.t;
}

real& Matrix4x4::e41()
{
    return column4.x;
}

real& Matrix4x4::e42()
{
    return column4.y;
}

real& Matrix4x4::e43()
{
    return column4.z;
}

real& Matrix4x4::e44()
{
    return column4.t;
}

Matrix4x4& Matrix4x4::set(const real& col1_x, const real& col1_y, const real& col1_z, const real& col1_w,
    const real& col2_x, const real& col2_y, const real& col2_z, const real& col2_w, const real& col3_x,
    const real& col3_y, const real& col3_z, const real& col3_w, const real& col4_x, const real& col4_y,
    const real& col4_z, const real& col4_w)
{
    column1.set(col1_x, col1_y, col1_z, col1_w);
    column2.set(col2_x, col2_y, col2_z, col2_w);
    column3.set(col3_x, col3_y, col3_z, col3_w);
    column4.set(col4_x, col4_y, col4_z, col4_w);
    return *this;
}

Matrix4x4& Matrix4x4::set(const oeVec4& col1, const oeVec4& col2, const oeVec4& col3, const oeVec4& col4)
{
    column1.set(col1);
    column2.set(col2);
    column3.set(col3);
    column4.set(col4);
    return *this;
}

Matrix4x4& Matrix4x4::set(const Matrix4x4& other)
{
    column1.set(other.column1);
    column2.set(other.column2);
    column3.set(other.column3);
    column4.set(other.column4);
    return *this;
}

Matrix4x4& Matrix4x4::set(const Matrix3x3& other)
{
    column1.set(other.column1);
    column2.set(other.column2);
    column3.set(other.column3);
    column4.clear();
    return *this;
}

Matrix4x4& Matrix4x4::clear()
{
    column1.clear();
    column2.clear();
    column3.clear();
    column4.clear();
    return *this;
}

oeVec4 Matrix4x4::multiply(const oeVec4& rhs) const
{
    return multiply(*this, rhs);
}

Matrix4x4& Matrix4x4::multiply(const Matrix4x4& rhs)
{
    *this = multiply(*this, rhs);
    return *this;
}

real Matrix4x4::determinant() const
{
    return determinant(*this);
}

Matrix4x4& Matrix4x4::transpose()
{
    std::swap(column1.y, column2.x);
    std::swap(column1.z, column3.x);
    std::swap(column1.t, column4.x);
    std::swap(column2.z, column3.y);
    std::swap(column2.t, column4.y);

    std::swap(column3.t, column4.z);
    return *this;
}

Matrix4x4& Matrix4x4::invert()
{
    invert(*this);
    return *this;
}

Matrix4x4 Matrix4x4::identityMatrix()
{
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 };
}

Matrix4x4 Matrix4x4::multiply(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    return { multiply(lhs, rhs.column1),
        multiply(lhs, rhs.column2),
        multiply(lhs, rhs.column3),
        multiply(lhs, rhs.column4) };
}

oeVec4 Matrix4x4::multiply(const Matrix4x4& lhs, const oeVec4& rhs)
{
    return {
        lhs.column1.x * rhs.x + lhs.column2.x * rhs.y + lhs.column3.x * rhs.z + lhs.column4.x * rhs.t,
        lhs.column1.y * rhs.x + lhs.column2.y * rhs.y + lhs.column3.y * rhs.z + lhs.column4.y * rhs.t,
        lhs.column1.z * rhs.x + lhs.column2.z * rhs.y + lhs.column3.z * rhs.z + lhs.column4.z * rhs.t,
        lhs.column1.t * rhs.x + lhs.column2.t * rhs.y + lhs.column3.t * rhs.z + lhs.column4.t * rhs.t
    };
}

real Matrix4x4::determinant(const Matrix4x4& mat)
{
    const real det11 = Matrix3x3::determinant({
        mat.column2.y, mat.column2.z, mat.column2.t,
    mat.column3.y, mat.column3.z, mat.column3.t ,
    mat.column4.y, mat.column4.z, mat.column4.t });

    const real det21 = -Matrix3x3::determinant({
        mat.column2.x, mat.column2.z, mat.column2.t,
    mat.column3.x, mat.column3.z, mat.column3.t ,
    mat.column4.x, mat.column4.z, mat.column4.t });

    const real det31 = Matrix3x3::determinant({
        mat.column2.x, mat.column2.y, mat.column2.t,
    mat.column3.x, mat.column3.y, mat.column3.t ,
    mat.column4.x, mat.column4.y, mat.column4.t });

    const real det41 = -Matrix3x3::determinant({
        mat.column2.x, mat.column2.y, mat.column2.z,
    mat.column3.x, mat.column3.y, mat.column3.z ,
    mat.column4.x, mat.column4.y, mat.column4.z });

    return mat.column1.x * det11 +
        mat.column1.y * det21 +
        mat.column1.z * det31 +
        mat.column1.t * det41;
}

bool Matrix4x4::invert(Matrix4x4& mat)
{
    const real det = mat.determinant();
    if (det<Constant::Epsilon)
        return false;

    const real det11 = Matrix3x3::determinant(
        { mat.e22(),mat.e23(),mat.e24(),
        mat.e32(),mat.e33(),mat.e34(),
        mat.e42(),mat.e43(),mat.e44() });

    const real det12 = -Matrix3x3::determinant(
        { mat.e12(),mat.e13(),mat.e14(),
        mat.e32(),mat.e33(),mat.e34(),
        mat.e42(),mat.e43(),mat.e44() });

    const real det13 = Matrix3x3::determinant(
        { mat.e12(),mat.e13(),mat.e14(),
        mat.e22(),mat.e23(),mat.e24(),
        mat.e42(),mat.e43(),mat.e44() });

    const real det14 = -Matrix3x3::determinant(
        { mat.e12(),mat.e13(),mat.e14(),
        mat.e22(),mat.e23(),mat.e24(),
        mat.e32(),mat.e33(),mat.e34() });


    const real det21 = -Matrix3x3::determinant(
        { mat.e21(),mat.e23(),mat.e24(),
        mat.e31(),mat.e33(),mat.e34(),
        mat.e41(),mat.e43(),mat.e44() });

    const real det22 = Matrix3x3::determinant(
        { mat.e11(),mat.e13(),mat.e14(),
        mat.e31(),mat.e33(),mat.e34(),
        mat.e41(),mat.e43(),mat.e44() });

    const real det23 = -Matrix3x3::determinant(
        { mat.e11(),mat.e13(),mat.e24(),
        mat.e21(),mat.e23(),mat.e24(),
        mat.e41(),mat.e43(),mat.e44() });

    const real det24 = Matrix3x3::determinant(
        { mat.e11(),mat.e13(),mat.e14(),
        mat.e21(),mat.e23(),mat.e24(),
        mat.e31(),mat.e33(),mat.e34() });


    const real det31 = Matrix3x3::determinant(
        { mat.e21(),mat.e22(),mat.e24(),
        mat.e31(),mat.e32(),mat.e34(),
        mat.e41(),mat.e42(),mat.e44() });

    const real det32 = -Matrix3x3::determinant(
        { mat.e11(),mat.e12(),mat.e14(),
        mat.e31(),mat.e32(),mat.e34(),
        mat.e41(),mat.e42(),mat.e44() });

    const real det33 = Matrix3x3::determinant(
        { mat.e11(),mat.e12(),mat.e14(),
        mat.e21(),mat.e22(),mat.e24(),
        mat.e41(),mat.e42(),mat.e44() });

    const real det34 = -Matrix3x3::determinant(
        { mat.e11(),mat.e12(),mat.e14(),
        mat.e21(),mat.e22(),mat.e24(),
        mat.e31(),mat.e32(),mat.e34() });


    const real det41 = -Matrix3x3::determinant(
        { mat.e21(),mat.e22(),mat.e23(),
        mat.e31(),mat.e32(),mat.e33(),
        mat.e41(),mat.e42(),mat.e43() });

    const real det42 = Matrix3x3::determinant(
        { mat.e11(),mat.e12(),mat.e13(),
        mat.e31(),mat.e32(),mat.e33(),
        mat.e41(),mat.e42(),mat.e43() });

    const real det43 = -Matrix3x3::determinant(
        { mat.e11(),mat.e12(),mat.e13(),
        mat.e21(),mat.e22(),mat.e23(),
        mat.e41(),mat.e42(),mat.e43() });

    const real det44 = Matrix3x3::determinant(
        { mat.e11(),mat.e12(),mat.e13(),
        mat.e21(),mat.e22(),mat.e23(),
        mat.e31(),mat.e32(),mat.e33() });

    mat.set(det11, det12, det13, det14,
        det21, det22, det23, det24,
        det31, det32, det33, det34,
        det41, det42, det43, det44);
    
    mat /= det;
    return true;
}

