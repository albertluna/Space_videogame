#include "GameEnviroment.h"
#include "game.h"

void GameEnviroment::render() {
	std::cout << "Render is working " << name << "\n";

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

	//mesh->render(GL_TRIANGLES, shader);

	//enable shader and set uniforms
	shader->setUniform3("u_cam_pos", g->camera->eye.x, g->camera->eye.y, g->camera->eye.z);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	mesh->render(GL_TRIANGLES, shader);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);


	//disable the shader
	shader->disable();
}

void GameEnviroment::update(float dt) {
	std::cout << "Update with dt " << dt << "\n";
	Game* g = Game::instance;

	model.setTranslation(g->camera->eye.x, g->camera->eye.y, g->camera->eye.z);
}