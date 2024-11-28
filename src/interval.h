#pragma once

struct Interval {
	float min, max;

	Interval() : min(+infinity), max(-infinity) {}
	Interval(float min, float max) : min(min), max(max) {}
	Interval(const Interval& a, const Interval& b) {
		// Create the interval tightly enclosing the two given intervals
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= b.max ? a.max : b.max;
	}

	float size() const { return max - min; }
	bool contains(float x) const { return min <= x && x <= max; }
	bool surrounds(float x) const { return min < x && x < max; }
	float clamp(float x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
	Interval expand(float delta) const {
		auto padding = delta / 2;
		return Interval(min - padding, max + padding);
	}

	static const Interval Empty, Universe;
};

const Interval Interval::Empty = Interval(+infinity, -infinity);
const Interval Interval::Universe = Interval(-infinity, +infinity);