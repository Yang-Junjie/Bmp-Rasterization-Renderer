#include "Rasterizer.hpp"
#include "vector2d.hpp"
#include <algorithm>

bool InsideTriangle(oeVec2 A, oeVec2 B, oeVec2 C, oeVec2 P)
{
    auto sgin = [](oeVec2 p1, oeVec2 p2, oeVec2 p3)
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };

    double d1 = sgin(P, A, B);
    double d2 = sgin(P, B, C);
    double d3 = sgin(P, C, A);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

auto to_vec2(const oeVec4 &v3)
{
    return oeVec2(v3.x, v3.y);
}

auto to_vec4(const oeVec3 &v3, real w = 1.0f)
{
    return oeVec4(v3.x, v3.y, v3.z, w);
}

inline std::pair<int, int> Rasterizer::ConvertCoordinate(const real& x_user,const real& y_user)
{

    int x_bmp = static_cast<int>(x_user);
    int y_bmp = static_cast<int>(y_user);
    return {x_bmp, y_bmp};
}

Rasterizer::Rasterizer(Bmp *bmp) : m_bmp(bmp), msaaFactor(4)
{

    width = bmp->GetWidth();
    height = bmp->GetHeight();
    int samples = msaaFactor * msaaFactor;
    frame_buffer.resize(width, std::vector<std::vector<SubSample>>(height, std::vector<SubSample>(samples)));

    depth_buffer.resize(width,
                           std::vector<std::vector<real>>(height,
                                                          std::vector<real>(samples, std::numeric_limits<real>::max())));
}

ver_buf_id Rasterizer::load_vertex(const std::vector<Vertex> &vertex)
{
    auto id = get_next_id();
    ver_buf.emplace(id, vertex);

    return {id};
}


ind_buf_id Rasterizer::load_indices(const std::vector<oeVec3> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

void Rasterizer::SpreadBackground(const int &r,const int &g,const int &b)
{
    for (int y = 0; y < m_bmp->GetHeight(); ++y)
    {
        for (int x = 0; x < m_bmp->GetWidth(); ++x)
        {
            m_bmp->setPixel(x, y, r, g, b);
        }
    }
}



void Rasterizer::resolveMSAA()
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            int total_r = 0, total_g = 0, total_b = 0;
            int samples = msaaFactor * msaaFactor;
            for (int i = 0; i < samples; ++i)
            {
                total_r += frame_buffer[x][y][i].r;
                total_g += frame_buffer[x][y][i].g;
                total_b += frame_buffer[x][y][i].b;
            }
            m_bmp->setPixel(x, y, total_r / samples, total_g / samples, total_b / samples);
        }
    }
}

