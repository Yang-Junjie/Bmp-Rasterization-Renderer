#include "bmp.hpp"

Bmp::Bmp(int width, int height) : width(width), height(height)
{
    this->data = std::vector<std::vector<oeVec3>>(
        width,
        std::vector<oeVec3>(
            height,
            oeVec3::Zero()));
}

Bmp::Bmp(const std::string &path)
{
    this->data = Bmp::readBMP(path);
}

int Bmp::GetWidth()
{
    return this->width;
}
int Bmp::GetHeight()
{
    return this->height;
}

void Bmp::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
    data[x][y].x = r;
    data[x][y].y = g;
    data[x][y].z = b;
}

std::vector<std::vector<oeVec3>> Bmp::readBMP(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return {};
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    file.read(reinterpret_cast<char *>(&fileHeader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));

    if (fileHeader.fileType != 0x4D42 || infoHeader.bitCount != 24)
    {
        std::cerr << "不支持的 BMP 格式" << std::endl;
        return {};
    }

    int width = infoHeader.width;
    int height = infoHeader.height;
    int padding = (4 - ((width * 3) % 4)) % 4;

    std::vector<std::vector<oeVec3>> image(height, std::vector<oeVec3>(width, oeVec3::Zero()));

    file.seekg(fileHeader.offsetData, std::ios::beg);

    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            unsigned char b, g, r;
            file.read(reinterpret_cast<char *>(&b), 1);
            file.read(reinterpret_cast<char *>(&g), 1);
            file.read(reinterpret_cast<char *>(&r), 1);

            image[y][x].x = static_cast<int>(r); // R
            image[y][x].y = static_cast<int>(g); // G
            image[y][x].z = static_cast<int>(b); // B
        }

        file.seekg(padding, std::ios::cur);
    }

    return image;
}

bool Bmp::LoadData(const std::vector<oeVec3> &frame_buf)
{
    if (frame_buf.size() != static_cast<size_t>(width * height))
    {
        std::cerr << "Error: Frame buffer size does not match BMP dimensions." << std::endl;
        return false;
    }

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            int index = y * width + x;
            data[x][y] = frame_buf[index];
        }
    }

    return true;
}

void Bmp::save(const std::string &filename) const
{
    int width = data[0].size();
    int height = data.size();

    if (width <= 0 || height <= 0)
    {
        std::cerr << "无效的图像数据" << std::endl;
        return;
    }

    int padding = (4 - ((width * 3) % 4)) % 4;
    uint32_t dataSize = (width * 3 + padding) * height;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + dataSize;
    fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.sizeImage = dataSize;

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }

    file.write(reinterpret_cast<char *>(&fileHeader), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));

    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            unsigned char r = static_cast<unsigned char>(data[y][x].x); // R
            unsigned char g = static_cast<unsigned char>(data[y][x].y); // G
            unsigned char b = static_cast<unsigned char>(data[y][x].z); // B
            file.write(reinterpret_cast<char *>(&b), 1);
            file.write(reinterpret_cast<char *>(&g), 1);
            file.write(reinterpret_cast<char *>(&r), 1);
        }

        for (int i = 0; i < padding; ++i)
        {
            file.put(0);
        }
    }

    file.close();
}