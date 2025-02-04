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

auto to_vec3(const oeVec4 &v3)
{
    return oeVec3(v3.x, v3.y, v3.z);
}

inline std::pair<int, int> Rasterizer::ConvertCoordinate(const real &x_user, const real &y_user)
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

void Rasterizer::SpreadBackground(const int &r, const int &g, const int &b)
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


// 在Rasterizer类内添加
oeVec3 phong_shader(const oeVec3& point, const oeVec3& normal,
                   const oeVec3& eye_pos, const oeVec3& light_pos,
                   const oeVec3& light_color, const oeVec3& ambient,
                   const oeVec3& intensity, real shininess,oeVec3 color) {
        oeVec3 result_color = {0, 0, 0};
        oeVec3 ka = oeVec3(0.005, 0.005, 0.005);
        oeVec3 kd = color;
        oeVec3 ks = oeVec3(0.7937, 0.7937, 0.7937);
        auto v = eye_pos - point; //v为出射光方向（指向眼睛）
        auto l = light_pos - point; //l为指向入射光源方向
        auto h = (v + l).normalize(); //h为半程向量即v+l归一化后的单位向量
        auto r = l.dot(l); //衰减因子
        auto diffuse = kd.cwiseProduct(intensity / r) * std::max(0.0, normal.normalize().dot(-l.normalize()));
        auto specular = ks.cwiseProduct(intensity / r) * std::pow(std::max(0.0, normal.normalize().dot(-h)), 150);
        result_color += (ka.cwiseProduct(ambient) + diffuse + specular);
       // std::cout<<std::max(0.0, normal.normalize().dot(-h))<<std::endl;
    return result_color * 255;

    
}

void Rasterizer::DrawTriangle(const oeVec4 &v1,const oeVec3& n1,const oeVec3& c1,
                      const oeVec4 &v2,const oeVec3& n2,const oeVec3& c2,
                      const oeVec4 &v3,const oeVec3& n3,const oeVec3& c3)
{
    // Triangle Aare
    double denominator = (v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y);
    if (denominator == 0)
        return;

    // Get Bounding Box
    real min_x = std::min({v1.x, v2.x, v3.x});
    real max_x = std::max({v1.x, v2.x, v3.x});
    real min_y = std::min({v1.y, v2.y, v3.y});
    real max_y = std::max({v1.y, v2.y, v3.y});

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

                    // 判断子样本点是否在三角形内
                    if (InsideTriangle(to_vec2(v1), to_vec2(v2), to_vec2(v3), P))
                    {

                        auto TriangleArea = [](oeVec2 p1, oeVec2 p2, oeVec2 p3)
                        {
                            return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
                        };

                        double d1 = TriangleArea(P, to_vec2(v1), to_vec2(v2));
                        double d2 = TriangleArea(P, to_vec2(v2), to_vec2(v3));
                        double d3 = TriangleArea(P, to_vec2(v3), to_vec2(v1));

                        // 判断分母符号，确保面积比例正确
                        if (denominator < 0)
                        {
                            denominator = -denominator;
                            d1 = -d1;
                            d2 = -d2;
                            d3 = -d3;
                        }

                        // 计算该子样本点的重心坐标
                        double u = d2 / denominator;
                        double v = d3 / denominator;
                        double w = d1 / denominator;

                        if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
                        {


                            //颜色插值
                            real red = static_cast<int>(u * c1.x + v * c2.x + w * c3.x);
                            real green = static_cast<int>(u * c1.y  + v * c2.y + w * c3.y);
                            real blue = static_cast<int>(u * c1.z  + v * c2.z + w * c3.z);
                            oeVec3 color1 = {red,green ,blue};
                           oeVec3 frag_pos = {
                            u * v1.x + v * v2.x + w * v3.x,
                            u * v1.y + v * v2.y + w * v3.y,
                            u * v1.z + v * v2.z + w * v3.z
                        };
                        
                        oeVec3 interp_normal = (n1 * u + n2 * v + n3 * w).normalize();
                        
                        // 计算Phong光照
                        oeVec3 color = phong_shader(
                            frag_pos,
                            interp_normal,
                            light.viewPostion, // 需要添加获取相机位置的方法
                            light.lightPos,
                            light.lightColor,
                            light.ambientColor,
                            light.intensity,
                            light.shininess,
                            color1
                        );
                        //std::cout<<color<<std::endl;
                        // 设置颜色
                        
                        int r = std::clamp(static_cast<int>(color.x), 0, 255);
                        int g = std::clamp(static_cast<int>(color.y), 0, 255);
                        int b = std::clamp(static_cast<int>(color.z), 0, 255);
                       // std::cout<<color<<std::endl;
                            // 深度插值
                            real depth = 1.0 / (d1 / v1.w + d2 / v2.w + d3 / v3.w);
                            real z = (u * v1.z + v * v2.z + w * v3.z);

                            auto [x_bmp, y_bmp] = ConvertCoordinate(px, py);
                            if (x_bmp >= 0 && x_bmp < m_bmp->GetWidth() &&
                                y_bmp >= 0 && y_bmp < m_bmp->GetHeight())
                            {
                                int index = sx * msaaFactor + sy;
                                // 深度测试
                                if (z < depth_buffer[x_bmp][y_bmp][index])
                                {
                                    // 更新颜色和深度
                                    frame_buffer[x_bmp][y_bmp][index] = {r, g, b};
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
    // 拿到对应id索引的
    auto &ver = ver_buf[ver_buffer.ver_id];
    auto &ind = ind_buf[ind_buffer.ind_id];

    // mvp变换矩阵
    Matrix4x4 mvp = projMatrix.multiply(viewMatrix.multiply(modelMatrix));

    for (auto &i : ind)
    {
        // mvp变换每个点
        oeVec4 v[] = {
            mvp.multiply(to_vec4(ver[i.x].position, 1.0)),
            mvp.multiply(to_vec4(ver[i.y].position, 1.0)),
            mvp.multiply(to_vec4(ver[i.z].position, 1.0))};

        // 新增：法线变换（使用模型矩阵的逆转置）
        Matrix4x4 normalMatrix = modelMatrix.invert().transpose();
        oeVec3 n[] = {
               to_vec3( normalMatrix.multiply(to_vec4( ver[i.x].normal)).normalize()),
               to_vec3( normalMatrix.multiply(to_vec4( ver[i.y].normal)).normalize()),
               to_vec3( normalMatrix.multiply(to_vec4( ver[i.z].normal)).normalize())};

        // 归一化
        for (auto &vec : v)
        {
            vec /= vec.w;
        }

        // viewport transform
        for (auto &vert : v)
        {
            vert.x = (vert.x + 1.0) * 0.5 * (width - 1);
            vert.y = (1.0 - vert.y) * 0.5 * (height - 1);
            vert.z = 0.5f * vert.z + 0.5;
        }

        DrawTriangle(v[0], n[0],ver[i.x].color, v[1],n[1], ver[i.y].color, v[2],n[2], ver[i.z].color);
    }

    // 对所有的数据进行MSAA抗锯齿
    resolveMSAA();
}