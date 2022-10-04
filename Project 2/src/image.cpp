#include "image.h"
#include <iostream>
using namespace std;

void Image::ReadImage(string fileName) {
	ifstream file(fileName, ios_base::binary);
	// Load header.
	if (file.is_open()) {
		file.read(&header.idLength, 1);
		file.read(&header.colorMapType, 1);
		file.read(&header.dataTypeCode, 1);
		file.read((char*)&header.colorMapOrigin, 2);
		file.read((char*)&header.colorMapLength, 2);
		file.read(&header.colorMapDepth, 1);
		file.read((char*)&header.xOrigin, 2);
		file.read((char*)&header.yOrigin, 2);
		file.read((char*)&header.width, 2);
		file.read((char*)&header.height, 2);
		file.read(&header.bitsPerPixel, 1);
		file.read(&header.imageDescriptor, 1);
	}

	// Load image.
	int totalPixels = (int)header.width * header.height;
	for (int i = 0; i < totalPixels; i++) {
		Pixel pixel;
		if (file.is_open()) {
			file.read((char*)&pixel.blue, 1);
			file.read((char*)&pixel.green, 1);
			file.read((char*)&pixel.red, 1);
		}
		pixels.push_back(pixel);
	}
	file.close();
}

void Image::WriteImage(string fileName) {
	ofstream file(fileName, ios_base::binary);
	// Write header.
	if (file.is_open()) {
		file.write(&header.idLength, 1);
		file.write(&header.colorMapType, 1);
		file.write(&header.dataTypeCode, 1);
		file.write((char*)&header.colorMapOrigin, 2);
		file.write((char*)&header.colorMapLength, 2);
		file.write(&header.colorMapDepth, 1);
		file.write((char*)&header.xOrigin, 2);
		file.write((char*)&header.yOrigin, 2);
		file.write((char*)&header.width, 2);
		file.write((char*)&header.height, 2);
		file.write(&header.bitsPerPixel, 1);
		file.write(&header.imageDescriptor, 1);
	}

	// Write image.
	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel = pixels.at(i);
		if (file.is_open()) {
			file.write((char*)&pixel.blue, 1);
			file.write((char*)&pixel.green, 1);
			file.write((char*)&pixel.red, 1);
		}
	}
	file.close();
}

bool Image::TestImage(Image& compare) {
	bool same = true;

	if (this->header.idLength != compare.header.idLength) {
		cout << "idLength wrong." << endl;
		same = false;
	}
	if (this->header.colorMapType != compare.header.colorMapType) {
		cout << "colorMapType wrong." << endl;
		same = false;
	}
	if (this->header.dataTypeCode != compare.header.dataTypeCode) {
		cout << "dataTypeCode wrong." << endl;
		same = false;
	}
	if (this->header.colorMapOrigin != compare.header.colorMapOrigin) {
		cout << "colorMapOrigin wrong." << endl;
		same = false;
	}
	if (this->header.colorMapLength != compare.header.colorMapLength) {
		cout << "colorMapLength wrong." << endl;
		same = false;
	}
	if (this->header.colorMapDepth != compare.header.colorMapDepth) {
		cout << "colorMapDepth wrong." << endl;
		same = false;
	}
	if (this->header.xOrigin != compare.header.xOrigin) {
		cout << "xOrigin wrong." << endl;
		same = false;
	}
	if (this->header.yOrigin != compare.header.yOrigin) {
		cout << "yOrigin wrong." << endl;
		same = false;
	}
	if (this->header.width != compare.header.width) {
		cout << "width wrong." << endl;
		same = false;
	}
	if (this->header.height != compare.header.height) {
		cout << "height wrong." << endl;
		same = false;
	}
	if (this->header.bitsPerPixel != compare.header.bitsPerPixel) {
		cout << "bitsPerPixel wrong." << endl;
		same = false;
	}
	if (this->header.imageDescriptor != compare.header.imageDescriptor) {
		cout << "imageDescriptor wrong." << endl;
		same = false;
	}
	if (this->pixels.size() != compare.pixels.size()) {
		cout << "Different pixel sizes." << endl;
		same = false;
	}
	for (unsigned int i = 0; i < compare.pixels.size(); i++) {
		if (this->pixels.at(i).blue != compare.pixels.at(i).blue) {
			cout << "Blue pixel at " << i << " is wrong." << endl;
			same = false;
		}
		if (this->pixels.at(i).green != compare.pixels.at(i).green) {
			cout << "Green pixel at " << i << " is wrong." << endl;
			same = false;
		}
		if (this->pixels.at(i).red != compare.pixels.at(i).red) {
			cout << "Red pixel at " << i << " is wrong." << endl;
			same = false;
		}
	}
	return same;
}

Image Image::Multiply(Image& rhs) {
	// rhs is bottom layer.
	Image result;
	result.header = header;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel;

		pixel.blue = (unsigned char)(pixels.at(i).blue * rhs.pixels.at(i).blue / 255.0f + 0.5f);
		pixel.green = (unsigned char)(pixels.at(i).green * rhs.pixels.at(i).green / 255.0f + 0.5f);
		pixel.red = (unsigned char)(pixels.at(i).red * rhs.pixels.at(i).red / 255.0f + 0.5f);

		result.pixels.push_back(pixel);
	}

	return result;
}

