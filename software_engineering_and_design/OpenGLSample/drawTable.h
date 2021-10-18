#pragma once
#ifndef DRAW_TABLE_H
#define DRAW_TABLE_H

void drawTable(float tableHeight, float tableDepth) {
	float tableWidth = 4;

	float footWidth = 0.2;
	float footHeight = 0.1;
	float legWidth = 0.1;
	float legDepth = 0.2;
	float legBackPos = (tableDepth / -2.0) / 2.0;


	float tableTopThickness = 0.05;


	// The feet/legs should have high specular light as they are very shiny
	GLint specularIntensityLoc = glGetUniformLocation(gProgramId, "specularIntensity");
	glUniform1f(specularIntensityLoc, 1.0);
	// FEET/LEGS
	updateTexture(TextureKey::off_white, 1.0f, 1.0f);
	// --- LEFT FOOT/LEG ---
	Shape leftFoot = Cuboid::Create((tableWidth/-2.0) + (footWidth/2.0), footHeight/2.0, 0, tableDepth, footWidth, footHeight);
	leftFoot.Draw();
	Shape leftLeg = Cuboid::Create((tableWidth / -2.0) + (footWidth / 2.0), tableHeight / 2 - 0.001, legBackPos, legDepth, legWidth, tableHeight);
	leftLeg.Draw();
	// --- RIGHT FOOT/LEG ---
	Shape rightFoot = Cuboid::Create((tableWidth / 2.0) - (footWidth / 2.0), footHeight / 2.0, 0, tableDepth, footWidth, footHeight);
	rightFoot.Draw();
	Shape rightLeg = Cuboid::Create((tableWidth / 2.0) - (footWidth / 2.0), tableHeight / 2 - 0.001, legBackPos, legDepth, legWidth, tableHeight);
	rightLeg.Draw();

	// The table top should have moderate specular light.
	glUniform1f(specularIntensityLoc, 6.0);
	// TABLE TOP
	updateTexture(TextureKey::wood, 2.0f, 2.0f);
	Shape tableTop = Cuboid::Create(0, tableHeight - (tableTopThickness / 2.0), 0, tableDepth, tableWidth, tableTopThickness);
	tableTop.Draw();

}

#endif