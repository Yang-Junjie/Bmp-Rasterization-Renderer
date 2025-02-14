
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
        if(u<0) u=0;
        if(v<0) v=0;
        if(u>1) u=1;
        if(v>1) v=1;


        auto x = u * width;
        auto y = (1 - v) * height;
        return oeVec3(image_data[x][y].x, image_data[x][y].y, image_data[x][y].z);
    }

};
#endif 
