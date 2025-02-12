//
// Created by LEI XU on 4/11/19.
//

#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H

#include "oe_math.hpp"
#include "Texture.hpp"

class Triangle{

public:
    oeVec4 v[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
    /*Per vertex values*/
    oeVec3 color[3]; //color at each vertex;
    oeVec2 tex_coords[3]; //texture u,v
    oeVec3 normal[3]; //normal vector for each vertex

    Texture *tex= nullptr;
    Triangle();

    oeVec4 a() const { return v[0]; }
    oeVec4 b() const { return v[1]; }
    oeVec4 c() const { return v[2]; }

    void setVertex(int ind, oeVec4 ver); /*set i-th vertex coordinates */
    void setNormal(int ind, oeVec3 n); /*set i-th vertex normal vector*/
    void setColor(int ind, float r, float g, float b); /*set i-th vertex color*/

    void setNormals(const std::array<oeVec3, 3>& normals);
    void setColors(const std::array<oeVec3, 3>& colors);
    void setTexCoord(int ind,oeVec2 uv ); /*set i-th vertex texture coordinate*/
    std::array<oeVec4, 3> toVector4() const;
};






#endif //RASTERIZER_TRIANGLE_H
