#include "bmp.hpp"

Bmp::Bmp(int width, int height) : width(width), height(height)
{
    this->data = std::vector<std::vector<std::vector<uint8_t>>>(
        width, 
        std::vector<std::vector<uint8_t>>(
            height, 
            std::vector<uint8_t>(3)
        )
    );
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
    data[x][y][0] = r;
    data[x][y][1] = g;
    data[x][y][2] = b;
}

// 函数：读取 BMP 图片并存储颜色数据到 vector 中
std::vector<std::vector<std::vector<uint8_t>>> Bmp::readBMP(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return {};
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // 读取文件头和信息头
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    if (fileHeader.fileType != 0x4D42 || infoHeader.bitCount != 24) {
        std::cerr << "不支持的 BMP 格式" << std::endl;
        return {};
    }

    int width = infoHeader.width;
    int height = infoHeader.height;
    int padding = (4 - ((width * 3) % 4)) % 4; // 计算每行的填充字节数

    // 创建目标数据结构
    std::vector<std::vector<std::vector<uint8_t>>> image(height, std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(3)));

    // 跳过文件头和信息头
    file.seekg(fileHeader.offsetData, std::ios::beg);

    // 逐行读取像素数据
    for (int y = height - 1; y >= 0; --y) { // BMP 文件是从底部开始存储的
        for (int x = 0; x < width; ++x) {
            unsigned char b, g, r;
            file.read(reinterpret_cast<char*>(&b), 1);
            file.read(reinterpret_cast<char*>(&g), 1);
            file.read(reinterpret_cast<char*>(&r), 1);

            // 存储 RGB 值
            image[y][x][0] = static_cast<int>(r); // R
            image[y][x][1] = static_cast<int>(g); // G
            image[y][x][2] = static_cast<int>(b); // B
        }
        // 跳过填充字节
        file.seekg(padding, std::ios::cur);
    }

    return image;
}


void Bmp::save(const std::string& filename)const {
    int width = data[0].size();
    int height = data.size();

    if (width <= 0 || height <= 0) {
        std::cerr << "无效的图像数据" << std::endl;
        return;
    }

    int padding = (4 - ((width * 3) % 4)) % 4; // 计算每行的填充字节数
    uint32_t dataSize = (width * 3 + padding) * height; // 像素数据总大小

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // 设置文件头
    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + dataSize;
    fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    // 设置信息头
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.sizeImage = dataSize;

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }

    // 写入文件头和信息头
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    // 写入像素数据
    for (int y = height - 1; y >= 0; --y) { // BMP 文件是从底部开始存储的
        for (int x = 0; x < width; ++x) {
            unsigned char r = static_cast<unsigned char>(data[y][x][0]); // R
            unsigned char g = static_cast<unsigned char>(data[y][x][1]); // G
            unsigned char b = static_cast<unsigned char>(data[y][x][2]); // B
            file.write(reinterpret_cast<char*>(&b), 1); // BMP 存储顺序是 BGR
            file.write(reinterpret_cast<char*>(&g), 1);
            file.write(reinterpret_cast<char*>(&r), 1);
        }
        // 写入填充字节
        for (int i = 0; i < padding; ++i) {
            file.put(0);
        }
    }

    file.close();
}