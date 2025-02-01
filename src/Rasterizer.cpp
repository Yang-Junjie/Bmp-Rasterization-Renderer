#include "Rasterizer.hpp"
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
auto to_vec2(const oeVec4& v3)
{
    return oeVec2(v3.x, v3.y);
}

auto to_vec4(const oeVec3& v3, float w = 1.0f)
{
    return oeVec4(v3.x, v3.y, v3.z, w);
}

inline std::pair<int, int> Rasterizer::ConvertCoordinate(int x_user, int y_user)
{
     
    int x_bmp = x_user + m_center_x;
    int y_bmp = m_center_y - y_user - 1;
    return {x_bmp, y_bmp};
    
}

Rasterizer::Rasterizer(Bmp *bmp) : m_bmp(bmp), msaaFactor(4)
{
    m_center_x = bmp->GetWidth() / 2;
    m_center_y = bmp->GetHeight() / 2;
    width = bmp->GetWidth();
    height = bmp->GetHeight();
    int samples = msaaFactor * msaaFactor;
    msaaBuffer.resize(width, std::vector<std::vector<SubSample>>(height, std::vector<SubSample>(samples)));
}

Rasterizer::~Rasterizer()
{
}



void Rasterizer::SetColor(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

std::tuple<int, int, int> Rasterizer::GetColor()
{
    return std::tuple<int, int, int>(this->r,this->g,this->b);
}

pos_buf_id Rasterizer::load_positions(const std::vector<oeVec3> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

ind_buf_id Rasterizer::load_indices(const std::vector<oeVec3> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

void Rasterizer::SpreadBackground()
{
    for (int y = 0; y < m_bmp->GetHeight(); ++y) {
        for (int x = 0; x < m_bmp->GetWidth(); ++x) {
            m_bmp->setPixel(x, y, this->r,this->g,this->b);
        }
    }
}

void Rasterizer::DrawPoint(int x, int y) {
    auto [x_bmp, y_bmp] = ConvertCoordinate(x, y);
    if (x_bmp < 0 || x_bmp >= m_bmp->GetWidth() || 
        y_bmp < 0 || y_bmp >= m_bmp->GetHeight()) {
        return;
    }
    m_bmp->setPixel(x_bmp, y_bmp, this->r, this->g, this->b);
}

void Rasterizer::clear(Buffers buff)
{
    if ((buff & Buffers::Color) == Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), oeVec3{0, 0, 0});
    }
    if ((buff & Buffers::Depth) == Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

void Rasterizer::resolveMSAA() {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                int total_r = 0, total_g = 0, total_b = 0;
                int samples = msaaFactor * msaaFactor;
                for (int i = 0; i < samples; ++i) {
                    total_r += msaaBuffer[x][y][i].r;
                    total_g += msaaBuffer[x][y][i].g;
                    total_b += msaaBuffer[x][y][i].b;
                }
                m_bmp->setPixel(x, y, total_r/samples, total_g/samples, total_b/samples);
            }
        }
}

int Rasterizer::get_index(int x, int y)
{
     return (height-y)*width + x;
}

void Rasterizer::DrawLineBresenham(const oeVec3 &begin, const oeVec3 &end)
{
    int x0 = begin.x;
    int y0 = begin.y;
    int x1 = end.x;
    int y1 = end.y;

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


void Rasterizer::DrawRectangle(int x, int y, int width, int height)
{
    for(int i = x;i<=x+width;i++){
        for(int j = y;j<=y+height;j++){
            DrawPoint(x, y); 
        }
    }
}

void Rasterizer::DrawTriangle(const oeVec4& vertex1, int r1, int g1, int b1,
                        const oeVec4& vertex2, int r2, int g2, int b2,
                        const oeVec4& vertex3, int r3, int g3, int b3) {
   

   
   double denominator = (vertex2.x - vertex1.x) * (vertex3.y - vertex1.y) - (vertex3.x - vertex1.x) * (vertex2.y - vertex1.y);
    if (denominator == 0) return;

    
    float min_x = std::min({vertex1.x, vertex2.x, vertex3.x});
    float max_x = std::max({vertex1.x, vertex2.x, vertex3.x});
    float min_y = std::min({vertex1.y, vertex2.y, vertex3.y});
    float max_y = std::max({vertex1.y, vertex2.y, vertex3.y});

    int start_x = std::floor(min_x);
    int end_x = std::ceil(max_x);
    int start_y = std::floor(min_y);
    int end_y = std::ceil(max_y);

    for (int x = start_x; x <= end_x; ++x) {
        for (int y = start_y; y <= end_y; ++y) {
            for (int sx = 0; sx < msaaFactor; ++sx) {
                for (int sy = 0; sy < msaaFactor; ++sy) {
                    // 计算子样本点位置
                    float px = x + (sx + 0.5f) / msaaFactor;
                    float py = y + (sy + 0.5f) / msaaFactor;
                    oeVec2 P(px, py);
                    if (InsideTriangle(to_vec2(vertex1), to_vec2(vertex2), to_vec2(vertex3), P)) {
                        auto sign = [](oeVec2 p1, oeVec2 p2, oeVec2 p3) {
                            return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
                        };

                        double d1 = sign(P, to_vec2(vertex1), to_vec2(vertex2));
                        double d2 = sign(P, to_vec2(vertex2), to_vec2(vertex3));
                        double d3 = sign(P, to_vec2(vertex3), to_vec2(vertex1));

                        // 判断分母符号，确保面积比例正确
                        if (denominator < 0) {
                            denominator = -denominator;
                            d1 = -d1;
                            d2 = -d2;
                            d3 = -d3;
                        }

                        double u = d2 / denominator;
                        double v = d3 / denominator;
                        double w = d1 / denominator;

                        if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1) {
                            int red = static_cast<int>(u * r1 + v * r2 + w * r3);
                            int green = static_cast<int>(u * g1 + v * g2 + w * g3);
                            int blue = static_cast<int>(u * b1 + v * b2 + w * b3);

                            // 限制颜色范围
                            red = std::clamp(red, 0, 255);
                            green = std::clamp(green, 0, 255);
                            blue = std::clamp(blue, 0, 255);
                            auto [x_bmp, y_bmp] = ConvertCoordinate(px, py);
                        if (x_bmp >= 0 && x_bmp < m_bmp->GetWidth() &&
                            y_bmp >= 0 && y_bmp < m_bmp->GetHeight()) {
                            int index = sx * msaaFactor + sy;
                            msaaBuffer[x_bmp][y_bmp][index] = {red, green, blue};
                        }
                        //     auto [x_bmp, y_bmp] = ConvertCoordinate(x, y);
                        // if (x_bmp >= 0 && x_bmp < m_bmp->GetWidth() &&
                        //     y_bmp >= 0 && y_bmp < m_bmp->GetHeight()) {
                        //     m_bmp->setPixel(x_bmp, y_bmp, red, green, blue);
                        // }
                    }
                    }
                }
            }
        }
    }
}

void Rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, Primitive type)
{

    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];


    Matrix4x4 mvp = projMatrix.multiply(viewMatrix.multiply(modelMatrix)); 
    for (auto& i : ind) {
        oeVec4 v[] = {
            mvp.multiply(to_vec4(buf[i.x], 1.0f)),
            mvp.multiply(to_vec4(buf[i.y], 1.0f)),
            mvp.multiply(to_vec4(buf[i.z], 1.0f))  
        };

        for (auto& vec : v) {
            vec /= vec.w;
        }
        for (auto & vert : v)
        {
            vert.x = 0.5*width*(vert.x);
            vert.y = 0.5*height*(vert.y);
            vert.z = vert.z;
        }
        DrawTriangle(v[0],255,0,0,v[1],0,255,0,v[2],0,0,255);
        resolveMSAA();
    }
}