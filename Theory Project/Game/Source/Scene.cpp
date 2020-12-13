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

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define FORCE 2000
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
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	
	/*Collider* my = new Collider({ 0,0,(int)PIXEL_TO_METERS(1000),(int)PIXEL_TO_METERS(1000) });
	Body*myBody = app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::PLAYER, NULL, my, { 0.0f,0.0f }, { 0.0f,0.0f });
	myBody->position = { PIXEL_TO_METERS(1000.0f),PIXEL_TO_METERS(1000.0f) };*/

	Collider* my2 = new Collider({ 0,0,(int)PIXEL_TO_METERS(1000),(int)PIXEL_TO_METERS(1000) });
	Body* myBody2 = app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::PLAYER, NULL, my2, { 0.0f,0.0f }, { 0.0f,0.0f });
	myBody2->position = { PIXEL_TO_METERS(6000.0f),PIXEL_TO_METERS(1000.0f) };
	myBody2->mass = 10;

	Collider* groundColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(65000),(int)PIXEL_TO_METERS(1000) });
	Body* groundBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, NULL, groundColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	groundBody->position = { PIXEL_TO_METERS(0.0f),PIXEL_TO_METERS(30000.0f) };
	groundBody->mass = 10;

	Collider* roofColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(65000),(int)PIXEL_TO_METERS(1000) });
	Body* roofBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, NULL, roofColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	roofBody->position = { PIXEL_TO_METERS(0.0f),PIXEL_TO_METERS(0.0f) };
	roofBody->mass = 10;

	Collider* leftWallColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(1000),(int)PIXEL_TO_METERS(50000) });
	Body* leftWallBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::WALL, NULL, leftWallColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	leftWallBody->position = { PIXEL_TO_METERS(0.0f),PIXEL_TO_METERS(0.0f) };
	leftWallBody->mass = 10;

	Collider* rightWallColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(1000),(int)PIXEL_TO_METERS(50000) });
	Body* rightWallBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::WALL, NULL, rightWallColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	rightWallBody->position = { PIXEL_TO_METERS(50000.0f),PIXEL_TO_METERS(0.0f) };
	rightWallBody->mass = 10;


	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	app->physics->ChangeGravityAcceleration({0.0f, 10.0f});

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

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		ListItem<Body*>* list;
		for (list = app->physics->bodyList.start; list != NULL; list = list->next)
		{
			// Apply forces to all dynamic bodies
			if (list->data->bodyType == BodyType::DYNAMIC_BODY)
			{
				DynamicBody* currentBody = (DynamicBody*)list->data;
				currentBody->ApplyForce(0, -FORCE);
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		ListItem<Body*>* list;
		for (list = app->physics->bodyList.start; list != NULL; list = list->next)
		{
			// Apply forces to all dynamic bodies
			if (list->data->bodyType == BodyType::DYNAMIC_BODY)
			{
				DynamicBody* currentBody = (DynamicBody*)list->data;
				currentBody->ApplyForce(FORCE, 0);
			}
		}
	}


	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		ListItem<Body*>* list;
		for (list = app->physics->bodyList.start; list != NULL; list = list->next)
		{
			// Apply forces to all dynamic bodies
			if (list->data->bodyType == BodyType::DYNAMIC_BODY)
			{
				DynamicBody* currentBody = (DynamicBody*)list->data;
				currentBody->ApplyForce(-FORCE, 0);
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		ListItem<Body*>* list;
		for (list = app->physics->bodyList.start; list != NULL; list = list->next)
		{
			// Apply forces to all dynamic bodies
			if (list->data->bodyType == BodyType::DYNAMIC_BODY)
			{
				DynamicBody* currentBody = (DynamicBody*)list->data;
				currentBody->ApplyForce(0, FORCE);
			}
		}
	}

	/*---------------------------------------------HARDCODED---------------------------------------------*/
	ListItem<Body*>* list;
	for (list = app->physics->bodyList.start; list != NULL; list = list->next)
	{
		// Apply forces to all dynamic bodies
		if (list->data->bodyType == BodyType::DYNAMIC_BODY)
		{
			DynamicBody* currentBody = (DynamicBody*)list->data;
			/*if (currentBody->position.y >= 720 - currentBody->collider->r1.h)
			{
				currentBody->position.y = 720 - currentBody->collider->r1.h;
				currentBody->velocity.y = -currentBody->velocity.y * 0.7;
			}

			if (currentBody->position.y <= 0 )
			{
				currentBody->position.y = 0;
				currentBody->velocity.y = -currentBody->velocity.y * 0.7;
			}

			if (currentBody->position.x <= 0)
			{
				currentBody->position.x = 0;
				currentBody->velocity.x = -currentBody->velocity.x * 0.2;
			}

			uint width, height;
			app->win->GetWindowSize(width, height);
			if (currentBody->position.x >= width - currentBody->collider->r1.w)
			{
				currentBody->position.x = width - currentBody->collider->r1.w;
				currentBody->velocity.x = -currentBody->velocity.x * 0.2;
			}*/

			if (fabs(currentBody->velocity.x) < 0.01f) // Stop the player once velocity is too small
				currentBody->velocity.x = 0;
		}
		/*---------------------------------------------HARDCODED---------------------------------------------*/
	}
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
