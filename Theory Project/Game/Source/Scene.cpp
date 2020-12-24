#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#define FORCE 20.0f
#define TURN_ANGLE 2.0f

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
	img = app->tex->Load("Assets/textures/SpaceshipLow.png");

	Collider* groundColl = new Collider({ 0,0,1920,20 });
	Body* groundBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, { 0,1060 }, NULL, groundColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	groundBody->mass = 10;

	Collider* roofColl = new Collider({ 0,0,1920,20 });
	Body* roofBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, { 0,0 }, NULL, roofColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	roofBody->mass = 10;

	Collider* leftWallColl = new Collider({ 0,0,20,1080 });
	Body* leftWallBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::WALL, { 0,0 }, NULL, leftWallColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	leftWallBody->mass = 10;

	Collider* rightWallColl = new Collider({ 0,0,20,1080 });
	Body* rightWallBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::WALL, { 1900,0 }, NULL, rightWallColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	rightWallBody->mass = 10;

	AddPlanet(CircleCollider(150, 300, 100), 10);
	AddPlanet(CircleCollider(600, 600, 100), 10);

	AddPlanet(CircleCollider(900, 300, 100), 10);

	AddPlanet(CircleCollider(1400, 600, 100), 10);

	app->physics->ChangeGravityAcceleration({ 0.0f, 10.0f });

	ListItem<Planet*>*list;
	for (list = planets.start; list != NULL; list = list->next)
	{
		list->data->planetBody->gravityAcceleration = { 0.0f,0.0f };
		list->data->orbitBody->gravityAcceleration = { 0.0f,0.0f };
	}
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate(float dt)
{
	app->player->onOrbit = false;
	for (ListItem<Planet*>* list = planets.start; list && app->player->onOrbit == false; list = list->next)
	{
		CircleCollider currentPlanet = list->data->planet;
		CircleCollider currentOrbit = list->data->orbit;

		if (app->player->onOrbit = app->player->playerBody->collider->CheckCollision(currentOrbit, app->player->playerBody->collider->r1))
		{
			// Collision with orbit
			LOG("ORBIT!!!!");
			float force = 105.0f;

			fPoint directionGravity = fPoint({ currentPlanet.x,currentPlanet.y }) - app->player->playerBody->position;

			float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

			directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
			directionGravity = { directionGravity.x * force, directionGravity.y * force };

			app->player->playerBody->SetGravityAcceleration(directionGravity);
			app->player->playerBody->coeficientAeroDrag = { 0.1f, 0.1f };
		}

		if (app->player->playerBody->collider->CheckCollision(currentPlanet, app->player->playerBody->collider->r1))
		{
			// Collision with PLANET
			LOG("PLANET!!!!");
		}
	}

	
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	for (ListItem<Planet*>* list = planets.start; list; list = list->next)
	{
		CircleCollider currentPlanet = list->data->planet;
		CircleCollider currentOrbit = list->data->orbit;

		app->render->DrawCircle(METERS_TO_PIXELS(currentPlanet.x), METERS_TO_PIXELS(currentPlanet.y), currentPlanet.radius, 123, 104, 238);
		app->render->DrawCircle(METERS_TO_PIXELS(currentOrbit.x), METERS_TO_PIXELS(currentOrbit.y), currentOrbit.radius, 0, 191, 255);
	}

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
