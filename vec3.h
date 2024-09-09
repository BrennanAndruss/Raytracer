#pragma once

struct Vec3 {
	float x, y, z;

	Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

// Type aliases for Vec3
using Point3 = Vec3;
using Color = Vec3;

inline Vec3 operator+(const Vec3& a, const Vec3& b) {
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) {
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3& a, float b) {
	return Vec3(a.x * b, a.y * b, a.z * b);
}

inline Vec3 operator*(float a, const Vec3& b) {
	return Vec3(a * b.x, a * b.y, a * b.z);
}

inline Vec3 operator/(const Vec3& a, float b) {
	return Vec3(a.x / b, a.y / b, a.z / b);
}

inline float dot(const Vec3& a, const Vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
	return Vec3(a.y * b.z - b.y * a.z,
				b.x * a.z - a.x * b.z,
				a.x * b.y - b.x * a.y);
}

inline float mag(const Vec3& a) {
	return std::sqrt(dot(a, a));
}

inline float sqrMag(const Vec3& a) {
	return dot(a, a);
}

inline Vec3 normalize(const Vec3& a) {
	float len = mag(a);
	if (len > 0.00001f) {
		return a / len;
	}
	return a;
}