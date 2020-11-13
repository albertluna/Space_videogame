#pragma once
#include "GameObjectMesh.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include <cmath>

class GameObjectEnemy : public GameObjectMesh {
public:
	GameObjectEnemy(Vector3 a, Vector3 b, Vector3 c);
	std::vector<Vector3> waypoints;
	int i;
	//Vector3 waypoint = Vector3(0, 30, 0);

	void update(float dt);
};