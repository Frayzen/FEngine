#pragma once

#include <assimp/material.h>
#include <GL/gl.h>
class Material {
    
    public:
        static Material createFrom(std::string folderRoot, aiMaterial* mat);

    GLuint diffuseText_ = 0;

};
