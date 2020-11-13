#include "GameObjectMesh.h"
#include "game.h"

void GameObjectMesh::render() {
	//std::cout << "Render is working " << name << "\n";

	Game* g = Game::instance;

	//create our mvp
	Matrix44 mvp = model * g->camera->viewprojection_matrix;
	//create normal matrix
	Matrix44 inverse_m = model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();
	//light position
	Vector3 light_pos(100.0f, 100.0f, 100.0f);
	//enable shader and set uniforms
	shader->enable();
	shader->setMatrix44("u_mvp", mvp);
	shader->setMatrix44("u_normal_matrix", normal_m);
	shader->setUniform3("u_light", light_pos);
	shader->setUniform3("u_cam_pos", g->camera->eye);
	shader->setTexture("u_texture_diffuse", texture);

	mesh->render(GL_TRIANGLES, shader);
	
	//enable shader and set uniforms
	shader->setUniform3("u_cam_pos", g->camera->eye.x, g->camera->eye.y, g->camera->eye.z);
  
	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);
	
	
	//disable the shader
	shader->disable();
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
}

void GameObjectMesh::update(float dt) {
	//std::cout << "Update with dt " << dt << "\n";
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}
}

void GameObjectMesh::setCollisionModel() {
	cm = newCollisionModel3D();
	cm->setTriangleNumber(mesh->indices.size() / 3);
	this->is_collider = true;

	for (size_t i = 0; i < mesh->indices.size() / 3; i++) {
		Vector3 v1 = mesh->vertices[mesh->indices[i * 3]];
		Vector3 v2 = mesh->vertices[mesh->indices[i * 3 + 1]];
		Vector3 v3 = mesh->vertices[mesh->indices[i * 3 + 2]];
		cm->addTriangle(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
	}

	cm->finalize();

}