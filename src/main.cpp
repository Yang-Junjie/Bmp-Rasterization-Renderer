#include "bmp.hpp"
#include "draw.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"

int main() {
    const real width = 256;
    const real heigth = 256;
    Bmp image(width, heigth);
    Draw draw(&image);


    draw.SetColor(0,0,0);
    draw.SpreadBackground();

    draw.SetColor(255,255,255);

    
    oeVec2 vertex[] = {oeVec2{0,0},oeVec2{0,-100},oeVec2{100,0}};
    
    Matrix2x2 scale_mat = Transform::Scale(0.5,0.5);
    Matrix2x2 rotation_mat = Transform::Rotation(45);
    
    Transform::ApplyMatrixToVertices(vertex,3,rotation_mat);
    Transform::ApplyMatrixToVertices(vertex,3,scale_mat);

    Transform::CoordChange(vertex[0],width,heigth);
    Transform::CoordChange(vertex[1],width,heigth);
    Transform::CoordChange(vertex[2],width,heigth);

   
    draw.DrawTriangle(vertex[0],vertex[1],vertex[2]);

    
    

    image.save("build/output.bmp");
    return 0;
}