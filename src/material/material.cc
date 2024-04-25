#include <GL/glew.h>
#include "material.hh"
#include "tools.hh"
#include <assimp/material.h>
#include <iostream>
#include <map>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static std::map<std::string, GLuint> textureCache;
static GLuint load(aiMaterial *mat, aiTextureType type,
                   std::string folderRoot) {
    // Diffuse maps
    int numTextures = mat->GetTextureCount(type);
    aiString fileName;
    if (numTextures > 0) {
        if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &fileName, NULL, NULL,
                            NULL, NULL, NULL) == AI_SUCCESS) {
            int width, height, nrChannels;
            auto path = folderRoot + "/" + fileName.C_Str();

            if (textureCache.find(path) != textureCache.end())
                return textureCache[path];

            auto *data =
                stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
            FAIL_ON(!data, "Error while trying to read " << path);
            GLuint buffer;
            glGenTextures(1, &buffer);
            textureCache[path] = buffer;
            glBindTexture(GL_TEXTURE_2D, buffer);
            glTexImage2D(buffer, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            __glewGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
            return buffer;
        }
    }
    return 0;
}

Material Material::createFrom(std::string folderRoot, aiMaterial *mat) {
    Material m = Material();
    aiString materialName;
    aiReturn ret;
    ret = mat->Get(AI_MATKEY_NAME, materialName);
    FAIL_ON(ret != AI_SUCCESS, "Failed to load a material");
    std::cout << "Material " << materialName.C_Str() << " loaded" << '\n';
    m.diffuseText_ = load(mat, aiTextureType_DIFFUSE, folderRoot);
    return m;
}
