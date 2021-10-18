#pragma once
#ifndef CUBOID_H
#define CUBOID_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Cuboid {
	Shape Create(float centerX, float centerY, float centerZ, float length, float width, float height)
	{
		// Because we're adding/subtracting from the center, these values should be half what's passed in.
		float wHalf = width / 2.0;
		float hHalf = height / 2.0;
		float lHalf = length / 2.0;

		std::vector<float> vertices = {
			// NOTE: in order to have the textures display correctly and facing the right directions,
			// the coordinates are duplicated such that each coordinate is only used for a single face.
			// 
			//			x,y,z position coordinates								  // x,y texture coordinates	 
			// back
			centerX - wHalf, centerY + hHalf, centerZ - lHalf, /* 0: left, top, back */ 0.0, 0.0, -1.0, /**/ 0.0, 1.0,
			centerX + wHalf, centerY + hHalf, centerZ - lHalf, /* 1: right, top, back */ 0.0, 0.0, -1.0, /**/ 1.0, 1.0,
			centerX - wHalf, centerY - hHalf, centerZ - lHalf, /* 2: left, bottom, back */ 0.0, 0.0, -1.0, /**/ 0.0, 0.0,
			centerX + wHalf, centerY - hHalf, centerZ - lHalf, /* 3: right, bottom, back */ 0.0, 0.0, -1.0, /**/ 1.0, 0.0,

			// front
			centerX - wHalf, centerY + hHalf, centerZ + lHalf, /* 4: left, top, front */ 0.0, 0.0, 1.0, /**/ 0.0, 1.0,
			centerX + wHalf, centerY + hHalf, centerZ + lHalf, /* 5: right, top, front */ 0.0, 0.0, 1.0, /**/ 1.0, 1.0,
			centerX - wHalf, centerY - hHalf, centerZ + lHalf, /* 6: left, bottom, front */ 0.0, 0.0, 1.0, /**/ 0.0, 0.0,
			centerX + wHalf, centerY - hHalf, centerZ + lHalf, /* 7: right, bottom, front */ 0.0, 0.0, 1.0, /**/ 1.0, 0.0,

			// left
			centerX - wHalf, centerY + hHalf, centerZ - lHalf, /* 8: left, top, back */ -1.0, 0.0, 0.0, /**/ 0.0, 1.0,
			centerX - wHalf, centerY + hHalf, centerZ + lHalf, /* 9: left, top, front */ -1.0, 0.0, 0.0, /**/ 1.0, 1.0,
			centerX - wHalf, centerY - hHalf, centerZ - lHalf, /* 10: left, bottom, back */ -1.0, 0.0, 0.0, /**/ 0.0, 0.0,
			centerX - wHalf, centerY - hHalf, centerZ + lHalf, /* 11: left, bottom, front */ -1.0, 0.0, 0.0, /**/ 1.0, 0.0,

			// right
			centerX + wHalf, centerY + hHalf, centerZ - lHalf, /* 12: right, top, back */ 1.0, 0.0, 0.0, /**/ 0.0, 1.0,
			centerX + wHalf, centerY + hHalf, centerZ + lHalf, /* 13: right, top, front */ 1.0, 0.0, 0.0, /**/ 1.0, 1.0,
			centerX + wHalf, centerY - hHalf, centerZ - lHalf, /* 14: right, bottom, back */ 1.0, 0.0, 0.0, /**/ 0.0, 0.0,
			centerX + wHalf, centerY - hHalf, centerZ + lHalf, /* 15: right, bottom, front */ 1.0, 0.0, 0.0, /**/ 1.0, 0.0,

			// top
			centerX - wHalf, centerY + hHalf, centerZ - lHalf, /* 16: left, top, back */ 0.0, 1.0, 0.0, /**/ 0.0, 1.0,
			centerX + wHalf, centerY + hHalf, centerZ - lHalf, /* 17: right, top, back */ 0.0, 1.0, 0.0, /**/ 1.0, 1.0,
			centerX - wHalf, centerY + hHalf, centerZ + lHalf, /* 18: left, top, front */ 0.0, 1.0, 0.0, /**/ 0.0, 0.0,
			centerX + wHalf, centerY + hHalf, centerZ + lHalf, /* 19: right, top, front */ 0.0, 1.0, 0.0, /**/ 1.0, 0.0,

			// bottom
			centerX - wHalf, centerY - hHalf, centerZ - lHalf, /* 20: left, bottom, back */ 0.0, -1.0, 0.0, /**/ 0.0, 1.0,
			centerX + wHalf, centerY - hHalf, centerZ - lHalf, /* 21: right, bottom, back */ 0.0, -1.0, 0.0, /**/ 1.0, 1.0,
			centerX - wHalf, centerY - hHalf, centerZ + lHalf, /* 22: left, bottom, front */ 0.0, -1.0, 0.0, /**/ 0.0, 0.0,
			centerX + wHalf, centerY - hHalf, centerZ + lHalf, /* 23: right, bottom, front */ 0.0, -1.0, 0.0, /**/ 1.0, 0.0,
		};

		std::vector<int> indices = {
			// back face
			0, 1, 2,
			2, 3, 1,

			// front face
			4, 5, 6,
			6, 7, 5,

			// left face
			8, 9, 10,
			10, 11, 9,

			// right face
			12, 13, 14,
			14, 15, 13,

			// top face
			16, 17, 18,
			18, 19, 17,

			// bottom face
			20, 21, 22,
			22, 23, 21,
		};

		return Shape(vertices, indices);

	}
};


#endif
