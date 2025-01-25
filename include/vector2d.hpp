
#pragma once
#include <cmath>  
#include <iostream>
struct oeVec2
{
    double x;
    double y;
    oeVec2() :x(0.0f), y(0.0f) {};
    oeVec2(double x, double y) : x(x), y(y) {}

    static oeVec2 Zero() {
        return oeVec2{ 0.0f,0.0f };
    }

    friend std::ostream& operator<<(std::ostream& os, const oeVec2& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os; // 返回输出流以便链式调用  
    }

    // 向量相加
    oeVec2 operator+(const oeVec2& other) const
    {
        return { this->x + other.x, this->y + other.y };
    }

    // 向量相减
    oeVec2 operator-(const oeVec2& other) const
    {
        return { this->x - other.x, this->y - other.y };
    }

    // 向量取负数
    oeVec2 operator-() const
    {
        return { -this->x, -this->y };
    }

    // 向量的数乘
    oeVec2 operator*(double scalar) const
    {
        return { this->x * scalar, this->y * scalar };
    }

    oeVec2& operator*=(const double& factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

     oeVec2& operator/=(const double& factor)
	{
		x /= factor;
		y /= factor;
		return *this;
	}

    // 向量的数乘
    friend oeVec2 operator*(double scalar, const oeVec2& vec)
    {
        return vec * scalar;
    }

    // 向量的数除
    oeVec2 operator/(double scalar) const
    {
        return { this->x / scalar, this->y / scalar };
    }

    oeVec2& operator+=(const oeVec2& other) {
        this->x += other.x; // 将另一个对象的值加到当前对象上
        this->y += other.y;
        return *this; // 返回当前对象的引用
    }

    oeVec2& operator-=(const oeVec2& other) {
        this->x -= other.x; // 将另一个对象的值加到当前对象上
        this->y -= other.y;
        return *this; // 返回当前对象的引用
    }
   
   
    // 判断两个向量是否相等
    bool operator==(const oeVec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    // 判断两个向量是否不相等
    bool operator!=(const oeVec2& other) const
    {
        return !(*this == other);
    }

    // 判断两个向量是否相等
    bool equals(const oeVec2& other) const
    {
        return *this == other;
    }

    //向量取模
    double  len() const {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    //向量取模
    static inline double len(oeVec2& v) {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    // Normalize the vector
    oeVec2 normalize() const {
        double length = len();
        if (length > 0.0f) {
            return { x / length, y / length };
        }
        return { 0.0f, 0.0f };
    }

    void clear(){
        this->x = 0.0;
        this->y = 0.0;
    }
    void set(double x_,double y_){
        this->x = x_;
        this->y = y_;
    }
    //向量的点积
    double dot(const oeVec2& v) const {
        return this->x * v.x + this->y * v.y;
    }

    //向量的点积
    static inline double dot(const oeVec2& v1, const oeVec2& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    //向量的叉积
    double cross(const oeVec2& v) const {
        return this->x * v.y - this->y * v.x;
    }

    static inline oeVec2 cross(const oeVec2& a, double s)
    {
        return oeVec2(s * a.y, -s * a.x);
    }

    static inline oeVec2 cross(double s, const oeVec2& a)
    {
        return oeVec2(-s * a.y, s * a.x);
    }

    static inline double cross(const oeVec2& v1, const oeVec2& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    //向量的模的平方
    double LengthSquared() const {
        return this->x * this->x + this->y * this->y;
    }

    //向量的模的平方
    static inline double LengthSquared(const oeVec2& v) {
        return v.x * v.x + v.y * v.y;
    }

    //两个向量的距离
    static inline double Distance(const oeVec2 v1, const oeVec2 v2) {
        double dx = v1.x - v2.x;
        double dy = v1.y - v2.y;
        return sqrt(dx * dx + dy * dy);
    }

    //两个向量距离的平方
    static inline double DistanceSquared(const oeVec2 v1, const oeVec2 v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }
};