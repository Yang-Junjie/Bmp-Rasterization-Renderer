#include "bmp.hpp"
#include "draw.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"

int main() {
    const real width = 512;
    const real height = 512;
    Bmp image(width, height);
    Draw draw(&image);
    
    draw.SetColor(0,0,0);
    draw.SpreadBackground();
    draw.SetColor(255,0,255);
    

    draw.SetColor(255, 255, 0);
    draw.DrawTriangle(oeVec2(-150, -150),255,0,0,oeVec2(150, -150),0,255,0, oeVec2(0, 0),0,0,255);
    
    image.save("output.bmp");
    return 0;
}