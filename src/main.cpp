#include "bmp.hpp"
#include "Rasterizer.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"

int main()
{
    const real width = 1080;
    const real height = 1080;
    Bmp bmp(width, height);
    Rasterizer draw(&bmp);

    draw.SpreadBackground(0, 0, 0);

    Camera camera(oeVec3(0, 0, 10));
    std::vector<Vertex> ver{
        
        {{ 1,  1,-1},{255,255,255}, {0,0,1}},  
        {{-1,  1,-1},{255,255,255}, {0,0,1}},  
        {{-1, -1,-1},{255,255,255}, {0,0,1}}, 
        {{ 1, -1,-1},{255,255,255}, {0,0,1}},  

        {{ 1,  1, 1},{0,255,255}, {-1,0,0}},   
        {{-1,  1, 1},{0,255,255}, {-1,0,0}},  
        {{-1, -1, 1},{0,255,255}, {-1,0,0}}, 
        {{ 1, -1, 1},{0,255,255}, {-1,0,0}}   
    };

    
    // 每个面由两个三角形组成
    std::vector<oeVec3> ind{
        // 前面
        {0, 1, 2},
        {2, 3, 0},
        // 右面
        {3, 7, 4},
        {4, 0, 3},
        // 顶面
        {0, 4, 5},
        {5, 1, 0},
        // 左面
        {1, 5, 6},
        {6, 2, 1},
        // 底面
        {2, 6, 7},
        {7, 3, 2},
        // 后面
        {4, 7, 6},
        {6, 5, 4}};

   

    Light l = {
    camera.GetPosition(), 
    oeVec3(0, 10, 0), 
    oeVec3(0, 255, 0), 
    oeVec3(10, 10, 10), 
    oeVec3(5500, 5500, 5500), 
    150
};
    auto ver_id = draw.load_vertex(ver);
    auto ind_id = draw.load_indices(ind);
    
    draw.SetLight(l);
    draw.SetModelMatrix(Transform::Rotation4x4(0, 20, 0));
    draw.SetViewMatrix(camera.GetViewMatrix());
    draw.SetProjMatrix(Transform::get_projection_matrix(45, 1, 0.1, 50));


    draw.draw(ver_id, ind_id, Primitive::Triangle);
    bmp.save("output.bmp");
    return 0;
}