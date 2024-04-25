#pragma once

#include <assimp/material.h>
class Material {
    
    public:
        static Material createFrom(aiMaterial* mat);

};
