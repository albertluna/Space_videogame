#pragma once 

#include "GameObject.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "coldet/coldet.h"

class GameObjectMesh : public GameObject {
public:

	Mesh* mesh;
	Texture* texture;
	Shader* shader;

	virtual void render();
	virtual void update(float dt);
	CollisionModel3D* cm;
	void setCollisionModel();

};