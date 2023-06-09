#include "skybox.h"

#include "../utils/layeredperlinnoise.h"
#include "../utils/perlinnoise.h"
#include "../utils/shader.h"
#include "../utils/shadermanager.h"
#include "../opengl/gl.h"

#include <QOpenGLFunctions_3_3_Core>

SkyBox::SkyBox(PerlinNoise *noise)
{
    float vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    Shader *shader = ShaderManager::getShader("skyboxShader");
    shader->use();

    float tmpVertices[36];
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++){
            tmpVertices[j * 6] = vertices[i * 18 + j * 3];
            tmpVertices[j * 6 + 1] = vertices[i * 18 + j * 3 + 1];
            tmpVertices[j * 6 + 2] = vertices[i * 18 + j * 3 + 2];

            tmpVertices[j * 6 + 3] = quadVertices[j * 3];
            tmpVertices[j * 6 + 4] = quadVertices[j * 3 + 1];
            tmpVertices[j * 6 + 5] = quadVertices[j * 3 + 2];
        }

        unsigned int noiseTexture = noise->getPermutationTexture();
        shader->setInt("permutation", 0);
        GL::funcs.glActiveTexture(GL_TEXTURE0);
        GL::funcs.glBindTexture(GL_TEXTURE_1D, noiseTexture);

        unsigned int tmpVAO, tmpVBO;
        GL::funcs.glGenVertexArrays(1, &tmpVAO);
        GL::funcs.glGenBuffers(1, &tmpVBO);

        GL::funcs.glBindVertexArray(tmpVAO);

        GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
        GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(tmpVertices), tmpVertices, GL_STATIC_DRAW);

        GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        GL::funcs.glEnableVertexAttribArray(0);
        GL::funcs.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
        GL::funcs.glEnableVertexAttribArray(1);

        unsigned int fbo;
        GL::funcs.glGenFramebuffers(1, &fbo);
        GL::funcs.glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        unsigned int quadTexture;
        GL::funcs.glGenTextures(1, &quadTexture);
        GL::funcs.glBindTexture(GL_TEXTURE_2D, quadTexture);

        GL::funcs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 2000, 2000, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GL::funcs.glBindTexture(GL_TEXTURE_2D, 0);

        GL::funcs.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, quadTexture, 0);

        GL::funcs.glViewport(0,0,2000,2000);
        GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 6);

        textures[i] = quadTexture;

        GL::funcs.glDeleteBuffers(1,&tmpVBO);
        GL::funcs.glDeleteVertexArrays(1, &tmpVAO);
        GL::funcs.glDeleteFramebuffers(1, &fbo);
    }

    float textureCoords[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    float sideVertices[30];
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++){
            sideVertices[j * 5] = vertices[i * 18 + j * 3];
            sideVertices[j * 5 + 1] = vertices[i * 18 + j * 3 + 1];
            sideVertices[j * 5 + 2] = vertices[i * 18 + j * 3 + 2];

            sideVertices[j * 5 + 3] = textureCoords[j * 2];
            sideVertices[j * 5 + 4] = textureCoords[j * 2 + 1];
        }

        GL::funcs.glGenVertexArrays(1, &VAO[i]);
        GL::funcs.glGenBuffers(1, &VBO[i]);

        GL::funcs.glBindVertexArray(VAO[i]);

        GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(sideVertices), sideVertices, GL_STATIC_DRAW);

        GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        GL::funcs.glEnableVertexAttribArray(0);
        GL::funcs.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
        GL::funcs.glEnableVertexAttribArray(1);
    }

//    GL::funcs.glGenVertexArrays(1, &VAO);
//    GL::funcs.glGenBuffers(1, &VBO);

//    GL::funcs.glBindVertexArray(VAO);

//    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    GL::funcs.glEnableVertexAttribArray(0);

//    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);

//    GL::funcs.glBindVertexArray(0);
}

void SkyBox::render(){
    Shader *shader = ShaderManager::getShader("textureShader");
    shader->use();
    shader->setInt("texture1", 0);
    for(int i = 0; i < 6; i++){
        GL::funcs.glActiveTexture(GL_TEXTURE0);
        GL::funcs.glBindTexture(GL_TEXTURE_2D, textures[i]);

        GL::funcs.glBindVertexArray(VAO[i]);
        GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 6);
    }

//    GL::funcs.glBindVertexArray(VAO);
//    GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 36);
}
