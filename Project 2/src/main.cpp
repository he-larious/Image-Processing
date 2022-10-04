#include "image.h"
#include <iostream>
using namespace std;

int main() {
	// Part 1
	Image layer1, pattern1;
	layer1.ReadImage("input/layer1.tga");
	pattern1.ReadImage("input/pattern1.tga");
	Image part1 = layer1.Multiply(pattern1);
	part1.WriteImage("output/part1.tga");

	// Part 2
	Image layer2, car;
	layer2.ReadImage("input/layer2.tga");
	car.ReadImage("input/car.tga");
	Image part2 = layer2.Subtract(car);
	part2.WriteImage("output/part2.tga");

	// Part 3
	Image pattern2, text;
	pattern2.ReadImage("input/pattern2.tga");
	text.ReadImage("input/text.tga");
	Image temp = layer1.Multiply(pattern2);
	Image part3 = text.Screen(temp);
	part3.WriteImage("output/part3.tga");

	// Part 4
	Image circles;
	circles.ReadImage("input/circles.tga");
	temp = layer2.Multiply(circles);
	Image part4 = pattern2.Subtract(temp);
	part4.WriteImage("output/part4.tga");

	// Part 5
	Image part5 = layer1.Overlay(pattern1);
	part5.WriteImage("output/part5.tga");

	// Part 6
	Image part6 = car.AddGreen();
	part6.WriteImage("output/part6.tga");

	// Part 7
	Image part7 = car.ScaleRB();
	part7.WriteImage("output/part7.tga");

	// Part 8
	Image part8_r = car.Channel("red");
	Image part8_g = car.Channel("green");
	Image part8_b = car.Channel("blue");
	part8_r.WriteImage("output/part8_r.tga");
	part8_g.WriteImage("output/part8_g.tga");
	part8_b.WriteImage("output/part8_b.tga");

	// Part 9
	Image layer_red, layer_green, layer_blue;
	layer_red.ReadImage("input/layer_red.tga");
	layer_green.ReadImage("input/layer_green.tga");
	layer_blue.ReadImage("input/layer_blue.tga");
	Image part9;
	part9.CombineRGB(layer_red, layer_green, layer_blue);
	part9.WriteImage("output/part9.tga");

	// Part 10
	Image text2;
	text2.ReadImage("input/text2.tga");
	text2.Rotate180();
	text2.WriteImage("output/part10.tga");

	return 0;
}