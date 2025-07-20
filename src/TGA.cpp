#include <fstream>
#include <iostream>
#include "TGA.h"
using namespace std;

//TGA Header:
TGA_Header::TGA_Header() {}

TGA_Header::TGA_Header(uint16_t width, uint16_t height)
{
    idLength = 0;
    colorMapType = 0;
    dataTypeCode = 2;
    colorMapOrigin = 0;
    colorMapLength = 0;
    colorMapDepth = 0;
    xOrigin = 0;
    yOrigin = 0;
    this->width = width;
    this->height = height;
    bitsPerPixel = 24;
    imageDescriptor = 0;
}

//TGA File:
TGA_File::TGA_File() {}

TGA_File::TGA_File(string filename, TGA_Header header, vector<uint8_t> data)
{
    this->filename = filename;
    this->header = header;
    this->data = data;
}

TGA_File::TGA_File(string filename)
{
    this->filename = filename;
    ifstream file(this->filename, ios::binary);
    if (!file.is_open())
    {
        cout << "File does not exist." << endl;
        exit(0);
    }

    // packing allows for header to be read all at once.
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Needs to be resized so the memory is the right size for reading.
    data.resize(header.width * header.height * 3);
    file.read(reinterpret_cast<char*>(data.data()), data.size());

    file.close();
    //std::cout << "Read TGA file " << this->filename << endl;
}

void TGA_File::TGA_Write(string filename)
{
    ofstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cout << "Failed to open file for writing." << endl;
        exit(0);
    }

    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    file.write(reinterpret_cast<char*>(data.data()), data.size());

    file.close();
    //std::cout << "Wrote TGA file to " << filename << endl;
}

// Methods:
uint8_t clamping(int num)
{
    if (num > 255)
    {
        num = 255;
    }
    else if (num < 0)
    {
        num = 0;
    }
    return num;
}

float normalize(uint8_t num)
{
    return ((float)num / 255.0);
}

void TGA_File::multiply(TGA_File image)
{
    for (int i = 0; i < data.size(); i++)
    {
        data[i] = (uint8_t)((normalize(image.data[i]) * normalize(data[i]) * 255)+0.5f);
    }
}

void TGA_File::subtract(TGA_File image)
{
    for (int i = 0; i < data.size(); i++)
    {
        data[i] = clamping((int)data[i] - (int)image.data[i]);
    }
}

void TGA_File::overlay(TGA_File image)
{
    for (int i = 0; i < data.size(); i++)
    {
        if (normalize(image.data[i]) <= (float) 0.5)
        {
            data[i] = (uint8_t)((2 * normalize(image.data[i]) * normalize(data[i]) * 255)+0.5f);
        }
        else
        {
            data[i] = (uint8_t)((((1 - (2 * (1-normalize(data[i])) * (1-normalize(image.data[i])))))*255)+0.5f);
        }
    }
}

void TGA_File::screen(TGA_File image)
{
    for (int i = 0; i < data.size(); i++)
    {
        data[i] = (uint8_t)(((1 - ((1 - normalize(data[i])) * (1 - normalize(image.data[i])))) * 255)+0.5f);
    }
}

void TGA_File::combine(TGA_File green, TGA_File blue)
{
    for (int i = 0; i < data.size()-2; i+=3)
    {
        data[i] = blue.data[i];
        data[i + 1] = green.data[i + 1];
    }
}

void TGA_File::flip()
{
    vector<uint8_t> flip(data.size());
    for (int i = 0; i < data.size()-2; i+=3)
    {
        flip[i] = data[data.size() - i - 3];
        flip[i+1] = data[data.size() - i - 2];
        flip[i+2] = data[data.size() - i - 1];
    }
    data = flip;
}

TGA_File TGA_File::onlyred()
{
    TGA_File red((filename.substr(0, filename.length() - 4)+"_r.tga"), header, data);
    for (int i = 0; i < red.data.size() - 2; i += 3)
    {
        red.data[i] = data[i+2];
        red.data[i + 1] = data[i+2];
    }
    return red;
}

TGA_File TGA_File::onlygreen()
{
    TGA_File green((filename.substr(0, filename.length() - 4) + "_g.tga"), header, data);
    for (int i = 0; i < green.data.size() - 2; i += 3)
    {
        green.data[i] = data[i+1];
        green.data[i + 2] = data[i+1];
    }
    return green;
}

TGA_File TGA_File::onlyblue()
{
    TGA_File blue((filename.substr(0, filename.length() - 4) + "_b.tga"), header, data);
    for (int i = 0; i < blue.data.size() - 2; i += 3)
    {
        blue.data[i + 1] = data[i];
        blue.data[i + 2] = data[i];
    }
    return blue;
}

void TGA_File::addred(int add)
{
    for (int i = 0; i < data.size() - 2; i += 3)
    {
        data[i + 2] = clamping((int)data[i + 2] + add);
    }
}

void TGA_File::addgreen(int add)
{
    for (int i = 0; i < data.size() - 2; i += 3)
    {
        data[i + 1] = clamping((int)data[i + 1] + add);
    }
}

void TGA_File::addblue(int add)
{
    for (int i = 0; i < data.size() - 2; i += 3)
    {
        data[i] = clamping((int)data[i] + add);
    }
}

void TGA_File::scalered(int scale)
{
    for (int i = 0; i < data.size() - 2; i += 3)
    {
        data[i + 2] = clamping((int)data[i + 2] * scale);
    }
}

void TGA_File::scalegreen(int scale)
{
    for (int i = 0; i < data.size() - 2; i += 3)
    {
        data[i + 1] = clamping((int)data[i + 1] * scale);
    }
}

void TGA_File::scaleblue(int scale)
{
    for (int i = 0; i < data.size() - 2; i += 3)
    {
        data[i] = clamping((int)data[i] * scale);
    }
}
