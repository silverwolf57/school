#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// 必须 2 字节对齐 [cite: 12, 13]
#pragma pack(2)

struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
};

struct BITMAPINFOHEADER {
    unsigned int   biSize;
    int            biWidth;
    int            biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
};

struct RGBTRIPLE {
    unsigned char rgbtBlue;
    unsigned char rgbtGreen;
    unsigned char rgbtRed;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: rotatebmp <src.bmp> <dest.bmp>" << endl;
        return 1;
    }

    ifstream is(argv[1], ios::binary);
    if (!is) return 1;

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    is.read((char*)&bfh, sizeof(bfh));
    is.read((char*)&bih, sizeof(bih));

    int oldW = bih.biWidth;
    int oldH = bih.biHeight;

    // 计算原图和新图的每行实际占用字节数（含Padding） [cite: 41, 94]
    int srcLineByte = ((oldW * 24 + 31) / 32) * 4;
    int destLineByte = ((oldH * 24 + 31) / 32) * 4;

    // 读取原图数据
    vector<unsigned char> srcData(srcLineByte * oldH);
    is.seekg(bfh.bfOffBits, ios::beg);
    is.read((char*)srcData.data(), srcData.size());
    is.close();

    // 更新新图的头信息
    int newW = oldH;
    int newH = oldW;
    bih.biWidth = newW;
    bih.biHeight = newH;
    bih.biSizeImage = destLineByte * newH;
    bfh.bfSize = bfh.bfOffBits + bih.biSizeImage;

    vector<unsigned char> destData(bih.biSizeImage, 0);

    // 旋转逻辑
    // BMP在内存中是倒置存储的（最后一列像素其实是图像的第一行） 
    for (int y = 0; y < oldH; y++) {
        for (int x = 0; x < oldW; x++) {
            // 找到原图像素位置
            unsigned char* sP = &srcData[y * srcLineByte + x * 3];

            // 顺时针旋转90度逻辑：
            // 新坐标 (nx, ny) 其中 nx = y, ny = (newH - 1) - x
            int nx = y;
            int ny = (oldW - 1) - x;

            unsigned char* dP = &destData[ny * destLineByte + nx * 3];

            dP[0] = sP[0]; // B
            dP[1] = sP[1]; // G
            dP[2] = sP[2]; // R
        }
    }

    ofstream os(argv[2], ios::binary);
    os.write((char*)&bfh, sizeof(bfh));
    os.write((char*)&bih, sizeof(bih));
    os.write((char*)destData.data(), destData.size());
    os.close();

    return 0;
}