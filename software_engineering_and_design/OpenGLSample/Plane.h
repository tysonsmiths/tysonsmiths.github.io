#pragma once_flag
#pragma once
#ifndef PLANE_H
#define PLANE_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>



namespace Plane {
	Shape Create(float centerX, float centerY, float centerZ, float length, float width, float offset = 0.0)
	{
		// Because we're adding/subtracting from the center, these values should be half what's passed in.
		float wHalf = width / 2.0;
		float lHalf = length / 2.0;

		std::vector<float> vertices = {
			centerX - wHalf, centerY, centerZ - lHalf, /* 0: left, back */ 0.0, 1.0, 0.0, /**/ 0.0f - offset, 1.0,
			centerX + wHalf, centerY, centerZ - lHalf, /* 1: right, back */ 0.0, 1.0, 0.0, /**/ 1.0f - offset, 1.0,
			centerX - wHalf, centerY, centerZ + lHalf, /* 2: left, front */ 0.0, 1.0, 0.0, /**/ 0.0, 0.0,
			centerX + wHalf, centerY, centerZ + lHalf, /* 3: right, front */ 0.0, 1.0, 0.0, /**/ 1.0, 0.0,
		};

		std::vector<int> indices = {
			0, 1, 2,
			2, 3, 1,
		};

		return Shape(vertices, indices);
	}
};

#endif
