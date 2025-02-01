#include "bmp.hpp"
#include "Rasterizer.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"

int main() {
    const real width = 516;
    const real height = 516;
    Bmp bmp(width, height);
    Rasterizer draw(&bmp);
    draw.SetColor(255, 0, 0);

   
    Camera camera(oeVec3(0, 0, 5));

    std::vector<oeVec3> pos{{2, 0, -2},
                    {0, 2, -2},
                    {-2, 0, -2},
                    {3.5, -1, -5},
                    {2.5, 1.5, -5},
                    {-1, 0.5, -5}};

    std::vector<oeVec3> ind{{0, 1, 2},
                    {3, 4, 5}};

    auto pos_id = draw.load_positions(pos);
    auto ind_id = draw.load_indices(ind);

    draw.SetModelMatrix(Transform::Rotation4x4(0,0,0));
    draw.SetViewMatrix(camera.GetViewMatrix());
    draw.SetProjMatrix(Transform::get_projection_matrix(45, 1, 0.1, 50));

    draw.draw(pos_id, ind_id, Primitive::Triangle);

    bmp.save("output.bmp");
    return 0;
}