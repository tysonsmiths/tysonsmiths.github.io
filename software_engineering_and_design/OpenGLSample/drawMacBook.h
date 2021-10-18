#pragma once
#ifndef DRAW_MACBOOK_H
#define DRAW_MACBOOK_H

void drawMacbook(float tableHeight) {
    glm::mat4 rotation;

    // The macbook should have high specular light
    GLint specularIntensityLoc = glGetUniformLocation(gProgramId, "secularIntensity");
    glUniform1f(specularIntensityLoc, 1.0);

    updateTexture(TextureKey::macbook_silver, 1.0f, 1.0f);
    float width = 0.8;
    float depth = 0.5;

    float baseThickness = 0.02;
    float screenThickness = 0.008;
    float baseCenterZ = 0.3;
    float baseCenterY = tableHeight + (baseThickness / 2.0);

    // Draw laptop base
    Shape base = Cuboid::Create(0.0, baseCenterY, baseCenterZ, depth, width, baseThickness);
    base.Draw();

    // Draw laptop screen
    Shape screen = Cuboid::Create(0.0, baseCenterZ + 0.27, -1.0 * baseCenterY - depth/2.0 + 0.09, depth, width, screenThickness);

    rotation = glm::rotate(glm::radians(75.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    updateModel(rotation);
    screen.Draw();

    // Draw screen texture on screen
    updateTexture(TextureKey::mac_screen, 1.0f, 1.0f);
    Shape screenTexture = Plane::Create(0.0, baseCenterZ + 0.27 + screenThickness/2.0 + 0.001, -1.0 * baseCenterY - depth / 2.0 + 0.09, depth, width);
    screenTexture.Draw();

    resetModel();

    // Draw keyboard texture on base
    updateTexture(TextureKey::keyboard, 1.0f, 1.0f);
    Shape keyboard = Plane::Create(0.0, baseCenterY + baseThickness / 2.0 + 0.001, baseCenterZ, depth, width);
    keyboard.Draw();
}

#endif