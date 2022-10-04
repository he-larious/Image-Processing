#pragma once
#include <fstream>
#include <vector>
using namespace std;

class Image {
public:
	struct Header {
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short width;
		short height;
		char bitsPerPixel;
		char imageDescriptor;
	};
	struct Pixel {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};
	void ReadImage(string fileName);
	void WriteImage(string fileName);
	bool TestImage(Image& compare);
	Image Multiply(Image& rhs);
	Image Subtract(Image& rhs);
	Image Screen(Image& rhs);
	Image Overlay(Image& rhs);
	Image AddGreen();
	Image ScaleRB();
	Image Channel(string rgb);
	void CombineRGB(Image& r, Image& g, Image& b);
	void Rotate180();
	unsigned char ClampValue(int value);

private:
	Header header;
	vector<Pixel> pixels;
};
