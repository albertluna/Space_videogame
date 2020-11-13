#include "GameObjectEnemy.h"
#include "GameObjectMesh.h"

GameObjectEnemy::GameObjectEnemy(Vector3 a, Vector3 b, Vector3 c) {
	i = 0;
	waypoints.push_back(a);
	waypoints.push_back(b);
	waypoints.push_back(c);
}
void GameObjectEnemy::update(float dt) {
	
	//Si l'angle es mes gran que un numero petit fer rotacio
	Vector3 pos = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	Vector3 mov = Vector3(this->waypoints[i] - pos).normalize();
	Vector3 to_target = waypoints[i] - pos;
	Vector3 front = Vector3(this->model.frontVector().x, this->model.frontVector().y, this->model.frontVector().z);
	float angle = acos(to_target.dot(front));
	Vector3 axis = to_target.cross(front);

	Matrix44 inv = this->model;
	inv.inverse();
	Vector3 rot_axis = inv.rotateVector(axis);
	this->model.rotateLocal(-0.005, rot_axis*10);
		
	if (pos.distance(waypoints[i]) < 5.0) {
		if (i < waypoints.size()-1){i++;}
		else {i = 0;}
	}
	this->model.traslate(mov.x * dt*10, mov.y * dt*10, mov.z * dt*10);
	GameObjectMesh::update(dt);
}