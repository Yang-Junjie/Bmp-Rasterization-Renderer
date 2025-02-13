
#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H

#include "common.hpp"
#include "oe_math.hpp"
#include "bmp.hpp"
class Texture{
private:
    std::vector<std::vector<oeVec3>> image_data;

public:
    int width, height;
    Texture(const std::string& name)
    {
        image_data = Bmp::readBMP(name);
        width = image_data.size();
        height = image_data[0].size();
    }

    std::vector<std::vector<oeVec3>> getData(){return image_data;}
    oeVec3 getColor(float u, float v)
    {
        return oeVec3(image_data[u][v].x, image_data[u][v].y, image_data[u][v].z);
    }

};
#endif 
