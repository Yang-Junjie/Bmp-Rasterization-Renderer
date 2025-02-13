#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H

#include "oe_math.hpp"
#include "Texture.hpp"

class Triangle{

public:
    oeVec4 v[3]; 
    oeVec3 color[3]; 
    oeVec2 tex_coords[3]; 
    oeVec3 normal[3]; 

    Texture *tex= nullptr;
    Triangle();

    oeVec4 a() const { return v[0]; }
    oeVec4 b() const { return v[1]; }
    oeVec4 c() const { return v[2]; }

    void setVertex(int ind, oeVec4 ver); 
    void setNormal(int ind, oeVec3 n); 
    void setColor(int ind, float r, float g, float b); 

    void setNormals(const std::array<oeVec3, 3>& normals);
    void setColors(const std::array<oeVec3, 3>& colors);
    void setTexCoord(int ind,oeVec2 uv ); 
    std::array<oeVec4, 3> toVector4() const;
};






#endif 
