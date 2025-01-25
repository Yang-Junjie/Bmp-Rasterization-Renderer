#pragma once 
#include "vector2d.hpp"
struct Matrix2x2
	{
		Matrix2x2() = default;
        Matrix2x2(const Matrix2x2& mat);
        Matrix2x2(const oeVec2& col1, const oeVec2& col2);
        Matrix2x2(const double& col1_x, const double& col1_y, const double& col2_x, const double& col2_y);
		Matrix2x2(Matrix2x2&& other) = default;

        Matrix2x2& operator=(const Matrix2x2& rhs);
        Matrix2x2& operator+=(const Matrix2x2& rhs);
        Matrix2x2& operator-=(const Matrix2x2& rhs);
        Matrix2x2& operator*=(const double& factor);
        Matrix2x2& operator/=(const double& factor);
        Matrix2x2 operator+(const Matrix2x2& rhs)const;
        Matrix2x2 operator-(const Matrix2x2& rhs)const;

        oeVec2 row1()const;
        oeVec2 row2()const;
		
        double& e11();
        double& e12();
		
        double& e21();
        double& e22();

        double determinant()const;
        Matrix2x2& transpose();
        Matrix2x2& invert();
        Matrix2x2& multiply(const Matrix2x2& rhs);
        oeVec2 multiply(const oeVec2& rhs)const;

        Matrix2x2& clear();
        Matrix2x2& set(const double& col1_x, const double& col1_y, const double& col2_x, const double& col2_y);
        Matrix2x2& set(const oeVec2& col1, const oeVec2& col2);
        Matrix2x2& set(const Matrix2x2& other);

        static Matrix2x2 skewSymmetricMatrix(const oeVec2& r);
        static Matrix2x2 identityMatrix();
        static oeVec2 multiply(const Matrix2x2& lhs, const oeVec2& rhs);
        static Matrix2x2 multiply(const Matrix2x2& lhs, const Matrix2x2& rhs);
        static double determinant(const Matrix2x2& mat);
        static bool invert(Matrix2x2& mat);

        oeVec2 column1;
		oeVec2 column2;
	};