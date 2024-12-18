#pragma once

class AABB {
public:
	Interval x, y, z;

	AABB() {}
	AABB(const Interval& x, const Interval& y, const Interval& z)
		: x(x), y(y), z(z) {}
	AABB(const Point3& a, const Point3& b) {
		x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
		y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
		z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);
	}
	AABB(const AABB& box0, const AABB& box1) {
		x = Interval(box0.x, box1.x);
		y = Interval(box0.y, box1.y);
		z = Interval(box0.z, box1.z);
	}

	const Interval& axisInterval(int n) const {
		if (n == 2) return z;
		if (n == 1) return y;
		return x;
	}

	bool hit(const Ray& ray, Interval rayT) const {
		const Point3& rayOrigin = ray.origin;
		const Vec3& rayDir = ray.dir;

		for (int axis = 0; axis < 3; axis++) {
			const Interval& ax = axisInterval(axis);
			const float axDirInv = 1.0f / rayDir[axis];

			float t0 = (ax.min - rayOrigin[axis]) * axDirInv;
			float t1 = (ax.max - rayOrigin[axis]) * axDirInv;

			if (t0 < t1) {
				if (t0 > rayT.min) rayT.min = t0;
				if (t1 < rayT.max) rayT.max = t1;
			}
			else {
				if (t1 > rayT.min) rayT.min = t1;
				if (t0 < rayT.min) rayT.max = t0;
			}

			if (rayT.max <= rayT.min) return false;
		}

		return true;
	}

	int longestAxis() const {
		if (x.size() > y.size()) {
			return x.size() > z.size() ? 0 : 2;
		}
		else {
			return y.size() > z.size() ? 1 : 2;
		}
	}

	static const AABB Empty, Universe;
};

const AABB AABB::Empty = AABB(Interval::Empty, Interval::Empty, Interval::Empty);
const AABB AABB::Universe = AABB(Interval::Universe, Interval::Universe, Interval::Universe);