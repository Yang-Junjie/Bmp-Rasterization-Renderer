#ifndef DRAW_HPP
#define DRAW_HPP


#include "bmp.hpp"
#include <tuple>
#include "oe_math.hpp"
#include <map>
#include <optional>
#include <algorithm>

enum class Buffers
{
    Color = 1,
    Depth = 2
};

enum class Primitive
{
    Line,
    Triangle
};

struct ver_buf_id
{
    int ver_id = 0;
};

struct ind_buf_id
{
    int ind_id = 0;
};

struct SubSample
{
    int r = 0;
    int g = 0;
    int b = 0;
};

struct Vertex{
    oeVec3 position; 
    oeVec3 color;    
};

class Rasterizer
{
private:
    Bmp *m_bmp;

    Matrix4x4 modelMatrix = Matrix4x4::identityMatrix();
    Matrix4x4 viewMatrix = Matrix4x4::identityMatrix();
    Matrix4x4 projMatrix = Matrix4x4::identityMatrix();

    std::map<int, std::vector<Vertex>> ver_buf;
    std::map<int, std::vector<oeVec3>> ind_buf;

    int width, height;

    int next_id = 0;

    int msaaFactor;
    std::vector<std::vector<std::vector<SubSample>>> frame_buffer;
    std::vector<std::vector<std::vector<real>>> depth_buffer;

private:
    int get_next_id() { return next_id++; }
    inline std::pair<int, int> ConvertCoordinate(const real &x_user, const real &y_user);
    void DrawTriangle(const oeVec4 &veticex1, int r1, int g1, int b1,
                      const oeVec4 &veticex2, int r2, int g2, int b2,
                      const oeVec4 &veticex3, int r3, int g3, int b3);
    void resolveMSAA();

public:
    Rasterizer(Bmp *bmp);

    ver_buf_id load_vertex(const std::vector<Vertex> &Vertex);
    ind_buf_id load_indices(const std::vector<oeVec3> &indices);

    void SetModelMatrix(const Matrix4x4 &mat) { modelMatrix = mat; }
    void SetViewMatrix(const Matrix4x4 &mat) { viewMatrix = mat; }
    void SetProjMatrix(const Matrix4x4 &mat) { projMatrix = mat; }

    void SpreadBackground(const int &r, const int &g, const int &b);
    void draw(ver_buf_id ver_buffer, ind_buf_id ind_buffer,Primitive type);
};

#endif