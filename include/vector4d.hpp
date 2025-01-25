
#pragma once
#include <cmath>  
#include <iostream>
#include "common.hpp"
#include "vector3d.hpp"
struct oeVec4
{
    real x;
    real y;
    real z;
    real t;
    oeVec4() :x(0.0f), y(0.0f),z(0.0f),t(0.0f) {};
    oeVec4(const real& x, const real& y,const real& z,const real& t) : x(x), y(y),z(z),t(t){}
    
    static oeVec4 Zero() {
        return oeVec4{ 0.0f,0.0f ,0.0f,0.0f};
    }

    friend std::ostream& operator<<(std::ostream& os, const oeVec4& p) {
        os << "(" << p.x << ", " << p.y << p.z<<p.t<<")";
        return os;   
    }

    oeVec4 operator+(const oeVec4& other) const
    {
        return { this->x + other.x, this->y + other.y ,this->z+other.z,this->t+other.t};
    }

    oeVec4 operator-(const oeVec4& other) const
    {
        return { this->x - other.x, this->y - other.y,this->z-other.z ,this->t-other.t};
    }

    oeVec4 operator-() const
    {
        return { -this->x, -this->y ,-this->z,-this->t};
    }

    oeVec4 operator*(const real& scalar) const
    {
        return { this->x * scalar, this->y * scalar,this->z*scalar ,this->t*scalar };
    }

    friend oeVec4 operator*(const real& scalar, const oeVec4& vec)
    {
        return vec * scalar;
    }

    oeVec4 operator/(const real& scalar) const
    {
        return { this->x / scalar, this->y / scalar ,this->z / scalar ,this->t / scalar};
    }

    oeVec4& operator+=(const oeVec4& other) {
        this->x += other.x; 
        this->y += other.y;
        this->z += other.z;
        this->t += other.t;
        return *this; 
    }

    oeVec4& operator-=(const oeVec4& other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->t -= other.t;
        return *this; 
    }
    oeVec4& operator*=(const real& scalar) {
        this->x *= scalar; 
        this->y *= scalar;
        this->z *= scalar;
        this->t *= scalar;
        return *this;
    }

    oeVec4& operator/=(const real& scalar) {
        this->x /= scalar; 
        this->y /= scalar;
        this->z /= scalar;
        this->t /= scalar;
        return *this;
    }

   oeVec4& operator=(const oeVec3& copy){
        x = copy.x;
		y = copy.y;
		z = copy.z;
		t = 0.0;
		return *this;
    }
    
    bool operator==(const oeVec4& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z&& this->t == other.t;
    }

    bool operator!=(const oeVec4& other) const
    {
        return !(*this == other);
    }

    bool equals(const oeVec4& other) const
    {
        return *this == other;
    }

    real  len() const {
        return sqrt(this->x * this->x + this->y * this->y+this->z * this->z+this->t * this->t);
    }

    static inline real len(oeVec4& v) {
        return sqrt(v.x * v.x + v.y * v.y+v.z * v.z+v.t * v.t);
    }

    oeVec4 normalize() const {
        real length = len();
        if (length > 0.0f) {
            return { x / length, y / length ,z / length ,t / length};
        }
        return { 0.0f, 0.0f ,0.0f,0.0f};
    }


    real dot(const oeVec4& v) const {
        return this->x * v.x + this->y * v.y+this->z * v.z+this->t * v.t;
    }

    static inline real dot(const oeVec4& v1, const oeVec4& v2) {
        return v1.x * v2.x + v1.y * v2.y+ v1.z * v2.z+ v1.t * v2.t;
    }

    
    real LengthSquared() const {
        return this->x * this->x + this->y * this->y+this->z * this->z+this->t * this->t;
    }

    static inline real LengthSquared(const oeVec4& v) {
        return v.x * v.x + v.y * v.y+v.z * v.z+v.t * v.t;
    }

    static inline real Distance(const oeVec4 v1, const oeVec4 v2) {
        real dx = v1.x - v2.x;
        real dy = v1.y - v2.y;
        real dz = v1.z - v2.z;
        real dt = v1.z - v2.t;
        return sqrt(dx * dx + dy * dy+dz*dz+dt*dt);
    }
    void clear(){
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
        this->t = 0.0;
    }
    void set(real x_,real y_,real z_,real t_){
        this->x = x_;
        this->y = y_;
        this->z = z_;
        this->t = t_;
    }

    void set(const oeVec4& other){
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->t = other.t;
    }

    void set(const oeVec3& other){
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->t = 0.0;
    }
};