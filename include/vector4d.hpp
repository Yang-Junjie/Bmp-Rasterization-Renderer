
#pragma once
#include <cmath>  
#include <iostream>
struct oeVec4
{
    float x;
    float y;
    float z;
    float t;
    oeVec4() :x(0.0f), y(0.0f),z(0.0f),t(0.0f) {};
    oeVec4(float x, float y,float z,float t) : x(x), y(y),z(z),t(t){}

    static oeVec4 Zero() {
        return oeVec4{ 0.0f,0.0f ,0.0f,0.0f};
    }

    friend std::ostream& operator<<(std::ostream& os, const oeVec4& p) {
        os << "(" << p.x << ", " << p.y << p.z<<p.t<<")";
        return os; // 返回输出流以便链式调用  
    }

    // 向量相加
    oeVec4 operator+(const oeVec4& other) const
    {
        return { this->x + other.x, this->y + other.y ,this->z+other.z,this->t+other.t};
    }

    // 向量相减
    oeVec4 operator-(const oeVec4& other) const
    {
        return { this->x - other.x, this->y - other.y,this->z-other.z ,this->t-other.t};
    }

    // 向量取负数
    oeVec4 operator-() const
    {
        return { -this->x, -this->y ,-this->z,-this->t};
    }

    // 向量的数乘
    oeVec4 operator*(float scalar) const
    {
        return { this->x * scalar, this->y * scalar,this->z*scalar ,this->t*scalar };
    }

    // 向量的数乘
    friend oeVec4 operator*(float scalar, const oeVec4& vec)
    {
        return vec * scalar;
    }

    // 向量的数除
    oeVec4 operator/(float scalar) const
    {
        return { this->x / scalar, this->y / scalar ,this->z / scalar ,this->t / scalar};
    }

    oeVec4& operator+=(const oeVec4& other) {
        this->x += other.x; // 将另一个对象的值加到当前对象上
        this->y += other.y;
        this->z += other.z;
        this->t += other.t;
        return *this; // 返回当前对象的引用
    }

    oeVec4& operator-=(const oeVec4& other) {
        this->x -= other.x; // 将另一个对象的值加到当前对象上
        this->y -= other.y;
        this->z -= other.z;
        this->t -= other.t;
        return *this; // 返回当前对象的引用
    }
    oeVec4& operator*=(const oeVec4& other) {
        this->x *= other.x; // 将另一个对象的值加到当前对象上
        this->y *= other.y;
        this->z *= other.z;
        this->t *= other.t;
        return *this; // 返回当前对象的引用
    }

    oeVec4& operator/=(const oeVec4& other) {
        this->x /= other.x; // 将另一个对象的值加到当前对象上
        this->y /= other.y;
        this->z /= other.z;
        this->t /= other.t;
        return *this; // 返回当前对象的引用
    }

   
    // 判断两个向量是否相等
    bool operator==(const oeVec4& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z&& this->t == other.t;
    }

    // 判断两个向量是否不相等
    bool operator!=(const oeVec4& other) const
    {
        return !(*this == other);
    }

    // 判断两个向量是否相等
    bool equals(const oeVec4& other) const
    {
        return *this == other;
    }

    //向量取模
    float  len() const {
        return sqrt(this->x * this->x + this->y * this->y+this->z * this->z+this->t * this->t);
    }

    //向量取模
    static inline float len(oeVec4& v) {
        return sqrt(v.x * v.x + v.y * v.y+v.z * v.z+v.t * v.t);
    }

    // Normalize the vector
    oeVec4 normalize() const {
        float length = len();
        if (length > 0.0f) {
            return { x / length, y / length ,z / length ,t / length};
        }
        return { 0.0f, 0.0f ,0.0f,0.0f};
    }


    //向量的点积
    float dot(const oeVec4& v) const {
        return this->x * v.x + this->y * v.y+this->z * v.z+this->t * v.t;
    }

    //向量的点积
    static inline float dot(const oeVec4& v1, const oeVec4& v2) {
        return v1.x * v2.x + v1.y * v2.y+ v1.z * v2.z+ v1.t * v2.t;
    }

    
    //向量的模的平方
    float LengthSquared() const {
        return this->x * this->x + this->y * this->y+this->z * this->z+this->t * this->t;
    }

    //向量的模的平方
    static inline float LengthSquared(const oeVec4& v) {
        return v.x * v.x + v.y * v.y+v.z * v.z+v.t * v.t;
    }

    //两个向量的距离
    static inline float Distance(const oeVec4 v1, const oeVec4 v2) {
        float dx = v1.x - v2.x;
        float dy = v1.y - v2.y;
        float dz = v1.z - v2.z;
        float dt = v1.z - v2.t;
        return sqrt(dx * dx + dy * dy+dz*dz+dt*dt);
    }

};