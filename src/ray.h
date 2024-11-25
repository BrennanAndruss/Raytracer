#pragma once

#include "vec3.h"

struct Ray {
	Point3 origin;
	Vec3 dir;
	float time;

	Ray() : origin(Point3()), dir(Vec3()), time(0) {}
	Ray(const Point3& origin, const Vec3& dir, float time) : origin(origin), dir(dir), time(time) {}
	Ray(const Point3& origin, const Vec3& dir) : Ray(origin, dir, 0) {}

	Point3 at(float t) const {
		return origin + t * dir;
	}
};