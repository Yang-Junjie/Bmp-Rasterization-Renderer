#include "Rasterizer.hpp"
#include "vector2d.hpp"
#include <algorithm>


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

col_buf_id Rasterizer::load_colors(const std::vector<oeVec3> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

nor_buf_id Rasterizer::load_normals(const std::vector<oeVec3>& normals)
{
    auto id = get_next_id();
    nor_buf.emplace(id, normals);

    normal_id = id;

    return {id};
}


void Rasterizer::draw_line(oeVec3 begin, oeVec3 end)
{
    auto x1 = begin.x;
    auto y1 = begin.y;
    auto x2 = end.x;
    auto y2 = end.y;

    oeVec3 line_color = {255, 255, 255};

    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
    dx1=fabs(dx);
    dy1=fabs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1)
    {
        if(dx>=0)
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else
        {
            x=x2;
            y=y2;
            xe=x1;
        }
        oeVec2 point = oeVec2(x, y);
        set_pixel(point,line_color);
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }

            oeVec2 point = oeVec2(x, y);
            set_pixel(point,line_color);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        oeVec2 point = oeVec2(x, y);
        set_pixel(point,line_color);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }

            oeVec2 point = oeVec2(x, y);
            set_pixel(point,line_color);
        }
    }
}

auto to_vec4(const oeVec3& v3, float w = 1.0f)
{
    return oeVec4(v3.x, v3.y, v3.z, w);
}

auto to_vec3(const oeVec4& v4)
{
    return oeVec3(v4.x, v4.y, v4.z);
}

static bool insideTriangle(int x, int y, const oeVec4* _v){
    oeVec3 v[3];
    for(int i=0;i<3;i++)
        v[i] = {_v[i].x,_v[i].y, 1.0};
    oeVec3 f0,f1,f2;
    f0 = v[1].cross(v[0]);
    f1 = v[2].cross(v[1]);
    f2 = v[0].cross(v[2]);
    oeVec3 p(x,y,1.0);
    if((p.dot(f0)*f0.dot(v[2])>0) && (p.dot(f1)*f1.dot(v[0])>0) && (p.dot(f2)*f2.dot(v[1])>0))
        return true;
    return false;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const oeVec4* v){
    float c1 = (x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*y + v[1].x*v[2].y - v[2].x*v[1].y) / (v[0].x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*v[0].y + v[1].x*v[2].y - v[2].x*v[1].y);
    float c2 = (x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*y + v[2].x*v[0].y - v[0].x*v[2].y) / (v[1].x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*v[1].y + v[2].x*v[0].y - v[0].x*v[2].y);
    float c3 = (x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*y + v[0].x*v[1].y - v[1].x*v[0].y) / (v[2].x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*v[2].y + v[0].x*v[1].y - v[1].x*v[0].y);
    return {c1,c2,c3};
}

void Rasterizer::draw(std::vector<Triangle *> &TriangleList) {
    
    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Matrix4x4 mvp =(projMatrix.multiply(viewMatrix)).multiply(modelMatrix);
    for (const auto& t:TriangleList)
    {
        Triangle newtri = *t;
        
        std::array<oeVec4, 3> mm {
                viewMatrix.multiply(modelMatrix.multiply(t->v[0])),
                viewMatrix.multiply(modelMatrix.multiply(t->v[1])),
                viewMatrix.multiply(modelMatrix.multiply(t->v[2]))
        };
        
        std::array<oeVec3, 3> viewspace_pos;

         for (int j = 0; j < 3; ++j)
         {
             viewspace_pos[j] = oeVec3(mm[j].x, mm[j].y, mm[j].z); // 或用to_vec3(mm[j])
         }

        oeVec4 v[] = {
                mvp.multiply(t->v[0]),
                mvp.multiply(t->v[1]),
                mvp.multiply(t->v[2])
        };
        // std::cout<<"1:"<<v[0]<<std::endl;
        // std::cout<<"2:"<<v[1]<<std::endl;
        // std::cout<<"3:"<<v[2]<<std::endl;
        for (auto& vec : v) {
            vec.x/=vec.w;
            vec.y/=vec.w;
            vec.z/=vec.w;
            vec.w = 1.0;

        }

        Matrix4x4 inv_trans = (viewMatrix.multiply(modelMatrix)).invert().transpose();
        
        
        oeVec4 n[] = {
                inv_trans.multiply(to_vec4(t->normal[0], 0.0f)),
                inv_trans.multiply(to_vec4(t->normal[1], 0.0f)),
                inv_trans.multiply(to_vec4(t->normal[2], 0.0f))
        };

       

        for (auto & vert : v)
        {
            // vert.x = (vert.x + 1.0) * 0.5 * (width - 1);
            // vert.y = (1.0 - vert.y) * 0.5 * (height - 1);
            // vert.z = f1 * vert.z + f2;

            // vert.x = std::clamp(vert.x, 0.0, width - 1.0);
            // vert.y = std::clamp(vert.y, 0.0, height - 1.0);

            vert.x = 0.5*width*(vert.x+1.0);
            vert.y = 0.5*height*(vert.y+1.0);
            vert.z = vert.z * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            newtri.setVertex(i, v[i]);
        }

        for (int i = 0; i < 3; ++i)
        {
            newtri.setNormal(i, to_vec3(n[i]));
        }

        newtri.setColor(0, 0,0,0);
        newtri.setColor(1, 0,255,255);
        newtri.setColor(2, 255,0,0);
       
        rasterize_triangle(newtri);
    }
}

