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

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define FORCE 20.0f

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

	img = app->tex->Load("Assets/textures/SpaceshipLow.png");

	Collider* theSquareColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(1000),(int)PIXEL_TO_METERS(1000) });
	theSquareBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::PLAYER, NULL, theSquareColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	theSquareBody->position = { PIXEL_TO_METERS(6000.0f),PIXEL_TO_METERS(1000.0f) };
	theSquareBody->mass = 10;
	theSquareBody->coeficientRestitution = { 1.0f,0.8f };
	theSquareBody->coeficientAeroDrag = { 0.01f, 0.01f };
	theSquareBody->coeficientAeroLift = 0.5f;

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

	/*Collider* middleBoxColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(5000),(int)PIXEL_TO_METERS(5000) });
	Body* middleBoxBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::WALL, NULL, middleBoxColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	middleBoxBody->position = { PIXEL_TO_METERS(25000.0f),PIXEL_TO_METERS(15000.0f) };
	middleBoxBody->mass = 10;*/

	AddPlanet(CircleCollider(PIXEL_TO_METERS(20000.0f), PIXEL_TO_METERS(20000.0f), PIXEL_TO_METERS(5000.0f)),
		CircleCollider(PIXEL_TO_METERS(25000.0f),PIXEL_TO_METERS(25000.0f), PIXEL_TO_METERS(15000.0f)));

	/*Collider* theSquareColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(100),(int)PIXEL_TO_METERS(100) });
	DynamicBody* theSquareBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::PLAYER, NULL, theSquareColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	theSquareBody->position = { PIXEL_TO_METERS(600.0f),PIXEL_TO_METERS(60.0f) };
	theSquareBody->mass = 10;
	theSquareBody->coeficientRestitution = { 1.0f,0.8f };

	Collider* groundColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(1280),(int)PIXEL_TO_METERS(100) });
	Body* groundBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, NULL, groundColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	groundBody->position = { PIXEL_TO_METERS(0.0f),PIXEL_TO_METERS(600.0f) };
	groundBody->mass = 10;

	Collider* roofColl = new Collider({ 0,0,(int)PIXEL_TO_METERS(1280),(int)PIXEL_TO_METERS(100) });
	Body* roofBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, NULL, roofColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	roofBody->position = { PIXEL_TO_METERS(0.0f),PIXEL_TO_METERS(0.0f) };
	roofBody->mass = 10;*/

	app->physics->ChangeGravityAcceleration({ 0.0f, 10.0f });

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate(float dt)
{
	bool onOrbit = false;
	for (ListItem<Planet*>* list = planets.start; list; list = list->next)
	{
		CircleCollider currentPlanet = list->data->planet;
		CircleCollider currentOrbit = list->data->orbit;

		if (onOrbit = theSquareBody->collider->CheckCollision(currentOrbit, theSquareBody->collider->r1))
		{
			// Collision with orbit
			LOG("ORBIT!!!!");
			float force = 25.0f;

			fPoint directionGravity = fPoint({ currentPlanet.x,currentPlanet.y }) - theSquareBody->position;

			float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

			directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
			directionGravity = { directionGravity.x * force, directionGravity.y * force };

			theSquareBody->SetGravityAcceleration(directionGravity);
			theSquareBody->coeficientAeroDrag = { 0.1f, 0.1f };
		}

		if (theSquareBody->collider->CheckCollision(currentPlanet, theSquareBody->collider->r1))
		{
			// Collision with PLANET
			LOG("PLANET!!!!");
		}
	}

	if (onOrbit == false)
	{
		// Return to default
		theSquareBody->coeficientAeroDrag = { 0.01f, 0.01f };
		theSquareBody->SetGravityAcceleration({ 0.0f, 10.0f });
	}
	// L02: DONE 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	// L08: TODO 6: Make the camera movement independent of framerate
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= ceil(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += ceil(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= ceil(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
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
				float angleRadians = DEGREES_TO_RADIANS(currentBody->rotation);
				fPoint direction = { 0.0f,0.0f };

				if (angleRadians >= 0 && angleRadians <= 90)
				{
					direction.x = currentBody->position.x * cos(angleRadians) * FORCE;
					direction.y = currentBody->position.y * sin(angleRadians) * FORCE;
				}
				else if (angleRadians > 90 && angleRadians <= 180)
				{
					direction.x = currentBody->position.x * cos(angleRadians) * FORCE;
					direction.y = currentBody->position.y * -sin(angleRadians) * FORCE;
				}
				else if (angleRadians > 180 && angleRadians <= 270)
				{
					direction.x = currentBody->position.x * -cos(angleRadians) * FORCE;
					direction.y = currentBody->position.y * -sin(angleRadians) * FORCE;
				}
				else if (angleRadians > 270 && angleRadians <= 360)
				{
					direction.x = currentBody->position.x * -cos(angleRadians) * FORCE;
					direction.y = currentBody->position.y * sin(angleRadians) * FORCE;
				}

				currentBody->ApplyForce(direction);
				//currentBody->ApplyForce(0, -FORCE);
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
				currentBody->ApplyForce(FORCE * 100.0f, 0);
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
				currentBody->ApplyForce(-FORCE * 100.0f, 0);
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
				currentBody->ApplyForce(0, FORCE * 100.0f);
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	{
		ListItem<Body*>* list;
		for (list = app->physics->bodyList.start; list != NULL; list = list->next)
		{
			// Apply forces to all dynamic bodies
			if (list->data->bodyType == BodyType::DYNAMIC_BODY)
			{
				DynamicBody* currentBody = (DynamicBody*)list->data;
				currentBody->rotation += 0.5;
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	{
		ListItem<Body*>* list;
		for (list = app->physics->bodyList.start; list != NULL; list = list->next)
		{
			// Apply forces to all dynamic bodies
			if (list->data->bodyType == BodyType::DYNAMIC_BODY)
			{
				DynamicBody* currentBody = (DynamicBody*)list->data;
				currentBody->rotation += -0.5f;
			}
		}
	}
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
  


	// Draw map
	// L03: DONE 7: Set the window title with map/tileset info
	/*SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.count());*/

	//app->win->SetTitle(title.GetString());

	//LOG("POSITION = %f , %f", theSquareBody->position.x, theSquareBody->position.y);
	//LOG("VELOCITY = %f , %f", theSquareBody->velocity.x, theSquareBody->velocity.y);
	LOG("ROTATION = %f",theSquareBody->rotation);
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, theSquareBody->position.x, theSquareBody->position.y, NULL, 0.0f, (theSquareBody->rotation));

	for (ListItem<Planet*>* list = planets.start; list; list = list->next)
	{
		CircleCollider currentPlanet = list->data->planet;
		CircleCollider currentOrbit = list->data->orbit;

		app->render->DrawCircle(currentPlanet.x, currentPlanet.y, currentPlanet.radius, 123, 104, 238);
		app->render->DrawCircle(currentOrbit.x, currentOrbit.y, currentOrbit.radius, 0, 191, 255);
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
