#pragma once
#ifndef DRAW_ARMS_AND_MONITORS_H
#define DRAW_ARMS_AND_MONITORS_H

void drawArmsAndMonitors(float tableHeight, float tableDepth) {
    GLint specularIntensityLoc = glGetUniformLocation(gProgramId, "secularIntensity");
    glm::mat4 rotation;
    glm::mat4 translation;
    glm::mat4 model;
    float poleHeight = 1.0;
    float poleRadius = 0.05;
    float monitorHeight = 1.0;
    float monitorWidth = 2.0;
    float monitorDepth = 0.05;
    float spaceBetweenTableAndMonitor = 0.4;
    float monitorCenterY = (monitorHeight / 2.0) + tableHeight + spaceBetweenTableAndMonitor;
    float monitorXDistanceFromCenter = monitorWidth / 2.0 - 0.1;
    float monitorZDistanceFromCenter = -1 * (tableDepth / 2.0 - 0.4);

    float poleCenterY = (-1 * tableHeight) - poleHeight / 2.0;

    // DRAW ARMS

    // The arms should have high specular light
    glUniform1f(specularIntensityLoc, 0.8);


    // center pole
    updateTexture(TextureKey::dark, 1.0f, 1.0f);
    rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    updateModel(rotation);
    Shape cylinder = Cylinder::Create(0.0, -1 * (tableDepth/2.0 - 0.1) , poleCenterY, poleRadius, poleHeight);
    cylinder.Draw();
    resetModel();

    // Left arm
    rotation = glm::rotate(glm::radians(-65.0f), glm::vec3(0.0, 1.0, 0.0f));
    translation = glm::translate(glm::vec3(-0.8, monitorCenterY, 0.1));
    model = rotation * translation;
    updateModel(model);
    Shape leftArm = Cylinder::Create(0.0, 0.0, 0.0, poleRadius, 1.0);
    leftArm.Draw();
    resetModel();

    // Left ball connection between arm and monitor
    Shape leftSphere = Sphere::Create(-1 * monitorXDistanceFromCenter, monitorCenterY, monitorZDistanceFromCenter + 0.13, 0.05);
    leftSphere.Draw();

    // right arm
    rotation = glm::rotate(glm::radians(-115.0f), glm::vec3(0.0, 1.0, 0.0f));
    translation = glm::translate(glm::vec3(-0.8, monitorCenterY, -0.1));
    model = rotation * translation;
    updateModel(model);
    Shape rightArm = Cylinder::Create(0.0, 0.0, 0.0, poleRadius, 1.0);
    rightArm.Draw();
    resetModel();

    // Right ball connection between arm and monitor
    Shape rightSphere = Sphere::Create(monitorXDistanceFromCenter, monitorCenterY, monitorZDistanceFromCenter + 0.13, 0.05);
    rightSphere.Draw();

    // center sphere on top of pole
    Shape centerSphere = Sphere::Create(0.0, -1 * (poleCenterY - poleHeight / 2.0) -0.01, -1 * (tableDepth/2.0 - 0.1) , 0.05);
    centerSphere.Draw();

    // MONITORS

    // left monitor
    Shape leftMonitor = Cuboid::Create(-1.0 * monitorXDistanceFromCenter, monitorCenterY, monitorZDistanceFromCenter, monitorDepth, monitorWidth, monitorHeight);

    rotation = glm::rotate(glm::radians(11.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    updateModel(rotation);
    leftMonitor.Draw();

    // left screen
    rotation = rotation * glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    updateModel(rotation);

    Shape leftScreen = Plane::Create(-1.0 * monitorXDistanceFromCenter, monitorZDistanceFromCenter + monitorDepth/2.0 + 0.001, -1 * monitorCenterY, monitorHeight, monitorWidth, 0.195);

    updateTexture(TextureKey::left_screen, 1.0f, 1.0f);

    // The screen should have high specular light
    glUniform1f(specularIntensityLoc, 1.0);
    leftScreen.Draw();

    resetModel();

    // right monitor

    // The monitors should have high specular light, but not as high as the screens
    glUniform1f(specularIntensityLoc, 0.8);

    updateTexture(TextureKey::dark, 1.0f, 1.0f);

    Shape rightMonitor = Cuboid::Create(monitorXDistanceFromCenter, monitorCenterY, monitorZDistanceFromCenter, monitorDepth, monitorWidth, monitorHeight);

    rotation = glm::rotate(glm::radians(11.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    updateModel(rotation);
    rightMonitor.Draw();

    // Right screen

    // The screen should have high specular light
    glUniform1f(specularIntensityLoc, 1.0);
    updateTexture(TextureKey::right_screen, 1.0f, 1.0f);
    rotation = rotation * glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    updateModel(rotation);

    Shape rightScreen = Plane::Create(monitorXDistanceFromCenter, monitorZDistanceFromCenter + monitorDepth / 2.0 + 0.001, -1 * monitorCenterY, monitorHeight, monitorWidth);
    rightScreen.Draw();
    resetModel();


}

#endif