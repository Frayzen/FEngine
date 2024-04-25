#include "material.hh"
#include "tools.hh"
#include <assimp/material.h>
#include <iostream>

void load(aiMaterial *mat, aiTextureType type) {
    // Diffuse maps
    int numTextures = mat->GetTextureCount(type);
    aiString textureName;
    aiString materialName;
    aiReturn ret;
    if (numTextures > 0) {
        ret = mat->Get(AI_MATKEY_TEXTURE(type, 0), textureName);
        FAIL_ON(ret != AI_SUCCESS, "Failed to load a texture");
        std::string textureType = "diff_";
        std::string textureFileName = textureType + textureName.data;
        std::cout << textureName.C_Str() << '\n';
    }
}

Material Material::createFrom(aiMaterial *mat) {
    Material m = Material();
    aiString materialName;
    aiReturn ret;
    ret = mat->Get(AI_MATKEY_NAME, materialName);
    FAIL_ON(ret != AI_SUCCESS, "Failed to load a material");
    std::cout << "Material " << materialName.C_Str() << " loaded" << '\n';
    load(mat, aiTextureType_DIFFUSE);
    return m;
}
