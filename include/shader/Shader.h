#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<stb/stb_image.h>
#include <string>
#include <iostream>

// read me:
// 1.本头文件定义父类Shader 以及两个子类 lightShader 、lightCubeShader
//使用时只需要使用两个子类，并且默认文件路径已经设置好，调用无参构造函数即可。
// 2.新加函数 unsigned int loadTexture(char const* path)，使用时直接输入纹理图片的路径即可
// 返回纹理的ID号
// 3.为了解决使用shader时，对各个光源信息进行的频繁的参数设置，使用函数
// void setMultiLights()封装起来，待后面确定光源信息及数量之后再填写
//

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr); 
    // activate the shader
    // ------------------------------------------------------------------------
    void use();
     // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const;
     // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const;
     // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const;
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w); // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type);
};


class lightShader :public Shader
{
public:
    lightShader(const char* vertexPath = "../include/shader/resources/multiple_lights.vs", const char* fragmentPath = "../include/shader/resources/multiple_lights.fs")
        :Shader(vertexPath, fragmentPath)
    {};
};

class lightCubeShader :public Shader
{
public:
    lightCubeShader(const char* vertexPath = "../include/shader/resources/light_cube.vs", const char* fragmentPath = "../include/shader/resources/light_cube.fs")
        :Shader(vertexPath, fragmentPath)
    {};

};


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void setMultiLights()
{

}

#endif
