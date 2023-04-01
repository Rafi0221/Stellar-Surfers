#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <QOpenGLFunctions>

#include <QVector3D>
#include <QMatrix4x4>

std::string getShaderCode(const std::string path){
    std::ifstream f(path);
    std::stringstream buffer;
    buffer << f.rdbuf();
    f.close();
    return buffer.str();
}

Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
{
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();

    std::string vertexCodeString = getShaderCode(vertexPath);
    std::string fragmentCodeString = getShaderCode(fragmentPath);

    const char *vertexCode = vertexCodeString.c_str();
    const char *fragmentCode = fragmentCodeString.c_str();

    unsigned int vertexProgram, fragmentProgram;

    vertexProgram = glFuncs->glCreateShader(GL_VERTEX_SHADER);
    glFuncs->glShaderSource(vertexProgram, 1, &vertexCode, NULL);
    glFuncs->glCompileShader(vertexProgram);

    fragmentProgram = glFuncs->glCreateShader(GL_FRAGMENT_SHADER);
    glFuncs->glShaderSource(fragmentProgram, 1, &fragmentCode, NULL);
    glFuncs->glCompileShader(fragmentProgram);

    ID = glFuncs->glCreateProgram();
    glFuncs->glAttachShader(ID, vertexProgram);
    glFuncs->glAttachShader(ID, fragmentProgram);
    glFuncs->glLinkProgram(ID);

    glFuncs->glDeleteShader(vertexProgram);
    glFuncs->glDeleteShader(fragmentProgram);
}

void Shader::use(){
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glUseProgram(ID);
}

void Shader::setInt(const std::string name, int value){
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glUniform1i(glFuncs->glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string name, float value){
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glUniform1f(glFuncs->glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string name, QVector3D &value){
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glUniform3fv(glFuncs->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setMat4(const std::string name, QMatrix4x4 &value){
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glUniformMatrix4fv(glFuncs->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value.data());
}
