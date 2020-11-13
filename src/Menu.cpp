#include "Menu.h"
#include "game.h"



Menu::Menu() {
	shader = new Shader();
	//create mesh and resize verts and uvs
	mesh = new Mesh();
	mesh->vertices.resize(4);
	mesh->uvs.resize(4);

	//fill verts and uvs
	mesh->vertices[0] = Vector3(-1, -1, 0);	mesh->uvs[0] = Vector2(0, 0);
	mesh->vertices[1] = Vector3(1, -1, 0);	mesh->uvs[1] = Vector2(1, 0);
	mesh->vertices[2] = Vector3(1, 1, 0);	mesh->uvs[2] = Vector2(1, 1);
	mesh->vertices[3] = Vector3(-1, 1, 0);	mesh->uvs[3] = Vector2(0,1);

	//create indices
	mesh->indices.resize(6);
	mesh->indices[0] = 0; mesh->indices[1] = 1; mesh->indices[2] = 2;
	mesh->indices[3] = 0; mesh->indices[4] = 2; mesh->indices[5] = 3;

	//send to GPU
	mesh->genBuffers();
}

void Menu::render() {
	Game* game = Game::instance;

	shader->enable();
	if(game->state == 1)shader->setTexture("u_texture_diffuse", entries[0]);
	//Game Over
	if (game->state == 3)shader->setTexture("u_texture_diffuse", entries[1]);
	//Victory
	if (game->state == 4)shader->setTexture("u_texture_diffuse", entries[2]);

	mesh->render(GL_TRIANGLES, shader);
	shader->disable();
}

void Menu::update(float dt) {
	Game* game = Game::instance;
	if (game->keystate[SDL_SCANCODE_SPACE]) game->state = 2;
	if (game->keystate[SDL_SCANCODE_R]) {
		game->restart();
	}
}