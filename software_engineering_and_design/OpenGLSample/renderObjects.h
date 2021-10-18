#pragma once
#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "drawTable.h"
#include "drawArmsAndMonitors.h"
#include "drawMacBook.h"


void renderObjects() {
    float tableHeight = 2.0;
    float tableDepth = 2.0;
    drawTable(tableHeight, tableDepth);

    drawArmsAndMonitors(tableHeight, tableDepth);

    drawMacbook(tableHeight);

    // -- DRAW PLANE --
    float planeSize = 10.0;
    float rugWidth = 3.0;
    // rug is 2 by 3 shaped.
    float carpetHeight = rugWidth * (2.0/3.0);
    GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
    glUniform1i(multipleTexturesLoc, true);
    GLint UVExtraLoc = glGetUniformLocation(gProgramId, "uvExtra");
    // center the rug on the plane
    glUniform1f(UVExtraLoc, -1 * (planeSize / (2.0 * rugWidth)) + 0.5);

    // The carpet should have no specular light
    GLint specularIntensityLoc = glGetUniformLocation(gProgramId, "specularIntensity");
    glUniform1f(specularIntensityLoc, 0.0);

    // base texture
    updateTexture(TextureKey::carpet, 5.0f, 5.0f);
    // extra texture
    updateTexture(TextureKey::rug, planeSize/rugWidth, planeSize/carpetHeight, true);
    Shape plane = Plane::Create(0.0, 0.0, 0.0, planeSize, planeSize);
    plane.Draw();
    glUniform1i(multipleTexturesLoc, false);
}



#endif