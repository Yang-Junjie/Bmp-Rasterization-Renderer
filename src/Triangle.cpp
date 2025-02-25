#include "Triangle.hpp"
#include <algorithm>
#include <array>

Triangle::Triangle() {
    v[0] ={ 0,0,0,1};
    v[1] ={ 0,0,0,1};
    v[2] ={ 0,0,0,1};

    color[0] ={ 0.0, 0.0, 0.0};
    color[1] ={ 0.0, 0.0, 0.0};
    color[2] ={ 0.0, 0.0, 0.0};

    tex_coords[0] ={ 0.0, 0.0};
    tex_coords[1] ={ 0.0, 0.0};
    tex_coords[2] ={ 0.0, 0.0};
}

void Triangle::setVertex(int ind, oeVec4 ver){
    v[ind] = ver;
}
void Triangle::setNormal(int ind, oeVec3 n){
    normal[ind] = n;
}
void Triangle::setColor(int ind, float r, float g, float b) {
    if((r<0.0) || (r>255.) ||
       (g<0.0) || (g>255.) ||
       (b<0.0) || (b>255.)) {
        fprintf(stderr, "ERROR! Invalid color values");
        fflush(stderr);
        exit(-1);
    }

    color[ind] = oeVec3((float)r/255.,(float)g/255.,(float)b/255.);
    return;
}
void Triangle::setTexCoord(int ind, oeVec2 uv) {
    tex_coords[ind] = uv;
}

std::array<oeVec4, 3> Triangle::toVector4() const
{
    std::array<oeVec4, 3> res;
    std::transform(std::begin(v), std::end(v), res.begin(), [](auto& vec) { return oeVec4(vec.x, vec.y, vec.z, 1.f); });
    return res;
}

void Triangle::setNormals(const std::array<oeVec3, 3>& normals)
{
    normal[0] = normals[0];
    normal[1] = normals[1];
    normal[2] = normals[2];
}

void Triangle::setColors(const std::array<oeVec3, 3>& colors)
{
    auto first_color = colors[0];
    setColor(0, colors[0].x, colors[0].y, colors[0].z);
    setColor(1, colors[1].x, colors[1].y, colors[1].z);
    setColor(2, colors[2].x, colors[2].y, colors[2].z);
}