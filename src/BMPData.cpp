#include "BMPData.h"
#include "stdafx.h"

Bitmap::Bitmap()
{
}

Bitmap::Bitmap(char * file)
{
    if (data != 0)
    {
        delete[] data;
        wss.clear();
    }

    loadBMP(file);
}

Bitmap::~Bitmap()
{
    if (data != 0)
    {
        delete[] data;
    }
}

void Bitmap::loadBMP(char * file)
{
    std::ifstream ifs;
    ifs.open(file, std::ios::binary);
    ifs.seekg (0, std::ios::end);
    streampos length = ifs.tellg();
    ifs.seekg (0, std::ios::beg);
    data = (BYTE *)malloc(sizeof(BYTE) * length);
    ifs.read ((char *)data, length);
    ifs.close();
    bmfh = *(BITMAPFILEHEADER *)data;
    bmih  = *(BITMAPINFOHEADER *)(data + sizeof(BITMAPFILEHEADER));
    colours = (RGBQUAD *)(data + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
    width = bmih.biWidth;
    height = bmih.biHeight;
    datasize = width * height * (bmih.biBitCount / 8);
    lineByteSize = padding = (int)((float)width * (float)bmih.biBitCount / 8.0);

    //adjust the width for padding as necessary
    while (padding % 4 != 0)
    {
        padding++;
    }

    StoreBitmapInfo();
}

void Bitmap::StoreBitmapInfo()
{
    wss << " --- BITMAPFILEHEADER --- " << endl;
    wss << "	bfType          =" << (int)bmfh.bfType << endl;
    wss << "	bfSize          =" << (int)bmfh.bfSize << endl;
    wss << "	bfOffBits       =" << (int)bmfh.bfOffBits << endl;
    wss << " --- BITMAPINFOHEADER --- " << endl;
    wss << "	biBitCount      =" << (int)bmih.biBitCount << endl;
    wss << "	biClrImportant  =" << (int)bmih.biClrImportant << endl;
    wss << "	biClrUsed       =" << (int)bmih.biClrUsed << endl;
    wss << "	biCompression   =" << (int)bmih.biCompression << endl;
    wss << "	biHeight        =" << (int)bmih.biHeight << endl;
    wss << "	biPlanes        =" << (int)bmih.biPlanes << endl;
    wss << "	biSize          =" << (int)bmih.biSize << endl;
    wss << "	biSizeImage     =" << (int)bmih.biSizeImage << endl;
    wss << "	biWidth         =" << (int)bmih.biWidth << endl;
    wss << "	biXPelsPerMeter =" << (int)bmih.biXPelsPerMeter << endl;
    wss << "	biYPelsPerMeter =" << (int)bmih.biYPelsPerMeter << endl;
    bmpStringInfo = new wchar_t[1024];
    wcscpy_s(bmpStringInfo, 1024, wss.str().c_str());
}

LPCWSTR Bitmap::GeneralBitmapInfo()
{
    return (LPCWSTR)bmpStringInfo;
}

RGBQUAD * Bitmap::getColours()
{
    return colours;
}

BITMAPFILEHEADER Bitmap::getBitmapFileHeader()
{
    return bmfh;
}

BITMAPINFOHEADER Bitmap::getBitmapInfoHeader()
{
    return bmih;
}

BYTE * Bitmap::getBitmapData()
{
    return data;
}

void Bitmap::bitmapApplyNegative()
{
    if (bmih.biBitCount <= 8)
    {
        WORD numColors = 1 << bmih.biBitCount;

        for (int i = 0; i < numColors; i++)
        {
            colours[i].rgbBlue = 255 - colours[i].rgbBlue;
            colours[i].rgbGreen = 255 - colours[i].rgbGreen;
            colours[i].rgbRed = 255 - colours[i].rgbRed;
        }
    }

    else
    {
        for (int i = 0; i < datasize; i += 3)
        {
            *(data + i + 2) = 255 - *(data + i + 2);
            *(data + i + 1) = 255 - *(data + i + 1);
            *(data + i) = 255 - *(data + i);
        }
    }
}


void reverseLine(BYTE * src, LONG size, WORD indexSize)
{
    BYTE * buffer = new BYTE[size];
    memcpy(buffer, src, size);
    LONG i = 0;

    while (i < size)
    {
        memcpy(src + i, buffer + size - i - indexSize, indexSize);
        i += indexSize;
    }

    free(buffer);
}