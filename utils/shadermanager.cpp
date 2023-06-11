#include "shadermanager.h"

#include "../opengl/gl.h"
#include "shader.h"

#include <QOpenGLFunctions_3_3_Core>

std::map<std::string, Shader*> ShaderManager::shaders;

void ShaderManager::initialize(){
    shaders["testShader"] = new Shader("shaders/testShader.vs", "shaders/testShader.fs");
    shaders["terrainShader"] = new Shader("shaders/terrainShader.vs", "shaders/terrainShader.fs");
    shaders["waterShader"] = new Shader("shaders/waterShader.vs", "shaders/waterShader.fs");
    shaders["skyboxShader"] = new Shader("shaders/skyboxShader.vs", "shaders/skyboxShader.fs", {} , {"shaders/perlin.fs"});
    shaders["normalMapShader"] = new Shader("shaders/normalMap.vs", "shaders/normalMap.fs");
    shaders["positionTextureShader"] = new Shader("shaders/positionTexture.vs", "shaders/positionTexture.fs", {}, {"shaders/perlin.fs"});
    shaders["asteroidShader"] = new Shader("shaders/asteroidShader.vs", "shaders/asteroidShader.fs");
    shaders["textureShader"] = new Shader("shaders/textureShader.vs", "shaders/textureShader.fs");
    shaders["laserShader"] = new Shader("shaders/laserShader.vs", "shaders/laserShader.fs");

    Shader *waterShader = shaders["waterShader"];
    waterShader->use();
    unsigned int buffer_id = GL::funcs.glGetUniformBlockIndex(waterShader->getID(), "WaveBuffer");
    GL::funcs.glUniformBlockBinding(waterShader->getID(), buffer_id, 0);
}

Shader* ShaderManager::getShader(std::string name){
    return shaders[name];
}
