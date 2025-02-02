#include "bmp.hpp"
#include "Rasterizer.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"

int main() {
    const real width = 1080;
    const real height = 1080;
    Bmp bmp(width, height);
    Rasterizer draw(&bmp);
    draw.SetColor(255, 0, 0);

   
    Camera camera(oeVec3(0, 0, 5));

        std::vector<oeVec3> pos{
            // 前面
            { 1, 1, -1}, // 0
            {-1, 1, -1}, // 1
            {-1,-1, -1}, // 2
            { 1,-1, -1}, // 3
            // 后面
            { 1, 1, 1},  // 4
            {-1, 1, 1},  // 5
            {-1,-1, 1},  // 6
            { 1,-1, 1}   // 7
        };

        // 每个面由两个三角形组成
        std::vector<oeVec3> ind{
            // 前面
            {0,1,2}, {2,3,0},
            // 右面
            {3,7,4}, {4,0,3},
            // 顶面
            {0,4,5}, {5,1,0},
            // 左面
            {1,5,6}, {6,2,1},
            // 底面
            {2,6,7}, {7,3,2},
            // 后面
            {4,7,6}, {6,5,4}
        };
////
    auto pos_id = draw.load_positions(pos);
    auto ind_id = draw.load_indices(ind);

    draw.SetModelMatrix(Transform::Rotation4x4(0,45,0));
    draw.SetViewMatrix(camera.GetViewMatrix());
    draw.SetProjMatrix(Transform::get_projection_matrix(45, 1, 0.1, 50));

    draw.draw(pos_id, ind_id, Primitive::Triangle);

    bmp.save("output.bmp");
    return 0;
}