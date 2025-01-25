#include "bmp.hpp"
#include "draw.hpp"
#include <iostream>

int main() {
    Bmp image(256, 256);
    Draw draw(&image);

    draw.SetColor(0,0,0);
    draw.SpreadBackground();

    draw.SetColor(255,255,255);
   // draw.DrawTriangle({100,100},{0,100},{100,0});
    draw.DrawRectangle(0,0,100,100);
    image.save("output.bmp");
    return 0;
}