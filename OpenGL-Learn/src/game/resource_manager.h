#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
	//resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textues;
	// loads (and generates) as shader program from file loading vertex, fragement (and geometry) shaser's spirce code. if gShaderFile is not nullptr, it  also loads a geometry shader
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	// retrieves a stored sader
	static Shader GetShader(std::string name);
	// loads (and generates) a textue from file
	static Texture2D LoadTextue(const char* file, bool alpha, std::string name);
	// retrieves a stored texture
	static Texture2D GetTextue(std::string name);
	// properly de-allocates all loaded resources
	static void Clear();
private:
	// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() {}
	// loads and generates a shader from file
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	// loads a single texture from file
	static Texture2D LoadTextureFromFile(const char* file, bool alpha);
};
#endif // !RESOURCE_MANAGER_H
