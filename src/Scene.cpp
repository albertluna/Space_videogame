#include "Scene.h"

Scene::Scene() {
	this->root = new GameObject();
}

Scene::~Scene() {
	this->root->clear();
	delete this->root;
}

void Scene::render() {
	this->root->render();
}

void Scene::update(float dt) {
	this->root->update(dt);
}
void Scene::getColliders(std::vector<GameObject*>& colliders, GameObject* current) {
	for (GameObject* fill : root->children) {
		fill->getColliders(colliders, fill);
	}
}