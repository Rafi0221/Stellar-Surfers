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

Shader::Shader(const std::string vertexPath, const std::string fragmentPath,
               const std::vector<std::string> vertexIncludes, const std::vector<std::string> fragmentIncludes)
{
    std::string vertexCodeString;
    for(int i = 0; i < vertexIncludes.size(); i++){
        vertexCodeString.append(getShaderCode(vertexIncludes[i]));
    }

    std::string fragmentCodeString;
    for(int i = 0; i < fragmentIncludes.size(); i++){
        fragmentCodeString.append(getShaderCode(fragmentIncludes[i]));
    }

    vertexCodeString.append(getShaderCode(vertexPath));
    fragmentCodeString.append(getShaderCode(fragmentPath));

    const char *vertexCode = vertexCodeString.c_str();
    const char *fragmentCode = fragmentCodeString.c_str();

    unsigned int vertexProgram, fragmentProgram;

    vertexProgram = GL::funcs.glCreateShader(GL_VERTEX_SHADER);
    GL::funcs.glShaderSource(vertexProgram, 1, &vertexCode, NULL);
    GL::funcs.glCompileShader(vertexProgram);

    fragmentProgram = GL::funcs.glCreateShader(GL_FRAGMENT_SHADER);
    GL::funcs.glShaderSource(fragmentProgram, 1, &fragmentCode, NULL);
    GL::funcs.glCompileShader(fragmentProgram);

    int success;
    char infoLog[1024];
    GL::funcs.glGetShaderiv(vertexProgram, GL_COMPILE_STATUS, &success);
    if(!success){
        GL::funcs.glGetShaderInfoLog(vertexProgram , 1024, NULL, infoLog);
        qDebug() << infoLog;
    }

    GL::funcs.glGetShaderiv(fragmentProgram, GL_COMPILE_STATUS, &success);
    if(!success){
        GL::funcs.glGetShaderInfoLog(fragmentProgram , 1024, NULL, infoLog);
        qDebug() << infoLog;
    }

    ID = GL::funcs.glCreateProgram();
    GL::funcs.glAttachShader(ID, vertexProgram);
    GL::funcs.glAttachShader(ID, fragmentProgram);
    GL::funcs.glLinkProgram(ID);

    GL::funcs.glDeleteShader(vertexProgram);
    GL::funcs.glDeleteShader(fragmentProgram);
}

unsigned int Shader::getID(){
    return ID;
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
void Shader::setVec3(const std::string name, QVector3D value){
    GL::funcs.glUniform3fv(GL::funcs.glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setMat4(const std::string name, QMatrix4x4 value){
    GL::funcs.glUniformMatrix4fv(GL::funcs.glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value.data());
}
