#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include <random>
#include <chrono>

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;

// Utility Functions

inline float degreesToRadians(float degrees) {
	return degrees * pi / 180.0f;
}

inline float randomFloat() {
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float randomFloat(float min, float max) {
	static std::uniform_real_distribution<float> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
}

inline int randomInt(int min, int max) {
	return int(randomFloat(min, max + 1));
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"