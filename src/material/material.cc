#include <GL/glew.h>
#include "material.hh"
#include "tools.hh"
#include <assimp/material.h>
#include <assimp/vector3.h>
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

            if (textureCache.find(path) != textureCache.end()) {
                std::cout << "Caching " << path << '\n';
                return textureCache[path];
            }

            auto *data =
                stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
            FAIL_ON(!data, "Error while trying to read " << path);
            GLuint buffer;
            glGenTextures(1, &buffer);
            textureCache[path] = buffer;
            glBindTexture(GL_TEXTURE_2D, buffer);

            // set the texture wrapping/filtering options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
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
    aiColor3D c;
    mat->Get(AI_MATKEY_SHININESS, m.shininess);
    mat->Get(AI_MATKEY_SHININESS_STRENGTH, m.shininessStrength);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, c))
        m.diffuseCol = vec3(c.r, c.g, c.b);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, c))
        m.specularCol = vec3(c.r, c.g, c.b);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, c))
        m.ambientCol = vec3(c.r, c.g, c.b);
    m.diffuseText_ = load(mat, aiTextureType_DIFFUSE, folderRoot);
    m.specularText_ = load(mat, aiTextureType_SPECULAR, folderRoot);
    return m;
}

void Material::enable(Render &r) {
    r.activate();
    /* /!\ activate the texture unit first before binding texture */
    if (diffuseText_) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseText_);
    }
    if (specularText_) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularText_);
    }
    r.setFloat("shininess", shininess);
    r.setFloat("shininessStrength", shininessStrength);
    r.setVec3("diffuseCol", diffuseCol);
    r.setVec3("specularCol", specularCol);
    r.setVec3("ambientCol", ambientCol);
}