Image Image::Subtract(Image& rhs) {
	// rhs is bottom layer.
	Image result;
	result.header = header;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel;

		pixel.blue = ClampValue((int)rhs.pixels.at(i).blue - (int)pixels.at(i).blue);
		pixel.green = ClampValue((int)rhs.pixels.at(i).green - (int)pixels.at(i).green);
		pixel.red = ClampValue((int)rhs.pixels.at(i).red - (int)pixels.at(i).red);

		result.pixels.push_back(pixel);
	}

	return result;
}

Image Image::Screen(Image& rhs) {
	// rhs is bottom layer.
	Image result;
	result.header = header;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel;

		float blueFloat = 1.0f - (1.0f - pixels.at(i).blue / 255.0f) * (1.0f - rhs.pixels.at(i).blue / 255.0f);
		pixel.blue = (unsigned char)(blueFloat * 255.0f + 0.5f);

		float greenFloat = 1.0f - (1.0f - pixels.at(i).green / 255.0f) * (1.0f - rhs.pixels.at(i).green / 255.0f);
		pixel.green = (unsigned char)(greenFloat * 255.0f + 0.5f);

		float redFloat = 1.0f - (1.0f - pixels.at(i).red / 255.0f) * (1.0f - rhs.pixels.at(i).red / 255.0f);
		pixel.red = (unsigned char)(redFloat * 255.0f + 0.5f);

		result.pixels.push_back(pixel);
	}

	return result;
}

Image Image::Overlay(Image& rhs) {
	Image result;
	result.header = header;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel;

		if (rhs.pixels.at(i).blue < 128) {
			pixel.blue = (unsigned char)(2 * pixels.at(i).blue * rhs.pixels.at(i).blue / 255.0f + 0.5f);
		}
		else {
			float blueFloat = 1.0f - 2.0f * (1.0f - pixels.at(i).blue / 255.0f) * (1.0f - rhs.pixels.at(i).blue / 255.0f);
			pixel.blue = (unsigned char)(blueFloat * 255.0f + 0.5f);
		}

		if (rhs.pixels.at(i).green < 128) {
			pixel.green = (unsigned char)(2 * pixels.at(i).green * rhs.pixels.at(i).green / 255.0f + 0.5f);
		}
		else {
			float greenFloat = 1.0f - 2.0f * (1.0f - pixels.at(i).green / 255.0f) * (1.0f - rhs.pixels.at(i).green / 255.0f);
			pixel.green = (unsigned char)(greenFloat * 255.0f + 0.5f);
		}

		if (rhs.pixels.at(i).red < 128) {
			pixel.red = (unsigned char)(2 * pixels.at(i).red * rhs.pixels.at(i).red / 255.0f + 0.5f);
		}
		else {
			float redFloat = 1.0f - 2.0f * (1.0f - pixels.at(i).red / 255.0f) * (1.0f - rhs.pixels.at(i).red / 255.0f);
			pixel.red = (unsigned char)(redFloat * 255.0f + 0.5f);
		}

		result.pixels.push_back(pixel);
	}

	return result;
}

Image Image::AddGreen() {
	Image result;
	result.header = this->header;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel;

		pixel.blue = pixels.at(i).blue;
		pixel.red = pixels.at(i).red;
		pixel.green = ClampValue(pixels.at(i).green + 200);

		result.pixels.push_back(pixel);
	}

	return result;
}

Image Image::ScaleRB() {
	Image result;
	result.header = this->header;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel;

		pixel.green = pixels.at(i).green;
		pixel.blue = (unsigned char)(0 * pixels.at(i).blue);
		float redFloat = 4.0f * (pixels.at(i).red / 255.0f);
		pixel.red = ClampValue((int)(redFloat * 255.0f + 0.5f));

		result.pixels.push_back(pixel);
	}

	return result;
}

Image Image::Channel(string rgb) {
	Image result;
	result.header = this->header;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		Pixel pixel;

		if (rgb == "red") {
			pixel.red = pixels.at(i).red;
			pixel.blue = pixels.at(i).red;
			pixel.green = pixels.at(i).red;
		}
		else if (rgb == "green") {
			pixel.red = pixels.at(i).green;
			pixel.blue = pixels.at(i).green;
			pixel.green = pixels.at(i).green;
		}
		else if (rgb == "blue") {
			pixel.red = pixels.at(i).blue;
			pixel.blue = pixels.at(i).blue;
			pixel.green = pixels.at(i).blue;
		}

		result.pixels.push_back(pixel);
	}

	return result;
}

void Image::CombineRGB(Image& r, Image& g, Image& b) {
	header = r.header;

	for (unsigned int i = 0; i < r.pixels.size(); i++) {
		Pixel pixel;

		pixel.red = r.pixels.at(i).red;
		pixel.green = g.pixels.at(i).green;
		pixel.blue = b.pixels.at(i).blue;

		pixels.push_back(pixel);
	}
}

void Image::Rotate180() {
	for (unsigned int i = 0; i < (pixels.size() / 2); i++) {
		Pixel temp = pixels.at(i);
		pixels.at(i) = pixels.at(pixels.size() - 1 - i);
		pixels.at(pixels.size() - 1 - i) = temp;
	}
}

unsigned char Image::ClampValue(int value) {
	unsigned char result;
	if (value < 0) {
		result = 0;
	}
	else if (value > 255) {
		result = 255;
	}
	else {
		result = (unsigned char)value;
	}
	return result;
}