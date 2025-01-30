#pragma once 
#include "oe_math.hpp"
class Camera {
    public:
        oeVec3 m_e;     
       // oeVec3 m_g; 
       // oeVec3 m_t;  

        Camera(const oeVec3& e): m_e(e) {}
        
       // oeVec3 w = -m_g.normalize();
       // oeVec3 u = (m_t.cross(w)).normalize();
       // oeVec3 v = w.cross(u);


    Matrix4x4 GetViewMatrix() const {
        Matrix4x4 view = Matrix4x4::identityMatrix();   
        Matrix4x4 translate;
        translate ={ 1, 0, 0, -m_e.x,
                     0, 1, 0, -m_e.y,
                     0, 0, 1, -m_e.z,
                     0, 0, 0, 1};

        view = translate.multiply(view);;
        return view;    
    }
        
    };