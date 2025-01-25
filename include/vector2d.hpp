
#pragma once
#include <cmath>  
#include <iostream>
#include "common.hpp"
struct oeVec2
{
    real x;
    real y;
    oeVec2() :x(0.0f), y(0.0f) {};
    oeVec2(const real& x, const real& y) : x(x), y(y) {}

    static oeVec2 Zero() {
        return oeVec2{ 0.0f,0.0f };
    }

    friend std::ostream& operator<<(std::ostream& os, const oeVec2& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os; 
    }

    
    oeVec2 operator+(const oeVec2& other) const
    {
        return { this->x + other.x, this->y + other.y };
    }

    
    oeVec2 operator-(const oeVec2& other) const
    {
        return { this->x - other.x, this->y - other.y };
    }

    
    oeVec2 operator-() const
    {
        return { -this->x, -this->y };
    }

    
    oeVec2 operator*(const real& scalar) const
    {
        return { this->x * scalar, this->y * scalar };
    }

    oeVec2& operator*=(const real& factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

     oeVec2& operator/=(const real& factor)
	{
		x /= factor;
		y /= factor;
		return *this;
	}

    
    friend oeVec2 operator*(const real& scalar, const oeVec2& vec)
    {
        return vec * scalar;
    }

    
    oeVec2 operator/(const real& scalar) const
    {
        return { this->x / scalar, this->y / scalar };
    }

    oeVec2& operator+=(const oeVec2& other) {
        this->x += other.x; 
        this->y += other.y;
        return *this; 
    }

    oeVec2& operator-=(const oeVec2& other) {
        this->x -= other.x; 
        this->y -= other.y;
        return *this; 
    }
   
   
    
    bool operator==(const oeVec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

   
    bool operator!=(const oeVec2& other) const
    {
        return !(*this == other);
    }

    
    bool equals(const oeVec2& other) const
    {
        return *this == other;
    }

    
    real  len() const {
        return sqrt(this->x * this->x + this->y * this->y);
    }

   
    static inline real len(const oeVec2& v) {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    
    oeVec2 normalize() const {
        real length = len();
        if (length > 0.0f) {
            return { x / length, y / length };
        }
        return { 0.0f, 0.0f };
    }

    void clear(){
        this->x = 0.0;
        this->y = 0.0;
    }
    void set(const real& x_,const real& y_) {
        this->x = x_;
        this->y = y_;
    }
    
    real dot(const oeVec2& v) const {
        return this->x * v.x + this->y * v.y;
    }

    
    static inline real dot(const oeVec2& v1, const oeVec2& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    
    real cross(const oeVec2& v) const {
        return this->x * v.y - this->y * v.x;
    }

    static inline oeVec2 cross(const oeVec2& a, real s)
    {
        return oeVec2(s * a.y, -s * a.x);
    }

    static inline oeVec2 cross(const real& s, const oeVec2& a)
    {
        return oeVec2(-s * a.y, s * a.x);
    }

    static inline real cross(const oeVec2& v1, const oeVec2& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    static inline real cross(const real& x1, const real& y1, const real& x2,const real& y2) {
        return x1 * y2 - y1 * x2;
    }

    
    real LengthSquared() const {
        return this->x * this->x + this->y * this->y;
    }

   
    static inline real LengthSquared(const oeVec2& v) {
        return v.x * v.x + v.y * v.y;
    }

    
    static inline real Distance(const oeVec2 v1, const oeVec2 v2) {
        real dx = v1.x - v2.x;
        real dy = v1.y - v2.y;
        return sqrt(dx * dx + dy * dy);
    }

    
    static inline real DistanceSquared(const oeVec2 v1, const oeVec2 v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }
};