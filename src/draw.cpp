#include "draw.hpp"
#include "vector2d.hpp"

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

Draw::Draw(Bmp *bmp):m_bmp(bmp)
{
}

Draw::~Draw()
{
}

void Draw::SetPosition(int x, int y)
{
    this->_x = x;
    this->_y = y;
}

std::tuple<int, int> Draw::GetPosition()
{
    return std::tuple<int, int>(this->_x,this->_y);
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

void Draw::DrawPoint(int x,int y)
{
    
    m_bmp->setPixel(x, y, this->r,this->g,this->b);  
}

void Draw::DrawLineBresenham(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int erro = (dx > dy ? dx : -dy) / 2;
 
    while(m_bmp->setPixel(x0,y0,this->r,this->g,this->b), x0 != x1 || y0 != y1){
        int e2 = erro;
        if(e2 > -dx) { erro -= dy; x0 += sx;}
        if(e2 <  dy) { erro += dx; y0 += sy;}
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
        m_bmp->setPixel(x,y,this->r,this->g,this->b);
		x += xIncre;   y += yIncre; 
	}
}

void Draw::DrawRectangle(int x, int y, int width, int height)
{
    for(int i = x;i<=x+width;i++){
        for(int j = y;j<=y+height;j++){
             m_bmp->setPixel(i,j,this->r,this->g,this->b);
        }
    }
}

void Draw::DrawTriangle(const oeVec2& vertex1,const oeVec2& vertex2,const oeVec2& vertex3)
{
    int max_x = (vertex1.x > vertex2.x) ? ((vertex1.x > vertex2.x) ? vertex1.x : vertex3.x) : ((vertex2.x > vertex3.x) ? vertex2.x : vertex3.x);
    int max_y = (vertex1.y > vertex2.y) ? ((vertex1.y > vertex2.y) ? vertex1.y : vertex3.y) : ((vertex2.y > vertex3.y) ? vertex2.y : vertex3.y);

   
    int min_x = (vertex1.x < vertex2.x) ? 
        ((vertex1.x < vertex3.x) ? vertex1.x : vertex3.x) : 
        ((vertex2.x < vertex3.x) ? vertex2.x : vertex3.x);

    
    int min_y = (vertex1.y < vertex2.y) ? 
        ((vertex1.y < vertex3.y) ? vertex1.y : vertex3.y) : 
        ((vertex2.y < vertex3.y) ? vertex2.y : vertex3.y);

    for(int x = min_x;x<=max_x;x ++){
        for(int y = min_y;y<=max_y;y ++){
        if(InsideTriangle(vertex1,vertex2,vertex3,oeVec2(x,y))){
            m_bmp->setPixel(x,y,this->r,this->g,this->b);
        };
        }
    }
}
