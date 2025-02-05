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
oeVec3 phong_shader(const oeVec3 &point, const oeVec3 &normal,
                    const oeVec3 &eye_pos, const oeVec3 &light_pos,
                    const oeVec3 &light_color, const oeVec3 &ambient,
                    const oeVec3 &intensity, real shininess, oeVec3 color)
{
    oeVec3 result_color = {0, 0, 0};
    oeVec3 ka = oeVec3(0.005, 0.005, 0.005);
    oeVec3 kd = color;
    oeVec3 ks = oeVec3(0.7937, 0.7937, 0.7937);

    oeVec3 v = (eye_pos - point).normalize();
    oeVec3 l = (light_pos - point).normalize();
    oeVec3 h = (v + l).normalize();

    real r = (light_pos - point).dot(light_pos - point);

    real diffuse_factor = std::max(0.0, normal.normalize().dot(l));
    oeVec3 diffuse = kd.cwiseProduct(intensity / r) * diffuse_factor;

    real specular_factor = std::pow(std::max(0.0, normal.normalize().dot(h)), shininess);
    oeVec3 specular = ks.cwiseProduct(intensity / r) * specular_factor;

    result_color += (ka.cwiseProduct(ambient) + specular + diffuse);
    return result_color * 255;
}

static oeVec3 interpolate(float alpha, float beta, float gamma, const oeVec3 &vert1, const oeVec3 &vert2, const oeVec3 &vert3, float weight)
{
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

static oeVec2 interpolate(float alpha, float beta, float gamma, const oeVec2 &vert1, const oeVec2 &vert2, const oeVec2 &vert3, float weight)
{
    auto u = (alpha * vert1.x + beta * vert2.x + gamma * vert3.x);
    auto v = (alpha * vert1.y + beta * vert2.y + gamma * vert3.y);

    u /= weight;
    v /= weight;

    return oeVec2(u, v);
}

void Rasterizer::DrawTriangle(const oeVec4 &v1, const oeVec3 &n1, const oeVec3 &c1,
                              const oeVec4 &v2, const oeVec3 &n2, const oeVec3 &c2,
                              const oeVec4 &v3, const oeVec3 &n3, const oeVec3 &c3)
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

                            // std::cout<<color<<std::endl;
                            // 深度插值
                            real depth = 1.0 / (d1 / v1.w + d2 / v2.w + d3 / v3.w);
                            real z = (u * v1.z / v1.w + v * v2.z / v2.w + w * v3.z / v3.w);
                            z *= depth;

                            auto [x_bmp, y_bmp] = ConvertCoordinate(px, py);
                            if (x_bmp >= 0 && x_bmp < m_bmp->GetWidth() &&
                                y_bmp >= 0 && y_bmp < m_bmp->GetHeight())
                            {
                                int index = sx * msaaFactor + sy;
                                // 深度测试
                                if (z < depth_buffer[x_bmp][y_bmp][index])
                                {

                                    // 颜色插值
                                    real red = static_cast<int>(u * c1.x + v * c2.x + w * c3.x);
                                    real green = static_cast<int>(u * c1.y + v * c2.y + w * c3.y);
                                    real blue = static_cast<int>(u * c1.z + v * c2.z + w * c3.z);
                                    oeVec3 color1 = {red, green, blue};
                                    oeVec3 frag_pos = {
                                        u * v1.x + v * v2.x + w * v3.x,
                                        u * v1.y + v * v2.y + w * v3.y,
                                        u * v1.z + v * v2.z + w * v3.z};

                                    oeVec3 interp_normal = (n1 * u + n2 * v + n3 * w).normalize();
                                 //   auto interp_view_pos = interpolate(u,v,w,viewpos[0],viewpos[1],viewpos[2],1);

                                    // 计算Phong光照
                                    // oeVec3 color = phong_shader(
                                    //     frag_pos,
                                    //     interp_normal,
                                    //     light.viewPostion, // 需要添加获取相机位置的方法
                                    //     light.lightPos,
                                    //     light.lightColor,
                                    //     light.ambientColor,
                                    //     light.intensity,
                                    //     light.shininess,
                                    //     color1);
                                    // std::cout<<color<<std::endl;
                                    //  设置颜色

                                    int r = std::clamp(static_cast<int>(color1.x), 0, 255);
                                    int g = std::clamp(static_cast<int>(color1.y), 0, 255);
                                    int b = std::clamp(static_cast<int>(color1.z), 0, 255);

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
    real f1 = (50 - 0.1) / 2.0;
    real f2 = (50 + 0.1) / 2.0;
    // mvp变换矩阵
    Matrix4x4 mvp = projMatrix.multiply(viewMatrix.multiply(modelMatrix));
    Matrix4x4 mv = viewMatrix.multiply(modelMatrix);

    for (auto &i : ind)
    {
        // 计算view * model变换后的顶点
        oeVec4 mm[] = {
            viewMatrix.multiply(modelMatrix.multiply(to_vec4(ver[i.x].position, 1.0))),
            viewMatrix.multiply(modelMatrix.multiply(to_vec4(ver[i.y].position, 1.0))),
            viewMatrix.multiply(modelMatrix.multiply(to_vec4(ver[i.z].position, 1.0)))};

        // 转换为视图空间坐标（取前三个分量）
        oeVec3 viewspace_pos[3];
        for (int j = 0; j < 3; ++j)
        {
            viewspace_pos[j] = oeVec3(mm[j].x, mm[j].y, mm[j].z); // 或用to_vec3(mm[j])
        }

        // MVP变换
        oeVec4 v[] = {
            mvp.multiply(to_vec4(ver[i.x].position, 1.0)),
            mvp.multiply(to_vec4(ver[i.y].position, 1.0)),
            mvp.multiply(to_vec4(ver[i.z].position, 1.0))};

        // 齐次除法（仅修改x,y,z分量）
        for (auto &vec : v)
        {
            float inv_w = 1.0f / vec.w;
            vec.x *= inv_w;
            vec.y *= inv_w;
            vec.z *= inv_w;
            // 注意：这里保留了原始w值，如需可添加vec.w = 1.0f;
        }

        // 法线变换矩阵
        Matrix4x4 inv_trans = viewMatrix.multiply(modelMatrix).invert().transpose();

        oeVec3 n[] = {
            to_vec3(inv_trans.multiply(to_vec4(ver[i.x].normal, 0.0))),
            to_vec3(inv_trans.multiply(to_vec4(ver[i.y].normal, 0.0))),
            to_vec3(inv_trans.multiply(to_vec4(ver[i.z].normal, 0.0)))};

        // viewport transform
        for (auto &vert : v)
        {
            vert.x = (vert.x + 1.0) * 0.5 * (width - 1);
            vert.y = (1.0 - vert.y) * 0.5 * (height - 1);
            vert.z = f1 * vert.z + f2;
        }

        DrawTriangle(v[0], n[0], ver[i.x].color, v[1], n[1], ver[i.y].color, v[2], n[2], ver[i.z].color);
    }

    // 对所有的数据进行MSAA抗锯齿
    resolveMSAA();
}