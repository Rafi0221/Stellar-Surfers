#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>

int load_obj(std::string path, unsigned int &VAO);

unsigned int load_texture(std::string path);


#endif // OBJ_LOADER_H
