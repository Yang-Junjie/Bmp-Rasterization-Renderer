#pragma once
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t fileType{0x4D42}; // 'BM'
    uint32_t fileSize{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offsetData{54}; // 默认头部大小为54字节
};

struct BMPInfoHeader {
    uint32_t size{40}; // 信息头大小
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bitCount{24}; // 每像素比特数
    uint32_t compression{0}; // 无压缩
    uint32_t sizeImage{0};
    int32_t xPixelsPerMeter{0};
    int32_t yPixelsPerMeter{0};
    uint32_t colorsUsed{0};
    uint32_t colorsImportant{0};
};
#pragma pack(pop)

class Bmp {
public:
    Bmp(int width, int height);
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    bool save(const std::string& path) const;
    int GetWidth();
    int GetHeight();

private:
    int width, height;
    size_t row_stride;
    std::vector<uint8_t> data;
};