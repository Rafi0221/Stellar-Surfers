#include "shadermanager.h"

#include "shader.h"

std::map<std::string, Shader*> ShaderManager::shaders;

void ShaderManager::initialize(){
    shaders["testShader"] = new Shader("shaders/testShader.vs", "shaders/testShader.fs");
    shaders["terrainShader"] = new Shader("shaders/terrainShader.vs", "shaders/terrainShader.fs");
    shaders["skyboxShader"] = new Shader("shaders/skyboxShader.vs", "shaders/skyboxShader.fs", {} , {"shaders/perlin.fs"});
    shaders["normalMapShader"] = new Shader("shaders/normalMap.vs", "shaders/normalMap.fs");
    shaders["positionTextureShader"] = new Shader("shaders/positionTexture.vs", "shaders/positionTexture.fs", {}, {"shaders/perlin.fs"});
}

Shader* ShaderManager::getShader(std::string name){
    return shaders[name];
}
