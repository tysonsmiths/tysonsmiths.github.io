#pragma once
#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

void updateTexture(TextureKey texture_key, GLfloat scaleX, GLfloat scaleY, bool isExtra = false) {
    glm::vec2 uVScale(scaleX, scaleY);
    // If isExtra bool is set, then we want to activate the second (1) texture, as wel as allow set the extra uv scale
    if (isExtra) {
        GLint UVScaleLoc = glGetUniformLocation(gProgramId, "uvScaleExtra");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(uVScale));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureMap[texture_key]);
    }
    else {
        GLint UVScaleLoc = glGetUniformLocation(gProgramId, "uvScaleBase");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(uVScale));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureMap[texture_key]);
    }
}

void updateModel(glm::mat4 model) {
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void resetModel() {
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * scale;
    updateModel(model);
}
#endif
