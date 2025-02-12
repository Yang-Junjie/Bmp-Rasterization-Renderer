#include "bmp.hpp"
#include "Rasterizer.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"
#include "Texture.hpp"
#include <string>

int main()
{   
    std::string file = "../img/Triangle.bmp";
    std::string save_path = "../img/TriangleCopy1.bmp";

    Bmp bmp(file);
    bmp.save(save_path);
    return 0;
}