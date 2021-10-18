#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>


const int SECTOR_COUNT = 36;
const int STACK_COUNT = 18;

namespace Sphere {
	Shape Create(float centerX, float centerY, float centerZ, float radius)
	{
		std::vector<float> vertices;
		std::vector<int> indices;

		/* GENERATE VERTEX ARRAY */
		float x, y, z, xy;                              // vertex position
		float lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = (float)(2 * M_PI / SECTOR_COUNT);
		float stackStep = (float)(M_PI / STACK_COUNT);
		float sectorAngle, stackAngle;

		for (int i = 0; i <= STACK_COUNT; ++i)
		{
			stackAngle = (float)(M_PI / 2 - i * stackStep);        // starting from pi/2 to -pi/2
			xy = 1.02f * radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

														// add (SECTOR_COUNT+1) vertices per stack
														// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= SECTOR_COUNT; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

														// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);				// r * cos(u) * sin(v)
				vertices.push_back(x + centerX);
				vertices.push_back(y + centerY);
				vertices.push_back(z + centerZ);

				// normal vector
				vertices.push_back(x + centerX);
				vertices.push_back(y + centerY);
				vertices.push_back(z + centerZ);

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / SECTOR_COUNT;
				t = (float)i / STACK_COUNT;
				vertices.push_back(s);
				vertices.push_back(t);

			}
		}
		/* GENERATE VERTEX ARRAY */


		/* GENERATE INDEX ARRAY */
		int k1, k2;
		for (int i = 0; i < STACK_COUNT; ++i)
		{
			k1 = i * (SECTOR_COUNT + 1);     // beginning of current stack
			k2 = k1 + SECTOR_COUNT + 1;      // beginning of next stack

			for (int j = 0; j < SECTOR_COUNT; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (STACK_COUNT - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		return Shape(vertices, indices);
	}
};


#endif
