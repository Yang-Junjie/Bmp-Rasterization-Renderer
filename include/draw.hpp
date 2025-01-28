#ifndef DRAW_HPP
#define DRAW_HPP
#include "bmp.hpp"
#include <tuple>
#include "oe_math.hpp"

class Draw{
private:
    int _x,_y;
    int r,g,b;
    Bmp *m_bmp;
public :
    Draw(Bmp *bmp);
    ~Draw();

    void SetPosition(int x,int y);
    std::tuple<int,int> GetPosition();

    void SetColor(int r,int g,int b);
    std::tuple<int,int,int> GetColor();

    void SpreadBackground();
    void DrawPoint(int x0, int y0);
    void DrawLineBresenham(int x0,int y0,int x1,int y1);
    void DrawLineDDA(int x0,int y0,int x1,int y1);
  
  
    void DrawRectangle(int x,int y,int width,int height);
    
    void DrawTriangle(const oeVec2& veticex1,const oeVec2& veticex2,const oeVec2& veticex3);
    void DrawPolygon(oeVec2 vertices[], size_t vertices_num,size_t index[],size_t index_num);
};
#endif