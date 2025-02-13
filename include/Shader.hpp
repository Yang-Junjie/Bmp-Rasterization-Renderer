
#ifndef RASTERIZER_SHADER_H
#define RASTERIZER_SHADER_H
#include "oe_math.hpp"
#include "Texture.hpp"


struct fragment_shader_payload
{
    fragment_shader_payload()
    {
        texture = nullptr;
    }

    fragment_shader_payload(const oeVec3& col, const oeVec3& nor,const oeVec2& tc, Texture* tex) :
         color(col), normal(nor), tex_coords(tc), texture(tex) {}


    oeVec3 view_pos;
    oeVec3 color;
    oeVec3 normal;
    oeVec2 tex_coords;
    Texture* texture;
};

struct vertex_shader_payload
{
    oeVec3 position;
};

#endif //RASTERIZER_SHADER_H
