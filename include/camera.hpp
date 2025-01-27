#pragma once 
#include "oe_math.hpp"
class Camera {
    public:
        oeVec3 m_e;  // position    
        oeVec3 m_g;  // look_at  
        oeVec3 m_t;  //  up direction

        Camera(const oeVec3& e, const oeVec3& g, const oeVec3& t)
            : m_e(e), m_g(g), m_t(t) {}
        Matrix4x4 GetViewMatrix() const {
        // 计算前向向量 (forward vector)
        oeVec3 f = (m_g - m_e).normalize();

        // 计算右向量 (right vector)
        oeVec3 s = f.cross(m_t).normalize();

        // 计算新的上向量 (up vector)
        oeVec3 u_new = s.cross(f);

        // 构造视图矩阵
        return Matrix4x4(
            s.x, s.y, s.z, -s.dot(m_e),
            u_new.x, u_new.y, u_new.z, -u_new.dot(m_e),
            -f.x, -f.y, -f.z, f.dot(m_e),
            0, 0, 0, 1
        );
    }
        
    };