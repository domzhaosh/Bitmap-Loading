#ifndef _BITMAP_H
#define _BITMAP_H
using namespace std;

const short BITMAP_MAGIC_NUMBER = 19778;
const int RGB_BYTE_SIZE = 3;

#pragma pack(push,bitmap_data,1)

#pragma pack(pop,bitmap_data)

class Bitmap
{
    private:
        RGBQUAD * colours;
        BITMAPFILEHEADER bmfh;
        BITMAPINFOHEADER bmih;
        BYTE * data;
        LONG datasize, padding, lineByteSize;
        wstringstream wss;
        wchar_t * bmpStringInfo;
        void loadBMP(char *);
        void Bitmap::StoreBitmapInfo();
    public:
        LONG width, height;
        Bitmap(void);
        Bitmap(char *);
        ~Bitmap();
        LPCWSTR GeneralBitmapInfo();
        RGBQUAD * getColours();
        BITMAPFILEHEADER getBitmapFileHeader();
        BITMAPINFOHEADER getBitmapInfoHeader();
        BYTE * Bitmap::getBitmapData();
        void bitmapApplyNegative();
};

void reverseLine(BYTE * src, LONG size, WORD indexSize);

#endif //_BITMAP_H