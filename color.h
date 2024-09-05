#pragma once

#include <iostream>
#include "vec3.h"

using color = vec3;

void writeColor(std::ostream& out, const color& pixelColor) {
	auto r = pixelColor.x;
	auto g = pixelColor.y;
	auto b = pixelColor.z;

	int rByte = int(255.999 * r);
	int gByte = int(255.999 * g);
	int bByte = int(255.999 * b);

	out << rByte << " " << gByte << " " << bByte << "\n";
}