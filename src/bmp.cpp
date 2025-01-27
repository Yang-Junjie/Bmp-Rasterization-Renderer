 #include "bmp.hpp"

 Bmp::Bmp(int width, int height) : width(width), height(height) {
        // 计算每行字节数（必须是4的倍数）
        row_stride = ((width * 3 + 3) / 4) * 4;
        data.resize(row_stride * height);
}
 bool Bmp::save(const std::string& path) const {
       
        BMPFileHeader fileHeader;
        BMPInfoHeader infoHeader;
        infoHeader.width = width;
        infoHeader.height = height;

        // 计算文件大小
        fileHeader.fileSize = fileHeader.offsetData + static_cast<uint32_t>(data.size());

        std::ofstream outFile(path, std::ios::binary);
        if (!outFile)  return false;

        outFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
        outFile.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
        outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
       
        return true;
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
    auto idx = (height - y - 1) * row_stride + x * 3;
    data[idx] = b;     // B
    data[idx + 1] = g; // G
    data[idx + 2] = r; // R
}