void Rasterizer::clear(Buffers buff)
{
    if ((buff & Buffers::Color) == Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(),oeVec3{0, 0, 0});
    }
    if ((buff & Buffers::Depth) == Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

static oeVec3 interpolate(float alpha, float beta, float gamma, const oeVec3& vert1, const oeVec3& vert2, const oeVec3& vert3, float weight)
{
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

static oeVec2 interpolate(float alpha, float beta, float gamma, const oeVec2& vert1, const oeVec2& vert2, const oeVec2& vert3, float weight)
{
    auto u = (alpha * vert1.x + beta * vert2.x + gamma * vert3.x);
    auto v = (alpha * vert1.y + beta * vert2.y + gamma * vert3.y);

    u /= weight;
    v /= weight;

    return oeVec2(u, v);
}

//Screen space rasterization
void Rasterizer::rasterize_triangle(const Triangle& t) 
{
    
 auto v = t.toVector4();
    // 使用浮点数计算边界
    float min_x = std::min({v[0].x, v[1].x, v[2].x});
    float max_x = std::max({v[0].x, v[1].x, v[2].x});
    float min_y = std::min({v[0].y, v[1].y, v[2].y});
    float max_y = std::max({v[0].y, v[1].y, v[2].y});
    
    // 转换为整数并钳制
    int start_x = std::max(0, (int)std::floor(min_x));
    int end_x = std::min(width-1, (int)std::ceil(max_x));
    int start_y = std::max(0, (int)std::floor(min_y));
    int end_y = std::min(height-1, (int)std::ceil(max_y));
    // for (auto point : v)
    // {
    //     if (point.x < min_x) min_x = point.x;
    //     if (point.x > max_x) max_x = point.x;
    //     if (point.y < min_y) min_y = point.y;
    //     if (point.y > max_y) max_y = point.y;
    // }
    for (int y = start_y; y <= end_y; y++)
    {
        for (int x = start_x; x <= end_x; x++)
        {
            if (insideTriangle((float)x + 0.5, (float)y + 0.5, t.v)) //以像素中心点作为采样点
            {
                
                //得到这个点的重心坐标
                auto abg = computeBarycentric2D((float)x + 0.5, (float)y + 0.5, t.v);
                float alpha = std::get<0>(abg);
                float beta = std::get<1>(abg);
                float gamma = std::get<2>(abg);
                //z-buffer插值
                float w_reciprocal = 1.0 / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w); //归一化系数
                float z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                z_interpolated *= w_reciprocal;
        
                if (z_interpolated < depth_buf[get_index(x, y)])
                {
                   
                    oeVec2 p = { (float)x,(float)y};
                    // 颜色插值
                    auto interpolated_color = interpolate(alpha, beta, gamma, t.color[0], t.color[1], t.color[2], 1);
                    interpolated_color = {std::floor(interpolated_color.x*255.0),
                        std::floor(interpolated_color.y*255.0),
                        std::floor(interpolated_color.z*255.0)};
                    // 纹理颜色插值
                    auto interpolated_texcoords = interpolate(alpha, beta, gamma, t.tex_coords[0], t.tex_coords[1], t.tex_coords[2], 1);
                    //std::cout<<this->texture->getColor(interpolated_texcoords.x,interpolated_texcoords.y)<<std::endl;
                    oeVec3 color = this->texture->getColor(interpolated_texcoords.x,interpolated_texcoords.y);
                    set_pixel(p,color); //设置颜色
                   // std::cout<<interpolated_texcoords<<std::endl;this->texture->getColor(interpolated_texcoords.x,interpolated_texcoords.y)
                    //
                    depth_buf[get_index(x, y)] = z_interpolated;//更新z值
                }
            }
        }
    }
}


// void Rasterizer::clear(Buffers buff)
// {
//     if ((buff & Buffers::Color) == Buffers::Color)
//     {
//         std::fill(frame_buf.begin(), frame_buf.end(), oeVec3{0, 0, 0});
//     }
//     if ((buff & Buffers::Depth) == Buffers::Depth)
//     {
//         std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity);
//     }
// }

Rasterizer::Rasterizer(Bmp* bmp) :m_bmp(bmp), width(bmp->GetWidth()), height(bmp->GetHeight())
{
    frame_buf.resize(bmp->GetWidth() * bmp->GetHeight());
    depth_buf.resize(bmp->GetWidth() * bmp->GetHeight());

    texture = std::nullopt;
}

int Rasterizer::get_index(int x, int y)
{
    
    return (height - 1 - y) * width + x;
}


void Rasterizer::set_pixel(const oeVec2 &point, const oeVec3 &color) {
    int x = static_cast<int>(point.x);
    int y = static_cast<int>(point.y);
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int ind = (height - 1 - y) * width + x;
    frame_buf[ind] = color;
}


