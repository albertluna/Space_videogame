#pragma once
#include "mesh.h"
#include "texture.h"
#include "shader.h"

class Menu {
public:
	Menu();
	~Menu();

	Mesh* mesh;

	std::vector<Texture*> entries;
	int current_entry;
	Shader* shader;

	virtual void render();
	virtual void update(float dt);

};

