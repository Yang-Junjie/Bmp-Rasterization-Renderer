//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H

#include "common.hpp"
#include "oe_math.hpp"
#include "bmp.hpp"
class Texture{
private:
    std::vector<std::vector<std::vector<uint8_t>>> image_data;

public:
    int width, height;
    Texture(const std::string& name)
    {
        image_data = Bmp::readBMP(name);
        width = image_data.size();
        height = image_data[0].size();
    }

    std::vector<std::vector<std::vector<uint8_t>>> getData(){return image_data;}
    oeVec3 getColor(float u, float v)
    {
        return oeVec3(image_data[u][v][0], image_data[u][v][1], image_data[u][v][2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
