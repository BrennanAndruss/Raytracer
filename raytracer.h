#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Common Headers

#include "color.h"
#include "ray.h"
#include "vec3.h"

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;

// Utility Functions

inline float degreesToRadians(float degrees) {
	return degrees * pi / 180.0f;
}