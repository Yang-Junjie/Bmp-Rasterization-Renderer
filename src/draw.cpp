#include "draw.hpp"
#include "vector2d.hpp"
#include <algorithm>
bool InsideTriangle(oeVec2 A, oeVec2 B, oeVec2 C, oeVec2 P) {
    auto sign = [](oeVec2 p1, oeVec2 p2, oeVec2 p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };

    double d1 = sign(P, A, B);
    double d2 = sign(P, B, C);
    double d3 = sign(P, C, A);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

inline std::pair<int, int> Draw::ConvertCoordinate(int x_user, int y_user)
{
     
    int x_bmp = x_user + m_center_x;
    int y_bmp = m_center_y - y_user - 1;
    return {x_bmp, y_bmp};
    
}

Draw::Draw(Bmp *bmp) : m_bmp(bmp)
{
    m_center_x = bmp->GetWidth() / 2;
    m_center_y = bmp->GetHeight() / 2;
}

Draw::~Draw()
{
}



void Draw::SetColor(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

std::tuple<int, int, int> Draw::GetColor()
{
    return std::tuple<int, int, int>(this->r,this->g,this->b);
}

void Draw::SpreadBackground()
{
    for (int y = 0; y < m_bmp->GetHeight(); ++y) {
        for (int x = 0; x < m_bmp->GetWidth(); ++x) {
            m_bmp->setPixel(x, y, this->r,this->g,this->b);
        }
    }
}

void Draw::DrawPoint(int x, int y) {
    auto [x_bmp, y_bmp] = ConvertCoordinate(x, y);
    if (x_bmp < 0 || x_bmp >= m_bmp->GetWidth() || 
        y_bmp < 0 || y_bmp >= m_bmp->GetHeight()) {
        return;
    }
    m_bmp->setPixel(x_bmp, y_bmp, this->r, this->g, this->b);
}

void Draw::DrawLineBresenham(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (true) {
        DrawPoint(x0, y0); 
        if (x0 == x1 && y0 == y1) break;
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void Draw::DrawLineDDA(int x0, int y0, int x1, int y1)
{
    int  dx = x1 - x0, dy = y1 - y0, epslion;   
	float  x = x0, y = y0, xIncre, yIncre;
	epslion = abs(dx) > abs(dy) ?  abs(dx) : abs(dy);
	xIncre = (float)(dx) / epslion;
	yIncre = (float)(dy) / epslion;
	
	for (int k = 0; k <= epslion; k++) {
         DrawPoint(x0, y0); 
		x += xIncre;   y += yIncre; 
	}
}

void Draw::DrawRectangle(int x, int y, int width, int height)
{
    for(int i = x;i<=x+width;i++){
        for(int j = y;j<=y+height;j++){
            DrawPoint(x, y); 
        }
    }
}

void Draw::DrawTriangle(const oeVec2& vertex1, int r1, int g1, int b1,
                        const oeVec2& vertex2, int r2, int g2, int b2,
                        const oeVec2& vertex3, int r3, int g3, int b3) {
   

   
    double denominator = (vertex2.x - vertex1.x) * (vertex3.y - vertex1.y) - (vertex3.x - vertex1.x) * (vertex2.y - vertex1.y);
    if (denominator == 0) {
        return; 
    }

    
    int max_x = (vertex1.x > vertex2.x) ?
        ((vertex1.x > vertex2.x) ? vertex1.x : vertex3.x) :
        ((vertex2.x > vertex3.x) ? vertex2.x : vertex3.x);
    int max_y = (vertex1.y > vertex2.y) ? 
        ((vertex1.y > vertex2.y) ? vertex1.y : vertex3.y) :
        ((vertex2.y > vertex3.y) ? vertex2.y : vertex3.y);
    int min_x = (vertex1.x < vertex2.x) ? 
        ((vertex1.x < vertex3.x) ? vertex1.x : vertex3.x) : 
        ((vertex2.x < vertex3.x) ? vertex2.x : vertex3.x);
    int min_y = (vertex1.y < vertex2.y) ? 
        ((vertex1.y < vertex3.y) ? vertex1.y : vertex3.y) : 
        ((vertex2.y < vertex3.y) ? vertex2.y : vertex3.y);
   

    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            oeVec2 P(x, y);
            if (InsideTriangle(vertex1, vertex2, vertex3, P)) {
                auto sign = [](oeVec2 p1, oeVec2 p2, oeVec2 p3) {
                    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
                };
                double d1 = sign(P, vertex1, vertex2);
                double d2 = sign(P, vertex2, vertex3);
                double d3 = sign(P, vertex3, vertex1);

                // 计算重心坐标
                double u = d2 / denominator;
                double v = d3 / denominator;
                double w = d1 / denominator;

                // 颜色插值
                int red = static_cast<int>(u * r1 + v * r2 + w * r3);
                int green = static_cast<int>(u * g1 + v * g2 + w * g3);
                int blue = static_cast<int>(u * b1 + v * b2 + w * b3);

                // 限制颜色范围
                red = std::clamp(red, 0, 255);
                green = std::clamp(green, 0, 255);
                blue = std::clamp(blue, 0, 255);

                // 转换坐标并设置像素颜色
                auto [x_bmp, y_bmp] = ConvertCoordinate(x, y);
                if (x_bmp >= 0 && x_bmp < m_bmp->GetWidth() &&
                    y_bmp >= 0 && y_bmp < m_bmp->GetHeight()) {
                    m_bmp->setPixel(x_bmp, y_bmp, red, green, blue);
                }
            }
        }
    }
}

void Draw::DrawPolygon(oeVec2 vertices[], size_t vertices_num, size_t indices[], size_t indices_num) {
    

    for (size_t i = 0; i + 2 < indices_num; i += 3) {
       
        const size_t idx0 = indices[i];
        const size_t idx1 = indices[i+1];
        const size_t idx2 = indices[i+2];
        
       
        if (idx0 >= vertices_num || idx1 >= vertices_num || idx2 >= vertices_num) {
            continue; 
        }
        const oeVec2& v0 = vertices[idx0];
        const oeVec2& v1 = vertices[idx1];
        const oeVec2& v2 = vertices[idx2];
        
        DrawTriangle(v0,r,g,b ,v1,r,g,b ,v2,r,g,b);
    }
}