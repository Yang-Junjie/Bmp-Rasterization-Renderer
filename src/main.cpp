#include "bmp.hpp"
#include "Rasterizer.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"

int main() {
    const real width = 512;
    const real height = 512;
    Bmp bmp(width, height);
    Rasterizer draw(&bmp);
    draw.SetColor(255, 0, 0);

    // 设置相机
    Camera camera(oeVec3(0, 0, 10));

    std::vector<oeVec3> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<oeVec3> ind{{0, 1, 2}};

    auto pos_id = draw.load_positions(pos);
    auto ind_id = draw.load_indices(ind);

    draw.SetModelMatrix(Transform::Rotation4x4(0,0,0));
    draw.SetViewMatrix(camera.GetViewMatrix());
    draw.SetProjMatrix(Transform::get_projection_matrix(45, 1, 0.1, 50));

    draw.draw(pos_id, ind_id, Primitive::Triangle);

    bmp.save("output.bmp");
    return 0;
}