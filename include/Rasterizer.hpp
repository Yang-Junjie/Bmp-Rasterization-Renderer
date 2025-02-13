#ifndef DRAW_HPP
#define DRAW_HPP

#include "bmp.hpp"
#include <tuple>
#include "oe_math.hpp"
#include <map>
#include <optional>
#include <algorithm>
#include <functional>
#include "Shader.hpp"
#include "Triangle.hpp"

enum class Buffers
{
    Color = 1,
    Depth = 2
};


inline Buffers operator|(Buffers a, Buffers b)
{
    return Buffers((int)a | (int)b);
}

inline Buffers operator&(Buffers a, Buffers b)
{
    return Buffers((int)a & (int)b);
}

enum class Primitive
{
    Line,
    Triangle
};
struct pos_buf_id
{
    int pos_id = 0;
};

struct ind_buf_id
{
    int ind_id = 0;
};

struct col_buf_id
{
    int col_id = 0;
};

struct nor_buf_id{

    int nor_id = 0;
};

struct SubSample
{
    int r = 0;
    int g = 0;
    int b = 0;
};



class Rasterizer
{
private:
    Bmp *m_bmp;

    Matrix4x4 modelMatrix = Matrix4x4::identityMatrix();
    Matrix4x4 viewMatrix = Matrix4x4::identityMatrix();
    Matrix4x4 projMatrix = Matrix4x4::identityMatrix();

    int normal_id = -1;

    std::map<int, std::vector<oeVec3>> pos_buf;
    std::map<int, std::vector<oeVec3>> ind_buf;
    std::map<int, std::vector<oeVec3>> col_buf;
    std::map<int, std::vector<oeVec3>> nor_buf;

    std::optional<Texture> texture;

    std::function<oeVec3(fragment_shader_payload)> fragment_shader;
    std::function<oeVec3(vertex_shader_payload)> vertex_shader;
    

    int width, height;
    int next_id = 0;

    std::vector<oeVec3> frame_buf;
    std::vector<float> depth_buf;

    int get_next_id() { return next_id++; }
    int get_index(int x, int y);
    inline std::pair<int, int> ConvertCoordinate(const real &x_user, const real &y_user);

    void DrawTriangle(const oeVec4 &v1,const oeVec3& n1,const oeVec3& c1,
                      const oeVec4 &v2,const oeVec3& n2,const oeVec3& c2,
                      const oeVec4 &v3,const oeVec3& n3,const oeVec3& c3);

    void rasterize_triangle(const Triangle& t, const std::array<oeVec3, 3>& world_pos);

    void draw_line(oeVec3 begin, oeVec3 end);

public:
    Rasterizer(Bmp *bmp);
    std::vector<oeVec3> GetFrameBuf(){return frame_buf;}
    pos_buf_id load_positions(const std::vector<oeVec3>& positions);
    ind_buf_id load_indices(const std::vector<oeVec3>& indices);
    col_buf_id load_colors(const std::vector<oeVec3>& colors);
    nor_buf_id load_normals(const std::vector<oeVec3>& normals);

    void set_modelMatrix(const Matrix4x4 &mat) { modelMatrix = mat; }
    void Set_viewMatrix(const Matrix4x4 &mat) { viewMatrix = mat; }
    void Set_projMatrix(const Matrix4x4 &mat) { projMatrix = mat; }

    void set_texture(Texture tex) { texture = tex; }

    void set_vertex_shader(std::function<oeVec3(vertex_shader_payload)> vert_shader);
    void set_fragment_shader(std::function<oeVec3(fragment_shader_payload)> frag_shader);
    void set_pixel(const oeVec2 &point, const oeVec3 &color);

    void SpreadBackground(const int &r, const int &g, const int &b);
    
    void draw(pos_buf_id ver_buffer, ind_buf_id ind_buffer, Primitive type);
    void draw(std::vector<Triangle *> &TriangleList);

    void clear(Buffers buff);
};

#endif