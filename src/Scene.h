#pragma once

#include "GameObject.h"
class Scene {
public:

	Scene();
	~Scene();

	//scene graph starts from here
	GameObject* root;

	//optional - you can choose whether to put the camera in Scene
	//or leave it in game 
	//Camera* camera;

	//a list of all lights - could be within graph or not
	//std::vector<GameObjectLight*> lights;

	//standard render and update functions
	void render();
	void update(float);
	void getColliders(std::vector<GameObject*>& colliders, GameObject* current);


};