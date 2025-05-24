#include "stdafx.h"
#include "SceneManager.h"
#include <ctime>


Fire::Fire() {
	lastClock = clock();
	u_Time = 0.0f;
	speedFactor = 0.05f;
	u_DispMax = 1.0f;
}

void Fire::Update(float deltaTime) {
    clock_t now = clock();
    float elapsed = float(now - lastClock) / CLOCKS_PER_SEC;
    lastClock = now;
    u_Time += elapsed * speedFactor;
    u_Time = u_Time - floor(u_Time);
}

void Fire::Draw(ESContext* esContext) {
    glUseProgram(shader->program);
    GLint timeLoc = glGetUniformLocation(shader->program, "u_Time");
    GLint dispMaxLoc = glGetUniformLocation(shader->program, "u_DispMax");
    if (timeLoc != -1) glUniform1f(timeLoc, u_Time);
    if (dispMaxLoc != -1) glUniform1f(dispMaxLoc, u_DispMax);

    for (int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->textureId);
        glUniform1i(shader->textureUniform[i], i);
    }

    sendCommonData(esContext);
    sendSpecificData(esContext);

    glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_SHORT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}