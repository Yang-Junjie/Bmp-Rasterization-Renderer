#pragma once
#include "oe_math.hpp"
class Camera
{
public:
    oeVec3 m_e;

    Camera(const oeVec3 &e) : m_e(e) {}

    Matrix4x4 GetViewMatrix() const
    {
        oeVec3 target(0, 0, 0); // 看向原点
        oeVec3 up(0, 1, 0);     // 上向量

        oeVec3 z = (m_e - target).normalize();
        oeVec3 x = up.cross(z).normalize();
        oeVec3 y = z.cross(x);

        Matrix4x4 view = {
            x.x, x.y, x.z, -x.dot(m_e),
            y.x, y.y, y.z, -y.dot(m_e),
            z.x, z.y, z.z, -z.dot(m_e),
            0, 0, 0, 1};
        return view;
    }
};