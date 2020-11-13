#pragma once 
#include <unordered_map>
#include "mesh.h"
#include "texture.h"
#include "shader.h"


using namespace std;

class ResourceManager {
private:

	unordered_map<string, Mesh*> m_meshes;
	unordered_map<string, Shader*> m_shader;
	unordered_map<string, Texture*> m_textures;

public:
	ResourceManager();
	~ResourceManager();

	void loadMesh(const string name, const char* path);

	Mesh* getMesh(const string handle);

	void loadTexture(const string handle, const char* path);
	void loadShader(const string handle, const char* frag, const char* vert);

	void unloadMesh(const string);
	void unloadTexture(const string);
	void unloadShader(const string);

	Texture* getTexture(const string);
	Shader* getShader(const string);

};