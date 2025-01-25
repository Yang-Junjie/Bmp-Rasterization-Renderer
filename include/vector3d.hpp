
#pragma once
#include <cmath>  
#include <iostream>
struct oeVec3
{
    float x;
    float y;
    float z;
    oeVec3() :x(0.0f), y(0.0f),z(0.0f) {};
    oeVec3(float x, float y,float z) : x(x), y(y),z(z) {}

    static oeVec3 Zero() {
        return oeVec3{ 0.0f,0.0f ,0.0f};
    }

    friend std::ostream& operator<<(std::ostream& os, const oeVec3& p) {
        os << "(" << p.x << ", " << p.y << p.z<<")";
        return os; // 返回输出流以便链式调用  
    }

    // 向量相加
    oeVec3 operator+(const oeVec3& other) const
    {
        return { this->x + other.x, this->y + other.y ,this->z+other.z};
    }

    // 向量相减
    oeVec3 operator-(const oeVec3& other) const
    {
        return { this->x - other.x, this->y - other.y,this->z-other.z };
    }

    // 向量取负数
    oeVec3 operator-() const
    {
        return { -this->x, -this->y ,-this->z};
    }

    // 向量的数乘
    oeVec3 operator*(float scalar) const
    {
        return { this->x * scalar, this->y * scalar,this->z*scalar };
    }

    // 向量的数乘
    friend oeVec3 operator*(float scalar, const oeVec3& vec)
    {
        return vec * scalar;
    }

    // 向量的数除
    oeVec3 operator/(float scalar) const
    {
        return { this->x / scalar, this->y / scalar ,this->z / scalar };
    }

    oeVec3& operator+=(const oeVec3& other) {
        this->x += other.x; // 将另一个对象的值加到当前对象上
        this->y += other.y;
        this->z += other.z;
        return *this; // 返回当前对象的引用
    }

    oeVec3& operator-=(const oeVec3& other) {
        this->x -= other.x; // 将另一个对象的值加到当前对象上
        this->y -= other.y;
        this->z -= other.z;
        return *this; // 返回当前对象的引用
    }
    oeVec3& operator*=(const oeVec3& other) {
        this->x *= other.x; // 将另一个对象的值加到当前对象上
        this->y *= other.y;
        this->z *= other.z;
        return *this; // 返回当前对象的引用
    }

    oeVec3& operator/=(const oeVec3& other) {
        this->x /= other.x; // 将另一个对象的值加到当前对象上
        this->y /= other.y;
        this->z /= other.z;
        return *this; // 返回当前对象的引用
    }

   
    // 判断两个向量是否相等
    bool operator==(const oeVec3& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    // 判断两个向量是否不相等
    bool operator!=(const oeVec3& other) const
    {
        return !(*this == other);
    }

    // 判断两个向量是否相等
    bool equals(const oeVec3& other) const
    {
        return *this == other;
    }

    //向量取模
    float  len() const {
        return sqrt(this->x * this->x + this->y * this->y+this->z * this->z);
    }

    //向量取模
    static inline float len(oeVec3& v) {
        return sqrt(v.x * v.x + v.y * v.y+v.z * v.z);
    }

    // Normalize the vector
    oeVec3 normalize() const {
        float length = len();
        if (length > 0.0f) {
            return { x / length, y / length ,z / length };
        }
        return { 0.0f, 0.0f ,0.0f};
    }


    //向量的点积
    float dot(const oeVec3& v) const {
        return this->x * v.x + this->y * v.y+this->z * v.z;
    }

    //向量的点积
    static inline float dot(const oeVec3& v1, const oeVec3& v2) {
        return v1.x * v2.x + v1.y * v2.y+ v1.z * v2.z;
    }

    
    //向量的模的平方
    float LengthSquared() const {
        return this->x * this->x + this->y * this->y,this->z * this->z;
    }

    //向量的模的平方
    static inline float LengthSquared(const oeVec3& v) {
        return v.x * v.x + v.y * v.y+v.z * v.z;
    }

    //两个向量的距离
    static inline float Distance(const oeVec3 v1, const oeVec3 v2) {
        float dx = v1.x - v2.x;
        float dy = v1.y - v2.y;
        float dz = v1.z - v2.z;
        return sqrt(dx * dx + dy * dy+dz*dz);
    }

};