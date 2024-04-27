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
            int from;
            switch(nrChannels)
            {
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
    aiColor3D c;
    mat->Get(AI_MATKEY_TWOSIDED, m.twosided);
    mat->Get(AI_MATKEY_SHININESS, m.shininess);
    mat->Get(AI_MATKEY_SHININESS_STRENGTH, m.shininessStrength);
    mat->Get(AI_MATKEY_OPACITY, m.opacity);
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, c))
    {
        m.textureMask_ |= AMBIENT_COLMASK;
        m.ambientCol = vec3(c.r, c.g, c.b);
    }
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, c))
    {
        m.textureMask_ |= DIFFUSE_COLMASK;
        m.diffuseCol = vec3(c.r, c.g, c.b);
    }
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, c))
    {
        m.textureMask_ |= SPECULAR_COLMASK;
        m.specularCol = vec3(c.r, c.g, c.b);
    }
    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_EMISSIVE, c))
        m.emissiveCol = vec3(c.r, c.g, c.b);
    if ((m.ambientText_ = load(mat, aiTextureType_AMBIENT, folderRoot)))
        m.textureMask_ |= AMBIENT_TEXMASK;
    if ((m.diffuseText_ = load(mat, aiTextureType_DIFFUSE, folderRoot)))
        m.textureMask_ |= DIFFUSE_TEXMASK;
    if ((m.specularText_ = load(mat, aiTextureType_SPECULAR, folderRoot)))
        m.textureMask_ |= SPECULAR_TEXMASK;
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
        textureMask_ |= DIFFUSE_TEXMASK;
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuseText_);
    }
    if (specularText_) {
        textureMask_ |= SPECULAR_TEXMASK;
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularText_);
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
