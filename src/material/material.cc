#include <GL/glew.h>
#include "material.hh"
#include "tools.hh"
#include <assimp/material.h>
#include <assimp/vector3.h>
#include <iostream>
#include <map>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"

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
            int from;
            switch (nrChannels) {
            case 1:
                from = GL_RED;
                break;
            case 3:
                from = GL_RGB;
                break;
            case 4:
                from = GL_RGBA;
                break;
            default:
                FAIL_ON(true, "Unsupoorted number of channel : " << nrChannels);
                return 0;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, from, width, height, 0, GL_RGB,
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

    // Load textures
    if ((m.ambientText_ = load(mat, aiTextureType_AMBIENT, folderRoot))) {
        m.ambientCol = vec3(1.0f);
        m.textureMask_ |= AMBIENT_TEXMASK;
    }
    if ((m.diffuseText_ = load(mat, aiTextureType_DIFFUSE, folderRoot))) {
        m.diffuseCol = vec3(1.0f);
        m.textureMask_ |= DIFFUSE_TEXMASK;
    }
    if ((m.specularText_ = load(mat, aiTextureType_SPECULAR, folderRoot))) {
        m.specularCol = vec3(1.0f);
        m.textureMask_ |= SPECULAR_TEXMASK;
    }
    if ((m.normalMap_ = load(mat, aiTextureType_NORMALS, folderRoot)))
        m.textureMask_ |= NORMAL_MAPMASK;

    // Load colors and scalars
    mat->Get(AI_MATKEY_TWOSIDED, m.twosided);
    mat->Get(AI_MATKEY_SHININESS, m.shininess);
    mat->Get(AI_MATKEY_SHININESS_STRENGTH, m.shininessStrength);
    mat->Get(AI_MATKEY_OPACITY, m.opacity);

    aiColor3D c;
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, c))
        m.ambientCol = vec3(c.r, c.g, c.b);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, c))
        m.diffuseCol = vec3(c.r, c.g, c.b);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, c))
        m.specularCol = vec3(c.r, c.g, c.b);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_EMISSIVE, c))
        m.emissiveCol = vec3(c.r, c.g, c.b);

    // Print informations
    std::cout << "Material " << materialName.C_Str() << " loaded";
    int textCount =
        (m.diffuseText_ != 0) + (m.ambientText_ != 0) + (m.specularText_ != 0);
    std::cout << "(" << textCount << "textures)" << '\n';
    return m;
}

void Material::enable(Render &r) {
    r.activate();
    /* /!\ activate the texture unit first before binding texture */
    if (ambientText_) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ambientText_);
    }
    if (diffuseText_) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuseText_);
    }
    if (specularText_) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularText_);
    }
    if (normalMap_) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalMap_);
    }
    if (twosided && opacity == 1.0f)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    r.setInt("textureMask", textureMask_);
    r.setFloat("shininess", shininess);
    r.setFloat("shininessStrength", shininessStrength);
    r.setFloat("opacity", opacity);
    r.setVec3("diffuseCol", diffuseCol);
    r.setVec3("specularCol", specularCol);
    r.setVec3("emissiveCol", emissiveCol);
    r.setVec3("ambientCol", ambientCol);
}

void Material::setColor(vec3 c, bool noDiffuse) {
    ambientCol = c * 0.5f;
    if (noDiffuse)
        diffuseCol = vec3(0);
    else
        diffuseCol = c;
}
