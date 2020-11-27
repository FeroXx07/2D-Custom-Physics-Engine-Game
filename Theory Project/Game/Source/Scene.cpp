#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"
#include "Physics.h"


Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	//app->map->Load("hello2.tmx");
	
	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	
	Collider* my = new Collider({ 0,0,100,100 });
	Body*myBody = app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::PLAYER, NULL, my, { 0.0f,0.0f }, { 0.0f,5.0f });
	myBody->position = { 100.0f,100.f };

	Collider* my2 = new Collider({ 0,0,100,100 });
	Body* myBody2 = app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::PLAYER, NULL, my2, { 0.0f,0.0f }, { 0.0f,5.0f });
	myBody2->position = { 600.0f,100.f };
	myBody2->mass = 10;
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

    // L08: TODO 6: Make the camera movement independent of framerate
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= ceil(200.0f*dt);

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += ceil(200.0f * dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= ceil(200.0f * dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += ceil(200.0f * dt);

	// Draw map
	// L03: DONE 7: Set the window title with map/tileset info
	/*SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.count());*/

	//app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
