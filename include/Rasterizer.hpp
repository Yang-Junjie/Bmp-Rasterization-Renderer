#ifndef DRAW_HPP
#define DRAW_HPP
#include "bmp.hpp"
#include <tuple>
#include "oe_math.hpp"
#include <map>


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

struct SubSample {
    int r = 0;
    int g = 0;
    int b = 0;
};

class Rasterizer{
private:
    int _x,_y;
    int r,g,b;
    Bmp *m_bmp;
    int m_center_x; 
    int m_center_y; 
    inline std::pair<int, int> ConvertCoordinate(int x_user, int y_user);

    Matrix4x4 modelMatrix = Matrix4x4::identityMatrix();
    Matrix4x4 viewMatrix = Matrix4x4::identityMatrix();
    Matrix4x4 projMatrix = Matrix4x4::identityMatrix();

    std::map<int, std::vector<oeVec3>> pos_buf;
    std::map<int, std::vector<oeVec3>> ind_buf;

    std::vector<oeVec3> frame_buf;
    std::vector<float>  depth_buf;
    int get_index(int x, int y);

    int width, height;

    int next_id = 0;
    int get_next_id() { return next_id++; }

    int msaaFactor; 
    std::vector<std::vector<std::vector<SubSample>>> msaaBuffer;

private:
    void DrawLineBresenham(const oeVec3& bigen , const oeVec3& end);
    void DrawRectangle(int x,int y,int width,int height);
    void DrawTriangle(const oeVec4& veticex1,int r1, int g1, int b1,
                      const oeVec4& veticex2,int r2, int g2, int b2,
                      const oeVec4& veticex3,int r3, int g3, int b3);

public :
    Rasterizer(Bmp *bmp);
    ~Rasterizer();

    void SetColor(int r,int g,int b);
    std::tuple<int,int,int> GetColor();

    pos_buf_id load_positions(const std::vector<oeVec3>& positions);
    ind_buf_id load_indices(const std::vector<oeVec3>& indices);

    void SetModelMatrix(const Matrix4x4& mat) { modelMatrix = mat; }
    void SetViewMatrix(const Matrix4x4& mat) { viewMatrix = mat; }
    void SetProjMatrix(const Matrix4x4& mat) { projMatrix = mat; }

    void SpreadBackground();

    void DrawPoint(int x0, int y0);

    void clear(Buffers buff);
    void resolveMSAA();
    void draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, Primitive type);
   
    
    
   
};
#endif