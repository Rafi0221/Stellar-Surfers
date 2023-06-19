#include "obj_loader.h"

#include <QDebug>
#include <QImage>
#include <QOpenGLFunctions_3_3_Core>
#include <QPixmap>
#include <QString>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../opengl/gl.h"

using namespace std;

int load_obj(string path, unsigned int &VAO){
    unsigned int VBO;

    vector<float> positions;
    vector<float> normals;
    vector<float> texCoords;

    vector<float> data;
    ifstream file(path);

    string line;
    while(getline(file, line)){
        istringstream iss(line);
        string type;
        iss >> type;
        float x,y,z;
        if(type == "v"){
            iss >> x >> y >> z;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }else if(type == "vt"){
            iss >> x >> y;
            texCoords.push_back(x);
            texCoords.push_back(y);
        }else if(type == "vn"){
            iss >> x >> y >> z;
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }else if(type == "f"){
            int v0, v1, v2;
            char c, c2;
            for(int i = 0; i < 3; i++){
                iss >> v0 >> c >> v1 >> c2 >> v2;
                data.push_back(positions[(v0-1) * 3]);
                data.push_back(positions[(v0-1) * 3 + 1]);
                data.push_back(positions[(v0-1) * 3 + 2]);

                data.push_back(normals[(v2-1) * 3]);
                data.push_back(normals[(v2-1) * 3 + 1]);
                data.push_back(normals[(v2-1) * 3 + 2]);

                data.push_back(texCoords[(v1-1) * 2]);
                data.push_back(texCoords[(v1-1) * 2 + 1]);
            }
        }else{
            continue;
        }
    }
    file.close();

    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &VBO);

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);
    GL::funcs.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(2);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);

    GL::funcs.glBindVertexArray(0);

    return data.size() / 8;

//    unsigned int VBO;

//    float vertices[] = {
//        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
//        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
//         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
//         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
//         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
//        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f
//    };

//        GL::funcs.glGenVertexArrays(1, &VAO);
//        GL::funcs.glGenBuffers(1, &VBO);

//        GL::funcs.glBindVertexArray(VAO);

//        GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//        GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        GL::funcs.glEnableVertexAttribArray(0);
//        GL::funcs.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
//        GL::funcs.glEnableVertexAttribArray(1);
//        GL::funcs.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
//        GL::funcs.glEnableVertexAttribArray(2);

//        GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);

//        GL::funcs.glBindVertexArray(0);

//        return 6;
}

unsigned int load_texture(std::string path){
    QImage img;
    img.load(QString::fromStdString(path), "PNG");
    img = img.convertToFormat(QImage::Format_RGBA8888);
    uchar *bits = img.bits();

    for(int y = 0; y < img.height()/2; y++){
        for(int x = 0; x < img.width(); x++){
            swap(bits[(y * img.width() + x) * 4], bits[((img.height()-y-1) * img.width() + x) * 4]);
            swap(bits[(y * img.width() + x) * 4 + 1], bits[((img.height()-y-1) * img.width() + x) * 4 + 1]);
            swap(bits[(y * img.width() + x) * 4 + 2], bits[((img.height()-y-1) * img.width() + x) * 4 + 2]);
            swap(bits[(y * img.width() + x) * 4 + 3], bits[((img.height()-y-1) * img.width() + x) * 4 + 3]);
        }
    }

    unsigned int textureID;
    GL::funcs.glGenTextures(1, &textureID);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, textureID);

    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GL::funcs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
    GL::funcs.glGenerateMipmap(GL_TEXTURE_2D);
    return textureID;
}
