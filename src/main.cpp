#include "bmp.hpp"
#include "Rasterizer.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"
#include "Texture.hpp"
#include "OBJ_Loader.h"
#include <string>


int main()
{   
    Bmp bmp(700,700); 
    Rasterizer draw(&bmp);


    Camera camera(oeVec3(0, 0, 5));
    std::vector<Triangle*> TriangleList;

    float angle = 140.0;
    bool command_line = false;

    std::string filename = "output.bmp";
    objl::Loader Loader;
    
    std::string obj_path = "../models/cube/";
    bool loadout = Loader.LoadFile("../models/cube/cube.obj");

    auto texture_path = "wall1.bmp";
    draw.set_texture(Texture(obj_path + texture_path));

    for(auto mesh:Loader.LoadedMeshes)
    {
        for(int i=0;i<mesh.Vertices.size();i+=3)
        {
            Triangle* t = new Triangle();
            for(int j=0;j<3;j++)
            {
                t->setVertex(j,oeVec4(mesh.Vertices[i+j].Position.X,mesh.Vertices[i+j].Position.Y,mesh.Vertices[i+j].Position.Z,1.0));
                t->setNormal(j,oeVec3(mesh.Vertices[i+j].Normal.X,mesh.Vertices[i+j].Normal.Y,mesh.Vertices[i+j].Normal.Z));
                t->setTexCoord(j,oeVec2(mesh.Vertices[i+j].TextureCoordinate.X, mesh.Vertices[i+j].TextureCoordinate.Y));
            }
            TriangleList.push_back(t);
        }
    }
    
    draw.set_modelMatrix(Transform::Rotation4x4(-45, 30, -90));
    draw.Set_viewMatrix(camera.GetViewMatrix());
    draw.Set_projMatrix(Transform::get_projection_matrix(45, 1, 0.1, 50));


    draw.draw(TriangleList);
    bmp.LoadData(draw.GetFrameBuf());
    bmp.save(filename);
    

    return 0;
}