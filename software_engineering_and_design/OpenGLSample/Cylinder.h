#pragma once
#ifndef CYLINDER_H
#define CYLINDER_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>


std::vector<float> getUnitCircleVertices(int sectorCount) {
	const float PI = 3.1415926f;
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle;  // radian

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); // x
		unitCircleVertices.push_back(sin(sectorAngle)); // y
		unitCircleVertices.push_back(0);                // z
	}
	return unitCircleVertices;
}

namespace Cylinder
{
	Shape Create(float centerX, float centerY, float centerZ, float radius, float length)
	{
		int sectorCount = 36;
		std::vector<float> vertices;
		std::vector<int> indices;

		// get unit circle vectors on XY-plane
		std::vector<float> unitVertices = getUnitCircleVertices(sectorCount);

		// put side vertices to arrays
		for (int i = 0; i < 2; ++i)
		{
			float h = -length / 2.0f + i * length;           // z value; -h/2 to h/2
			float t = 1.0f - i;                              // vertical tex coord; 1 to 0

			for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
			{
				float ux = unitVertices[k];
				float uy = unitVertices[k + 1];
				float uz = unitVertices[k + 2];
				// position vector
				vertices.push_back(ux * radius + centerX);
				vertices.push_back(uy * radius + centerY);
				vertices.push_back(h + centerZ);
				// normal vector
				vertices.push_back(ux + centerX);
				vertices.push_back(uy + centerY);
				vertices.push_back(uz + centerZ);
				// texture coordinate
				vertices.push_back((float)j / sectorCount); 
				vertices.push_back(t);
			}
		}

		// the starting index for the base/top surface
		//NOTE: it is used for generating indices later
		int baseCenterIndex = (int)vertices.size() / 3;
		int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

		// put base and top vertices to arrays
		for (int i = 0; i < 2; ++i)
		{
			float h = -length / 2.0f + i * length;           // z value; -h/2 to h/2
			float nz = -1 + i * 2;                           // z value of normal; -1 to 1

			// center point
			vertices.push_back(centerX);     vertices.push_back(centerY);     vertices.push_back(h + centerZ);
			vertices.push_back(centerX);      vertices.push_back(centerY);      vertices.push_back(nz + centerZ);
			vertices.push_back(0.5f); vertices.push_back(0.5f);

			for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
			{
				float ux = unitVertices[k];
				float uy = unitVertices[k + 1];
				// position vector
				vertices.push_back(ux * radius + centerX);
				vertices.push_back(uy * radius + centerY);
				vertices.push_back(h + centerZ);
				// normal vector
				vertices.push_back(centerX);
				vertices.push_back(centerY);
				vertices.push_back(nz + centerZ);
				// texture coordinate
				vertices.push_back(-ux * 0.5f + 0.5f);
				vertices.push_back(-uy * 0.5f + 0.5f); 
			}
		}

		// generate CCW index list of cylinder triangles
		int k1 = 0;                         // 1st vertex index at base
		int k2 = sectorCount + 1;           // 1st vertex index at top

		// indices for the side surface
		for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
		{
			// 2 triangles per sector
			// k1 => k1+1 => k2
			indices.push_back(k1);
			indices.push_back(k1 + 1);
			indices.push_back(k2);

			// k2 => k1+1 => k2+1
			indices.push_back(k2);
			indices.push_back(k1 + 1);
			indices.push_back(k2 + 1);
		}

		// indices for the base surface
		//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
		//      please see the previous code snippet
		for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
		{
			if (i < sectorCount - 1)
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(k + 1);
				indices.push_back(k);
			}
			else // last triangle
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(baseCenterIndex + 1);
				indices.push_back(k);
			}
		}

		// indices for the top surface
		for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
		{
			if (i < sectorCount - 1)
			{
				indices.push_back(topCenterIndex);
				indices.push_back(k);
				indices.push_back(k + 1);
			}
			else // last triangle
			{
				indices.push_back(topCenterIndex);
				indices.push_back(k);
				indices.push_back(topCenterIndex + 1);
			}
		}

		return Shape(vertices, indices);
	}
};


#endif
