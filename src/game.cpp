#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "GameObject.h"
#include "GameObjectEnemy.h"
#include "GameObjectPlayer.h"

#include <cmath>

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;
RenderToTexture* rt = NULL;

Game* Game::instance = NULL;
std::vector <GameObject*> collider;
GameObjectPlayer* fighter;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	mouse_locked = false;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
	state = 1;
    //SDL_SetWindowSize(window, 50,50);

	resource_manager = new ResourceManager();
	scene = new Scene();
	menu = new Menu();

	resource_manager->loadShader("menu_shader", "data/shaders/menu.frag", "data/shaders/menu.vert");
	//load initial texture
	Texture* sss = new Texture();
	sss->load("data/assets/menu_espai_Start.tga");
	menu->entries.push_back(sss);
	
	//load game over texture
	Texture* go = new Texture();
	go->load("data/assets/gameover.tga");
	menu->entries.push_back(go);

	//load victory texture	
	Texture* v = new Texture();
	v->load("data/assets/victory.tga");
	menu->entries.push_back(v);

	menu->shader = resource_manager->getShader("menu_shader");


	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0,25,25),Vector3(0,0,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
	camera->setPerspective(70.0f,(float)window_width/(float)window_height,0.1f,10000.0f); //set the projection, we want to be perspective

	//create a plane mesh
	//mesh = new Mesh();
	//mesh->createPlane(15.5f);
	//mesh->loadASE("data/assets/x3_fighter.ASE");

	//x3 fighter
	resource_manager->loadMesh("x3_fighter", "data/assets/x3_fighter.ASE");
	resource_manager->loadShader("phong_tex", "data/shaders/simple.frag", "data/shaders/simple.vert");
	resource_manager->loadTexture("x3_fighter_diffuse", "data/assets/x3_fighter.tga");
	
	//runner
	resource_manager->loadMesh("runner", "data/assets/x3_runner.ASE");
	resource_manager->loadTexture("runner_texture", "data/assets/x3_runner.tga");

	//eve
	resource_manager->loadMesh("eve", "data/assets/eve_station.ASE");
	resource_manager->loadTexture("eve_texture", "data/assets/eve1.tga");

	resource_manager->loadMesh("sphere", "data/assets/sphere.ASE");
	resource_manager->loadShader("milkyway_shader", "data/shaders/milkyway.frag", "data/shaders/milkyway.vert");
	resource_manager->loadTexture("sphere_texture", "data/assets/milkyway.tga");
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse


	milkyway = new GameEnviroment();
	milkyway->mesh = resource_manager->getMesh("sphere");
	milkyway->shader = resource_manager->getShader("milkyway_shader");
	milkyway->texture = resource_manager->getTexture("sphere_texture");

	fighter = new GameObjectPlayer();
	fighter->mesh = resource_manager->getMesh("x3_fighter");
	fighter->shader = resource_manager->getShader("phong_tex");
	fighter->texture = resource_manager->getTexture("x3_fighter_diffuse");
	fighter->setCollisionModel();

	//Enemies
	GameObjectEnemy* runner1 = new GameObjectEnemy(Vector3(0, 30, 40), Vector3(-20, -10, 40), Vector3(20, -10, 40));
	runner1->mesh = resource_manager->getMesh("runner");
	runner1->shader = resource_manager->getShader("phong_tex");
	runner1->texture = resource_manager->getTexture("runner_texture");
	runner1->setCollisionModel();
	//runner->is_collider = true;

	GameObjectEnemy* runner2 = new GameObjectEnemy(Vector3(0, 30, -40), Vector3(-20, -10, -40), Vector3(20, -10, -40));
	runner2->mesh = resource_manager->getMesh("runner");
	runner2->shader = resource_manager->getShader("phong_tex");
	runner2->texture = resource_manager->getTexture("runner_texture");
	runner2->setCollisionModel();

	GameObjectEnemy* runner3 = new GameObjectEnemy(Vector3(0, -80, 0), Vector3(70, 50, 0), Vector3(-80, 60, 0));
	runner3->mesh = resource_manager->getMesh("runner");
	runner3->shader = resource_manager->getShader("phong_tex");
	runner3->texture = resource_manager->getTexture("runner_texture");
	runner3->setCollisionModel();
	
	GameObjectMesh* target = new GameObjectMesh();
	target->mesh = resource_manager->getMesh("eve");
	target->shader = resource_manager->getShader("phong_tex");
	target->texture = resource_manager->getTexture("eve_texture");
	target->setCollisionModel();

	//D'alguna forma afegir el x fighter en la escena
	scene->root->addChild(milkyway);
	scene->root->addChild(fighter);
	scene->root->addChild(runner1);
	scene->root->addChild(runner2);
	scene->root->addChild(runner3);
	scene->root->addChild(target);
	fighter->model.traslate(0, 90, 0);
	target->model.traslate(0, 0, 0);
	target->model.scale(0.01, 0.01, 0.01);

	//scene->root->clear();
	scene->getColliders(collider, scene->root);
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update view and projection matrices, and push gl matrices for fixed pipeline rendering
	camera->set();
	switch (state) {
	case 1:
		menu->render();
		break;
	case 2:
		scene->render();
		break;
	default:
		menu->render();
		break;
	}

	//disable blending to draw text on top
    glDisable( GL_BLEND );

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	double speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

	//mouse input to rotate the cam
	if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(mouse_delta.x * 0.005f, Vector3(0,-1,0));
		camera->rotate(mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1,0,0)));
	}

	//to navigate with the mouse fixed in the middle
	if (mouse_locked) {
		int center_x = (int)floor(window_width*0.5f);
		int center_y = (int)floor(window_height*0.5f);
        //center_x = center_y = 50;
		SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
		//SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen
		
        this->mouse_position.x = (float)center_x;
        this->mouse_position.y = (float)center_y;
	}
	switch (state) {
	case 1:
		menu->update(seconds_elapsed);
		break;
	case 2:
		//Colisions
		for (int i = 0; i < collider.size(); i++) {
			for (int j = i + 1; j < collider.size(); j++) {
				testCollision(collider[i], collider[j]);
			}
		}

		scene->update(seconds_elapsed);
		break;
	default:
		menu->update(seconds_elapsed);
		break;
	}
	
	/*temp_game_object_mesh->update(seconds_elapsed);
	eve->update(seconds_elapsed);
	milkyway->update(seconds_elapsed);*/
	angle += (float)seconds_elapsed * 10;
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
	/*
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	*/

	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::testCollision(GameObject* A, GameObject* B) {

	GameObjectMesh* gom = dynamic_cast<GameObjectMesh*>(A);
	GameObjectMesh* gom1 = dynamic_cast<GameObjectMesh*>(B);

	gom->cm->setTransform(gom->model.m);
	gom1->cm->setTransform(gom1->model.m);

	bool collision = gom->cm->collision(gom1->cm);
	//Game Over
	if (collision&&(((dynamic_cast<GameObjectPlayer*>(B) != nullptr)||(dynamic_cast<GameObjectPlayer*>(A) != nullptr))&&((dynamic_cast<GameObjectEnemy*>(A) != nullptr)||(dynamic_cast<GameObjectEnemy*>(B) != nullptr)))) {
		cout << "\ncollision go";
		state = 3;
	} else {
		//Victory
		if (collision && (((dynamic_cast<GameObjectPlayer*>(B) != nullptr) || (dynamic_cast<GameObjectPlayer*>(A) != nullptr)) && ((dynamic_cast<GameObjectMesh*>(B) != nullptr) || (dynamic_cast<GameObjectMesh*>(A) != nullptr)))) {
			cout << "\ncolision v";
			state = 4;
		}
	}
}

void Game::restart() {
	state = 2;
	fighter->model.traslate(0, 90, 0);
}