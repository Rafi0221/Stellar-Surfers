#include "shader.h"

#include "../opengl/gl.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLFunctions>

std::string getShaderCode(const std::string path){
    std::ifstream f(path);
    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();
    return buffer.str();
}

Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
{
    std::string vertexCodeString = getShaderCode(vertexPath);
    std::string fragmentCodeString = getShaderCode(fragmentPath);

    const char *vertexCode = vertexCodeString.c_str();
    const char *fragmentCode = fragmentCodeString.c_str();

    unsigned int vertexProgram, fragmentProgram;

    vertexProgram = GL::funcs.glCreateShader(GL_VERTEX_SHADER);
    GL::funcs.glShaderSource(vertexProgram, 1, &vertexCode, NULL);
    GL::funcs.glCompileShader(vertexProgram);

    fragmentProgram = GL::funcs.glCreateShader(GL_FRAGMENT_SHADER);
    GL::funcs.glShaderSource(fragmentProgram, 1, &fragmentCode, NULL);
    GL::funcs.glCompileShader(fragmentProgram);

    ID = GL::funcs.glCreateProgram();
    GL::funcs.glAttachShader(ID, vertexProgram);
    GL::funcs.glAttachShader(ID, fragmentProgram);
    GL::funcs.glLinkProgram(ID);

    GL::funcs.glDeleteShader(vertexProgram);
    GL::funcs.glDeleteShader(fragmentProgram);
}

void Shader::use(){
    GL::funcs.glUseProgram(ID);
}

void Shader::setInt(const std::string name, int value){
    GL::funcs.glUniform1i(GL::funcs.glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string name, float value){
    GL::funcs.glUniform1f(GL::funcs.glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec3(const std::string name, QVector3D &value){
    GL::funcs.glUniform3fv(GL::funcs.glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setMat4(const std::string name, QMatrix4x4 &value){
    GL::funcs.glUniformMatrix4fv(GL::funcs.glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value.data());
}
