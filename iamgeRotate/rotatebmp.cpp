#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
#pragma pack(1)

struct BITMAPFILEHEADER
{
    unsigned short bfType;
    unsigned int bfsize;
    unsigned short bfReversed1=0;
    unsigned short bfReversed2=0;
    unsigned int bfOffBits;
};
struct tagBITMAPINFOHEADER
{
    unsigned int biSize;
    int biWIdth;
    int biHeight;
    unsigned short biPlanes=1;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};
struct RBGTRIPLE
{
    unsigned char rbgBlue;
    unsigned char rbgGreen;
    unsigned char rbgRed;
};
int main(int argc,char* argv[])
{
    if(argc==3)
    {
        ifstream f;
        f.open(argv[1],ios::binary);
        if(!f)
        {
            cout<<"the image wasn't opened correctly"<<endl;
            return 0;
        }
        BITMAPFILEHEADER fileheader;
        tagBITMAPINFOHEADER infoheader;
        f.read((char*)&fileheader,sizeof(BITMAPFILEHEADER));
        f.read((char*)&infoheader,sizeof(tagBITMAPINFOHEADER));
        if(fileheader.bfType!=0x4D42)
        {
            cout<<"error:the file mode isn't bmp"<<endl;
            return 0;
        }
        if(infoheader.biBitCount!=24)
        {
            cout<<"error:the image color mode isn't true color"<<endl;
            return 0;
        }
        //cout<<"correct"<<endl;
        int width=infoheader.biWIdth;
        int height=infoheader.biHeight;
        int padding = (4 - (width * 3 % 4)) % 4;
        vector<vector<RBGTRIPLE>> image(height, vector<RBGTRIPLE>(width));
        f.seekg(fileheader.bfOffBits, ios::beg);
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                f.read((char*)&image[i][j],sizeof(RBGTRIPLE));
            }
            f.seekg(padding,ios::cur);
        }
        //cout<<"reading completed"<<endl;
        BITMAPFILEHEADER newfileheader=fileheader;
        tagBITMAPINFOHEADER newinfoheader=infoheader;
        newinfoheader.biHeight=infoheader.biWIdth;
        newinfoheader.biWIdth=infoheader.biHeight;
        newinfoheader.biXPelsPerMeter=infoheader.biYPelsPerMeter;
        newinfoheader.biYPelsPerMeter=infoheader.biXPelsPerMeter;
        int new_real_line = height * 3;
        int newpadding = (4 - (new_real_line % 4)) % 4;
        newinfoheader.biSizeImage = (new_real_line + newpadding) * width;
        newfileheader.bfsize = fileheader.bfOffBits + newinfoheader.biSizeImage;
        vector<vector<RBGTRIPLE>> newimage(width, vector<RBGTRIPLE>(height));
        for(int i=0;i<width;i++)
        {
            for(int j=0;j<height;j++)
            {
                newimage[i][j]=image[j][width-1-i];
            }
        }
        ofstream of;
        of.open(argv[2],ios::binary);
        of.write((char*)&newfileheader,sizeof(BITMAPFILEHEADER));
        of.write((char*)&newinfoheader,sizeof(tagBITMAPINFOHEADER));
        for(int i=0;i<width;i++)
        {
            for(int j=0;j<height;j++)
            {
                of.write((char*)&newimage[i][j],sizeof(RBGTRIPLE));
            }
            for(int k=0;k<newpadding;k++)
            {
                unsigned char zero=0;
                of.write((char*)&zero,1);
            }
        }
        //cout<<"rotatecompleted"<<endl;
        f.close();
        of.close();
    }
    return 0;
}