void Rasterizer::DrawTriangle(const oeVec4 &vertex1, int r1, int g1, int b1,
                              const oeVec4 &vertex2, int r2, int g2, int b2,
                              const oeVec4 &vertex3, int r3, int g3, int b3)
{
    //Triangle Aare
    double denominator = (vertex2.x - vertex1.x) * (vertex3.y - vertex1.y) - (vertex3.x - vertex1.x) * (vertex2.y - vertex1.y);
    if (denominator == 0)
        return;
    
    // Get Bounding Box
    real min_x = std::min({vertex1.x, vertex2.x, vertex3.x});
    real max_x = std::max({vertex1.x, vertex2.x, vertex3.x});
    real min_y = std::min({vertex1.y, vertex2.y, vertex3.y});
    real max_y = std::max({vertex1.y, vertex2.y, vertex3.y});

    int start_x = std::floor(min_x);
    int end_x = std::ceil(max_x);
    int start_y = std::floor(min_y);
    int end_y = std::ceil(max_y);

    for (int x = start_x; x <= end_x; ++x)
    {
        for (int y = start_y; y <= end_y; ++y)
        {
            for (int sx = 0; sx < msaaFactor; ++sx)
            {
                for (int sy = 0; sy < msaaFactor; ++sy)
                {
                    // 计算子样本点位置
                    real px = x + (sx + 0.5) / msaaFactor;
                    real py = y + (sy + 0.5) / msaaFactor;
                    oeVec2 P(px, py);

                    //判断子样本点是否在三角形内
                    if (InsideTriangle(to_vec2(vertex1), to_vec2(vertex2), to_vec2(vertex3), P))
                    {
                        
                        auto TriangleArea = [](oeVec2 p1, oeVec2 p2, oeVec2 p3)
                        {
                            return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
                        };

                        double d1 = TriangleArea(P, to_vec2(vertex1), to_vec2(vertex2));
                        double d2 = TriangleArea(P, to_vec2(vertex2), to_vec2(vertex3));
                        double d3 = TriangleArea(P, to_vec2(vertex3), to_vec2(vertex1));

                        // 判断分母符号，确保面积比例正确
                        if (denominator < 0)
                        {
                            denominator = -denominator;
                            d1 = -d1;
                            d2 = -d2;
                            d3 = -d3;
                        }

                        //计算该子样本点的重心坐标
                        double u = d2 / denominator;
                        double v = d3 / denominator;
                        double w = d1 / denominator;

                        if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
                        {
                            //颜色插值
                            int red = static_cast<int>(u * r1 + v * r2 + w * r3);
                            int green = static_cast<int>(u * g1 + v * g2 + w * g3);
                            int blue = static_cast<int>(u * b1 + v * b2 + w * b3);

                            // 限制颜色范围
                            red = std::clamp(red, 0, 255);
                            green = std::clamp(green, 0, 255);
                            blue = std::clamp(blue, 0, 255);

                            // 深度插值
                            real depth = 1.0 / (d1 / vertex1.w + d2 / vertex2.w + d3 / vertex3.w);
                            real z = (u * vertex1.z + v * vertex2.z + w * vertex3.z);

                            auto [x_bmp, y_bmp] = ConvertCoordinate(px, py);
                            if (x_bmp >= 0 && x_bmp < m_bmp->GetWidth() &&
                                y_bmp >= 0 && y_bmp < m_bmp->GetHeight())
                            {
                                int index = sx * msaaFactor + sy;
                                // 深度测试
                                if (z < depth_buffer[x_bmp][y_bmp][index])
                                {
                                    // 更新颜色和深度
                                    frame_buffer[x_bmp][y_bmp][index] = {red, green, blue};
                                    depth_buffer[x_bmp][y_bmp][index] = z;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Rasterizer::draw(ver_buf_id ver_buffer, ind_buf_id ind_buffer, Primitive type)
{
    //拿到对应id索引的
    auto &ver = ver_buf[ver_buffer.ver_id];
    auto &ind = ind_buf[ind_buffer.ind_id];

    //mvp变换矩阵
    Matrix4x4 mvp = projMatrix.multiply(viewMatrix.multiply(modelMatrix));

    for (auto &i : ind)
    {
        //mvp变换每个点
        oeVec4 v[] = {
            mvp.multiply(to_vec4(ver[i.x].position, 1.0)),
            mvp.multiply(to_vec4(ver[i.y].position, 1.0)),
            mvp.multiply(to_vec4(ver[i.z].position, 1.0))};

        //归一化
        for (auto &vec : v)
        {
            vec /= vec.w;
        }

        //viewport transform
        for (auto &vert : v)
        {
            vert.x = (vert.x + 1.0) * 0.5 * (width - 1);
            vert.y = (1.0 - vert.y) * 0.5 * (height - 1);
            vert.z = 0.5f * vert.z + 0.5;
        }


        DrawTriangle(v[0],ver[i.x].color.x,ver[i.x].color.y,ver[i.x].color.z, v[1],ver[i.y].color.x,ver[i.y].color.y,ver[i.y].color.z , v[2],ver[i.z].color.x,ver[i.z].color.y,ver[i.z].color.z);
    }

    //对所有的数据进行MSAA抗锯齿
    resolveMSAA();
}