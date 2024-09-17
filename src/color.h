#pragma once

#include "interval.h"
#include "vec3.h"

using color = Vec3;

inline float linearToGamma(float linearComp) {
	if (linearComp > 0) {
		return std::sqrt(linearComp);
	}

	return 0;
}

void writeColor(std::ostream& out, const color& pixelColor) {
	auto r = pixelColor.x;
	auto g = pixelColor.y;
	auto b = pixelColor.z;

	// Apply a linear to gamma transformation for gamma 2
	r = linearToGamma(r);
	g = linearToGamma(g);
	b = linearToGamma(b);

	static const Interval intensity(0.0f, 0.999f);
	int rByte = static_cast<int>(256 * intensity.clamp(r));
	int gByte = static_cast<int>(256 * intensity.clamp(g));
	int bByte = static_cast<int>(256 * intensity.clamp(b));

	out << rByte << " " << gByte << " " << bByte << "\n";
}