#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP
#include <cmath>  
#include <iostream>
#include "common.hpp"
struct oeVec3
{
    real x;
    real y;
    real z;
    oeVec3() :x(0.0f), y(0.0f),z(0.0f) {};
    oeVec3(const real& x, const real& y,const real& z) : x(x), y(y),z(z) {}

    static oeVec3 Zero() {
        return oeVec3{ 0.0f,0.0f ,0.0f};
    }

    friend std::ostream& operator<<(std::ostream& os, const oeVec3& p) {
        os << "(" << p.x << ", " << p.y << p.z<<")";
        return os; 
    }

    
    oeVec3 operator+(const oeVec3& other) const
    {
        return { this->x + other.x, this->y + other.y ,this->z+other.z};
    }

   
    oeVec3 operator-(const oeVec3& other) const
    {
        return { this->x - other.x, this->y - other.y,this->z-other.z };
    }

   
    oeVec3 operator-() const
    {
        return { -this->x, -this->y ,-this->z};
    }

    
    oeVec3 operator*(const real& scalar) const
    {
        return { this->x * scalar, this->y * scalar,this->z*scalar };
    }

   
    friend oeVec3 operator*(const real& scalar, const oeVec3& vec)
    {
        return vec * scalar;
    }

    oeVec3& operator*=(const real& factor)
	{
		x *= factor;
		y *= factor;
        z *= factor;
		return *this;
	}

    oeVec3& operator/=(const real& factor)
	{
		x /= factor;
		y /= factor;
        z /= factor;
		return *this;
	}
    oeVec3 operator/(const real& scalar) const
    {
        return { this->x / scalar, this->y / scalar ,this->z / scalar };
    }

    

    oeVec3& operator+=(const oeVec3& other) {
        this->x += other.x; 
        this->y += other.y;
        this->z += other.z;
        return *this; 
    }

    oeVec3& operator-=(const oeVec3& other) {
        this->x -= other.x; 
        this->y -= other.y;
        this->z -= other.z;
        return *this; 
    }
   

   
    bool operator==(const oeVec3& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    bool operator!=(const oeVec3& other) const
    {
        return !(*this == other);
    }

    bool equals(const oeVec3& other) const
    {
        return *this == other;
    }

    real  len() const {
        return sqrt(this->x * this->x + this->y * this->y+this->z * this->z);
    }

    static inline real len(const oeVec3& v) {
        return sqrt(v.x * v.x + v.y * v.y+v.z * v.z);
    }

    oeVec3 normalize() const {
        real length = len();
        if (length > 0.0f) {
            return { x / length, y / length ,z / length };
        }
        return { 0.0f, 0.0f ,0.0f};
    }
   

    real dot(const oeVec3& v) const {
        return this->x * v.x + this->y * v.y+this->z * v.z;
    }

    static inline real dot(const oeVec3& v1, const oeVec3& v2) {
        return v1.x * v2.x + v1.y * v2.y+ v1.z * v2.z;
    }

    // 叉乘
    oeVec3 cross(const oeVec3& v) const {
        return oeVec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    // 叉乘
    static oeVec3 cross(const oeVec3& v1,const oeVec3& v2)  {
        return oeVec3(
            v2.y * v1.z - v2.z * v1.y,
            v2.z * v1.x - v2.x * v1.z,
            v2.x * v1.y - v2.y * v1.x
        );
    }
    real LengthSquared() const {
        return this->x * this->x + this->y * this->y,this->z * this->z;
    }

    static inline real LengthSquared(const oeVec3& v) {
        return v.x * v.x + v.y * v.y+v.z * v.z;
    }

    static inline real Distance(const oeVec3 v1, const oeVec3 v2) {
        real dx = v1.x - v2.x;
        real dy = v1.y - v2.y;
        real dz = v1.z - v2.z;
        return sqrt(dx * dx + dy * dy+dz*dz);
    }

    void clear(){
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }
    void set(const real& x_,const real& y_,const real& z_){
        this->x = x_;
        this->y = y_;
        this->z = z_;
    }

};
#endif