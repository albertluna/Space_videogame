#pragma once

#include "includes.h"
#include "framework.h"

class GameObject {

public:
	std::string name;
	Matrix44 model;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	GameObject();
	~GameObject();

	virtual void render();
	virtual void update(float elapsed_time);
	bool is_collider = false;

	void clear();
	void addChild(GameObject* obj);
	void removeChild(GameObject* obj);
	Matrix44 getGlobalMatrix();
	void getColliders(std::vector<GameObject*>& colliders, GameObject* current);

};