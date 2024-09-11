#pragma once

struct Interval {
	float min, max;

	Interval() : min(+infinity), max(-infinity) {}
	Interval(float min, float max) : min(min), max(max) {}

	float size() const { return max - min; }
	bool contains(float x) const { return min <= x && x <= max; }
	bool surrounds(float x) const { return min < x && x < max; }

	static const Interval Empty, Universe;
};

const Interval Interval::Empty = Interval(+infinity, -infinity);
const Interval Interval::Universe = Interval(-infinity, +infinity);