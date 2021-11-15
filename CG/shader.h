#ifndef SHADER_H
#define SHADER_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>// ����glad����ȡ���еı���OpenGLͷ�ļ�
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include"shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// read me: 
// 1.��ͷ�ļ����常��Shader �Լ��������� lightShader ��lightCubeShader
//ʹ��ʱֻ��Ҫʹ���������࣬����Ĭ���ļ�·���Ѿ����úã������޲ι��캯�����ɡ�
// 2.�¼Ӻ��� unsigned int loadTexture(char const* path)��ʹ��ʱֱ����������ͼƬ��·������
// ���������ID��
// 3.Ϊ�˽��ʹ��shaderʱ���Ը�����Դ��Ϣ���е�Ƶ���Ĳ������ã�ʹ�ú���
// void setMultiLights()��װ������������ȷ����Դ��Ϣ������֮������д
//

unsigned int loadTexture(char const* path);

void setMultiLights();  //����д

class Shader
{
public:
	// ��ɫ������ID
	unsigned int ID;

	//���캯�� ��ȡ����ɫ����Ƭ����ɫ��
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. ���ļ�·�����ļ�
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ȷ��ifstream���ܹ��׳��쳣
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// ���ļ����ж��뵽buffer��
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// �ر��ļ���
			vShaderFile.close();
			fShaderFile.close();
			// ����ת��string����
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			//�Լ��ӵģ����ڲ��ԣ���ӡ������̨
			//std::cout << vertexCode << std::endl;
			//std::cout << fragmentCode << std::endl;
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "����Shader��ȡ�ļ�ʧ��" << std::endl;
		}
		//ת���ɲ���Ҫ��char *���ͣ�
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. ������ɫ��
		unsigned int vertex, fragment;
		// ����ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// Ƭ����ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// ��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// ɾ����ɫ������Ϊ�����Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}


	Shader() // blinn-phong����ģ��
	{

		//ת���ɲ���Ҫ��char *���ͣ�
		const char* vShaderCode = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aNormal;\n"
			"out vec3 FragPos;\n"
			"out vec3 Normal;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
			"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
			"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
			"}\0";

		const char* fShaderCode = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec3 Normal;\n"
			"in vec3 FragPos;\n"
			"uniform vec3 lightPos;\n"
			"uniform vec3 viewPos;\n"
			"uniform vec3 lightColor;\n"
			"uniform vec3 objectColor;\n"
			"void main()\n"
			"{\n"
			// ��������
			"   float ambientStrength = 0.2;\n"
			"   vec3 ambient = ambientStrength * lightColor;\n"
			// ���������
			"   vec3 norm = normalize(Normal);\n"
			"   vec3 lightDir = normalize(lightPos - FragPos);\n"
			"   float diff = max(dot(norm, lightDir), 0.0);\n"
			"   vec3 diffuse = diff * lightColor;\n"
			// �������
			"   float specularStrength = 0.5;\n"
			"   vec3 viewDir = normalize(viewPos - FragPos);\n"
			"   vec3 reflectDir = reflect(-lightDir, norm);\n"
			"   vec3 halfwayDir = normalize(lightDir + viewDir);\n"
			"   float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);\n"
			"   vec3 specular = specularStrength * spec * lightColor;\n"
			// ���
			"   vec3 result = (ambient + diffuse + specular) * objectColor;\n"
			"   FragColor = vec4(result, 1.0);\n"
			"}\0";

		// 2. ������ɫ��
		unsigned int vertex, fragment;
		// ����ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// Ƭ����ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// ��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// ɾ����ɫ������Ϊ�����Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}



	// ������ɫ����
	// ------------------------------------------------------------------------
	//ʹ����ɫ����
	void use()
	{
		glUseProgram(ID);
	}
	// �Ƚ�ʵ�õ�ͳһ����
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	//�������ú���
	void setVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void setVec3(const std::string& name, const glm::vec3& value)const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z)const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec4(const std::string& name, const glm::vec4& value)const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void setMat2(const std::string& name, glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const std::string& name, glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const std::string& name, glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// ���ڼ����ɫ������ / ���Ӵ����ʵ�ó�������
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024]; //�洢��Ϣ��־
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "����Shader�������: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "����Shader���Ӵ���: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};


class lightShader :public Shader
{
public:
	lightShader(const char* vertexPath = "multiple_lights.vs", const char* fragmentPath = "multiple_lights.fs")
		:Shader(vertexPath, fragmentPath)
	{};
};

class lightCubeShader :public Shader
{
public:
	lightCubeShader(const char* vertexPath = "light_cube.vs", const char* fragmentPath = "light_cube.fs")
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
