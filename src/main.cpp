#include "bmp.hpp"
#include "Rasterizer.hpp"
#include <iostream>
#include "oe_math.hpp"
#include "Transfrom.hpp"
#include "camera.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "OBJ_Loader.h"
#include <string>



Matrix4x4 get_view_matrix(oeVec3 eye_pos)
{
    Matrix4x4 view = Matrix4x4::identityMatrix();

    Matrix4x4 translate;
    translate={ {1,0,0,-eye_pos.x},
                { 0,1,0,-eye_pos.y},
                { 0,0,1,-eye_pos.z},
                { 0,0,0,1}};

    view = translate.multiply(view);

    return view;
}

Matrix4x4 get_model_matrix(float angle,float scalecoef)
{
    Matrix4x4 rotation;
    angle = angle * Constant::Pi / 180.f;
    rotation ={{ cos(angle), 0, sin(angle), 0},
               { 0, 1, 0, 0},
               { -sin(angle), 0, cos(angle), 0},
               { 0, 0, 0, 1}};

    Matrix4x4 scale;
    scale ={{scalecoef, 0, 0, 0},
            {  0, scalecoef, 0, 0},
            {  0, 0, scalecoef, 0},
            {  0, 0, 0, 1}};

    Matrix4x4 translate;
    translate ={{ 1, 0, 0, 0},
           { 0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}};

    return scale.multiply( translate.multiply(rotation));
}

Matrix4x4 get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    
    Matrix4x4 projection = Matrix4x4::identityMatrix();
    
    float eye_angle = eye_fov *Constant::Pi / 180;
    float t,b,l,r;
    t = - zNear * tan(eye_angle /2);//------
    r = t * aspect_ratio;
    l = -r;
    b = -t;
   
    Matrix4x4 m1;
    Matrix4x4 m2;
    Matrix4x4 m3;
    m1={{ zNear,0,0,0},
        {0,zNear,0,0},
        {0,0,zNear + zFar,-zNear*zFar},
        {0,0,1,0}};
    m2={{1,0,0,0},
        {0,1,0,0},
        {0,0,1,-(zNear+ zFar)/2},
        {0,0,0,1}};
    m3={{2/(r-l),0,0,0},
        {0,2/(t-b),0,0},
        {0,0,2/(zNear -zFar),0},
        {0,0,0,1}};
    projection = projection.multiply(m1.multiply(m3.multiply(m2)));
    return projection;

}


struct light
{
    oeVec3 position;
    oeVec3 intensity;
};

oeVec3 vertex_shader(const vertex_shader_payload& payload)
{
    return payload.position;
}

oeVec3 phong_fragment_shader(const fragment_shader_payload& payload)
{
    oeVec3 ka = oeVec3(0.005, 0.005, 0.005);
    oeVec3 kd = payload.color;
    oeVec3 ks = oeVec3(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    oeVec3 amb_light_intensity{10, 10, 10};
    oeVec3 eye_pos{0, 0, 10};

    float p = 150;

    oeVec3 color = payload.color;
    oeVec3 point = payload.view_pos;
    oeVec3 normal = payload.normal;

    oeVec3 result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        
        float r2 = (light.position - point).dot((light.position - point));
        oeVec3 l = (light.position - point).normalize();
        oeVec3 n = normal.normalize();
        oeVec3 v = (eye_pos - point).normalize();
        oeVec3 h = (l+v).normalize();
        oeVec3 ambient = ka * amb_light_intensity.x;
        oeVec3 diffuse =  (kd * light.intensity.x/r2) * std::max(0.0,n.dot(l));
        oeVec3 specular = (ks * light.intensity.x/r2) * std::pow(std::max(0.0,(n.dot(h))),p);

        result_color+=(ambient+diffuse+specular);

    }

    return result_color * 255.f;
}

int main()
{   
    Bmp bmp(700,700);
    std::vector<Triangle*> TriangleList;

    float angle = 140.0;
    bool command_line = false;

    std::string filename = "output.bmp";
    objl::Loader Loader;
    
    std::string obj_path = "../models/cube/";
    bool loadout = Loader.LoadFile("../models/cube/cube.obj");


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

    Rasterizer r(&bmp);

    auto texture_path = "wall1.bmp";
    
    r.set_texture(Texture(obj_path + texture_path));

    std::function<oeVec3(fragment_shader_payload)> active_shader = phong_fragment_shader;
   
    oeVec3 eye_pos = {0,0,10};

    r.set_vertex_shader(vertex_shader);
    r.set_fragment_shader(active_shader);

    int key = 0;
    int frame_count = 0;

    r.clear(Buffers::Color | Buffers::Depth);
   
    r.set_modelMatrix(get_model_matrix(angle,2.5));
    r.Set_viewMatrix(get_view_matrix(eye_pos));
    r.Set_projMatrix(get_projection_matrix(45.0, 1, 0.1, 50));
    
    r.draw(TriangleList);
   
    bmp.LoadData(r.GetFrameBuf());
    bmp.save(filename);
    

    return 0;
}