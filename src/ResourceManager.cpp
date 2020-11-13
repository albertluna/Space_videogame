#include "ResourceManager.h"


ResourceManager::ResourceManager() {

	cout << "creacio resource manager\n";
}

ResourceManager::~ResourceManager() {
	cout << "Destruit resource manager!\n";
}

void ResourceManager::loadMesh(const string handle, const char* path) {
	Mesh* mesh = new Mesh();

	mesh->loadASE(path);

	//afegir la mesh que hem creat al mapa de recursos
	m_meshes[handle] = mesh;
}

void ResourceManager::loadShader(const string handle, const char* frag, const char* vert) {

	Shader* shader = new Shader();

	if (!shader->load(vert, frag))
	{
		std::cout << "shader not found or error" << std::endl;
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}
	m_shader[handle] = shader;

}

void ResourceManager::loadTexture(const string handle, const char* path) {
	Texture* texture = new Texture();
	texture->load(path);
	m_textures[handle] = texture;
}

//unload a mesh from memory, given its handle 
void ResourceManager::unloadMesh(const std::string handle) {
//try to find mesh in map 
	std::unordered_map<std::string, Mesh*>::iterator itr = m_meshes.find(handle);
//check it's there
	if (itr != m_meshes.end()) { 
		// found it - delete it and erase it 
		delete itr->second;
		m_meshes.erase(itr); 
	} else //else print error and quit
	{ std::cout << "could not find asset with handle" << handle << std::endl;
	exit(0); }
}

Mesh* ResourceManager::getMesh(const string handle) {
	return m_meshes[handle];
}

Shader* ResourceManager::getShader(const string handle) {
	return m_shader[handle];
}

Texture* ResourceManager::getTexture(const string handle) {
	return m_textures[handle];
}