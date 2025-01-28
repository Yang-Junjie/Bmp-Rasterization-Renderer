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
    draw.SetColor(255,0,255);

    
     oeVec2 polygonVertices[] = {
        oeVec2(100, 100),  
        oeVec2(200, 100),  
        oeVec2(200, 200),  
        oeVec2(100, 200)   
    };

   
    size_t triangleIndices[] = {
        0, 1, 2,  
        0, 2, 3   
    };

   
    draw.SetColor(255, 0, 0);
    draw.DrawPolygon(polygonVertices,4,triangleIndices,6);
    
    
    image.save("output.bmp");
    return 0;
}