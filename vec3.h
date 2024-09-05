#pragma once

#include <cmath>

struct vec3 {
	float x, y, z;

	vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

inline vec3 operator+(const vec3& a, const vec3& b) {
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator-(const vec3& a, const vec3& b) {
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator*(const vec3& a, float b) {
	return vec3(a.x * b, a.y * b, a.z * b);
}

inline vec3 operator*(float a, const vec3& b) {
	return vec3(a * b.x, a * b.y, a * b.z);
}

inline vec3 operator/(const vec3& a, float b) {
	return vec3(a.x / b, a.y / b, a.z / b);
}

inline float dot(const vec3& a, const vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(const vec3& a, const vec3& b) {
	return vec3(a.y * b.z - b.y * a.z,
				b.x * a.z - a.x * b.z,
				a.x * b.y - b.x * a.y);
}

inline float mag(const vec3& a) {
	return std::sqrt(dot(a, a));
}

inline float sqrMag(const vec3& a) {
	return dot(a, a);
}

vec3 normalize(const vec3& a) {
	float len = mag(a);
	if (len != 0) {
		return a / len;
	}
}