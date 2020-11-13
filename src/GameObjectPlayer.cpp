#include "GameObjectPlayer.h"
#include "game.h"
#include "Math.h"

void GameObjectPlayer::update(float dt) {
	Game* game = Game::instance;
	float pitch_speed = 2;
	float limit = 100;
	if ((game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP])) {
		this->model.rotateLocal(-dt * pitch_speed, Vector3(1, 0, 0));
	}
	if ((game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN])) {
		this->model.rotateLocal(dt * pitch_speed, Vector3(1, 0, 0));
	}
	if (game->keystate[SDL_SCANCODE_A] || game->keystate[SDL_SCANCODE_LEFT]) {
		this->model.rotateLocal(-dt * pitch_speed, Vector3(0, 0, 1));
	}
	if (game->keystate[SDL_SCANCODE_D] || game->keystate[SDL_SCANCODE_RIGHT]) {
		this->model.rotateLocal(dt * pitch_speed, Vector3(0, 0, 1));
	}
	//Control para que no se salga de unos límites
	if (this->model.m[13] > limit ||this->model.m[12] > limit || this->model.m[14] > limit) this->model.rotateLocal(dt * pitch_speed, Vector3(1, 0, 0));
	if (this->model.m[13] < -limit || this->model.m[12] < -limit || this->model.m[14] < -limit) this->model.rotateLocal(-dt * pitch_speed, Vector3(1, 0, 0));


	Vector3 f = this->model.frontVector().normalize();
	float speed_dt = -dt * pitch_speed * 10;
	if(!game->keystate[SDL_SCANCODE_T]) this->model.traslate(f.x * speed_dt, f.y * speed_dt, f.z * speed_dt);

	Vector3 player_pos(this->model.m[12], this->model.m[13], this->model.m[14]);
	Vector3 player_up(this->model.m[4], this->model.m[5], this->model.m[6]);
	player_up = player_up.normalize();
	

	game->camera->eye = game->camera->eye.lerp((player_pos + this->model.frontVector().normalize() * 30 + player_up * 10), 0.1);
	game->camera->center = ((player_pos - this->model.frontVector().normalize() * 30));
	game->camera->up = player_up;
}