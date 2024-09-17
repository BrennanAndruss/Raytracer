#pragma once

#include "vec3.h"

struct Ray {
	Point3 origin;
	Vec3 dir;

	Ray() : origin(Point3()), dir(Vec3()) {}
	Ray(const Point3& origin, const Vec3& dir) : origin(origin), dir(dir) {}

	Point3 at(float t) const {
		return origin + t * dir;
	}
};