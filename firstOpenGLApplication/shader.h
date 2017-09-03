//************************************************************************************************************************
//
//	LearnOpenGL - shader.h
//
//	Name:			Tucker Dane Walker
//	Date:			August 2017
//	Description:	Specifications for an OpenGL shader class to make using shaders more streamlined
//					Mostly taken from learnopengl.com
//
//***********************************************************************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	//---------------------------------
	unsigned int ID;

	// constructor reads and builds the shader
	//---------------------------------
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader
	//---------------------------------
	void use();

	// utility uniform functions
	//---------------------------------
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif
