#include "bmp.hpp"
#include "draw.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"

int main() {
    const real width = 256;
    const real height = 256;
    Bmp image(width, height);
    Draw draw(&image);

    draw.SetColor(0,0,0);
    draw.SpreadBackground();

    
    draw.SetColor(255,255,255);
    oeVec3 p = {100,100,-100};
    Matrix4x4 per =  Transform::PerspectiveProjection(45, 1, 0.1, 50);
    image.save("output.bmp");
    return 0;
}