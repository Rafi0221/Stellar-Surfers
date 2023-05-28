#include "model.h"

#include <qopenglfunctions_3_3_core.h>

#include <string>

#include "obj_loader.h"
#include "shader.h"
#include "../opengl/gl.h"

Model::Model(std::string path, std::string diffusePath, std::string specularPath, std::string normalPath){
    size = load_obj(path, VAO);
    if(diffusePath != "")
        diffuseTexture = load_texture(diffusePath);
    if(specularPath != "")
        specularTexture = load_texture(specularPath);
    if(normalPath != "")
        normalTexture = load_texture(normalPath);
}

unsigned int Model::getVAO(){
    return VAO;
}

unsigned int Model::getSize(){
    return size;
}

void Model::setupShader(Shader *shader){
    shader->use();
    if(diffuseTexture != -1){
        GL::funcs.glActiveTexture(GL_TEXTURE0);
        GL::funcs.glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        shader->setInt("textureDiffuse", 0);
    }
    if(specularTexture != -1){
        GL::funcs.glActiveTexture(GL_TEXTURE1);
        GL::funcs.glBindTexture(GL_TEXTURE_2D, specularTexture);
        shader->setInt("textureSpecular", 1);
    }
    if(normalTexture != -1){
        GL::funcs.glActiveTexture(GL_TEXTURE2);
        GL::funcs.glBindTexture(GL_TEXTURE_2D, normalTexture);
        shader->setInt("textureNormal", 2);
    }
}

