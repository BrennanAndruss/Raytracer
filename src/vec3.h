#pragma once

struct Vec3 {
	float x, y, z;

	float operator[](int i) const {
		if (i == 0) return x;
		if (i == 1) return y;
		if (i == 2) return z;
		throw std::out_of_range("Index out of range");
	}

	Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	static Vec3 random() {
		return Vec3(randomFloat(), randomFloat(), randomFloat());
	}

	static Vec3 random(float min, float max) {
		return Vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
	}

	bool nearZero() const {
		auto s = 1e-8;
		return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
	}

	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}

	Vec3 operator+=(const Vec3& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vec3 operator*=(float a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
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

inline Vec3 operator*(const Vec3& a, const Vec3& b) {
	return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
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

inline Vec3 randomUnitVector() {
	while (true) {
		Vec3 a = Vec3::random(-1.0f, 1.0f);
		float sqrLen = sqrMag(a);
		if (1e-160 < sqrLen && sqrLen <= 1.0f) {
			return a / std::sqrt(sqrLen);
		}
	}
}

inline Vec3 randomInUnitDisk() {
	while (true) {
		Point3 p = Vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), 0.0f);
		if (sqrMag(p) < 1.0f) {
			return p;
		}
	}
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
	Vec3 onUnitSphere = randomUnitVector();
	if (dot(onUnitSphere, normal) > 0.0f) {
		return onUnitSphere;
	}
	return -onUnitSphere;
}

inline Vec3 reflect(const Vec3& v, const Vec3& normal) {
	return v - 2.0f * dot(v, normal) * normal;
}

inline Vec3 refract(const Vec3& uv, const Vec3& normal, float refractionRatio) {
	float cosTheta = std::fmin(dot(-uv, normal), 1.0f);
	Vec3 rOutPerp = refractionRatio * (uv + cosTheta * normal);
	Vec3 rOutParallel = -std::sqrt(std::fabs(1.0f - sqrMag(rOutPerp))) * normal;
	return rOutPerp + rOutParallel;
}