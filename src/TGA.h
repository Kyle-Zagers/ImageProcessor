#pragma once
#include <string>
#include <vector>
#include <cstdint>
using namespace std;

#pragma pack(push, 1) // ensuring the header is always 18 bytes
struct TGA_Header {
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t dataTypeCode;
    uint16_t colorMapOrigin;
    uint16_t colorMapLength;
    uint8_t colorMapDepth;
    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t bitsPerPixel;
    uint8_t imageDescriptor;
    TGA_Header();
    TGA_Header(uint16_t width, uint16_t height);
};
#pragma pack(pop) // removing the packing alignment

struct TGA_File : private TGA_Header
{
    string filename;
    TGA_Header header;
    vector<uint8_t> data;
    TGA_File();
    TGA_File(string filename, TGA_Header header, vector<uint8_t> data);
    TGA_File(string filename);
    void TGA_Write(string filename);
    void multiply(TGA_File image);
    void subtract(TGA_File image);
    void overlay(TGA_File image);
    void screen(TGA_File image);
    void combine(TGA_File image1, TGA_File image2);
    void flip();
    TGA_File onlyred();
    TGA_File onlygreen();
    TGA_File onlyblue();
    void addred(int add);
    void addgreen(int add);
    void addblue(int add);
    void scalered(int scale);
    void scalegreen(int scale);
    void scaleblue(int scale);
};