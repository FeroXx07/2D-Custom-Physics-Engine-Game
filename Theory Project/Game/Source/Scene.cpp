#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "ModuleFadeToBlack.h"

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

	if (app->player->IsEnabled() == false)
	{
		app->player->Enable();
	}

	//Set Initial Scene
	SetScene(MAIN_MENU);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
		app->fade->FadeToBlack(this, this);

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (scene == MAIN_MENU) UpdateMainMenu();
	else if (scene == LEVEL_SELECTOR) UpdateLevelSelector();
	else if ((scene == LEVEL_1) || (scene == LEVEL_2) || (scene == LEVEL_3)) UpdateLevels();
	if (scene == PAUSE_MENU) UpdatePauseMenu();

	return true;
}

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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	if (skip == true) ret = false;

	return ret;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");
	if (img != nullptr && img != NULL)
		app->tex->UnLoad(img);

	if (mainMenuBackground != nullptr && img != NULL)
		app->tex->UnLoad(mainMenuBackground);

	if (mainMenuArrow.arrowTex != nullptr && img != NULL)
		app->tex->UnLoad(mainMenuArrow.arrowTex);

	if (levelSelectArrow.arrowTex != nullptr && img != NULL)
		app->tex->UnLoad(levelSelectArrow.arrowTex);

	if (levelSelectBackground != nullptr && img != NULL)
		app->tex->UnLoad(levelSelectBackground);

	ListItem<Planet*>* listPlanet;
	for (listPlanet = planets.start; listPlanet != NULL; listPlanet = listPlanet->next)
	{
		delete listPlanet->data;
		planets.del(listPlanet);
	}
	planets.clear();

	app->player->Disable();
	app->physics->CleanUp();
	theVoid = nullptr;

	return true;
}


//UPDATERS

void Scene::UpdateMainMenu()
{
	app->render->DrawTexture(mainMenuBackground, 0, 0);

	app->render->DrawTexture(mainMenuArrow.arrowTex, mainMenuArrow.position[mainMenuArrow.selection].x, mainMenuArrow.position[mainMenuArrow.selection].y);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (mainMenuArrow.selection == 1) SetScene(LEVEL_SELECTOR);
		else if (mainMenuArrow.selection == 2) skip = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && mainMenuArrow.selection == 2) mainMenuArrow.selection = 1;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && mainMenuArrow.selection == 1) mainMenuArrow.selection = 2;
}

void Scene::UpdateLevelSelector()
{
	app->render->DrawTexture(levelSelectBackground, 0, 0);

	app->render->DrawTexture(levelSelectArrow.arrowTex, levelSelectArrow.position[levelSelectArrow.selection].x, levelSelectArrow.position[levelSelectArrow.selection].y);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (levelSelectArrow.selection == 1) SetScene(LEVEL_1);
		else if (levelSelectArrow.selection == 2) SetScene(LEVEL_2);
		else if (levelSelectArrow.selection == 3) SetScene(LEVEL_3);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && levelSelectArrow.selection != 1) levelSelectArrow.selection--;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && levelSelectArrow.selection != 3) levelSelectArrow.selection++;
}

void Scene::UpdateLevels()
{
	app->player->onOrbit = false;

	for (ListItem<Planet*>* list = planets.start; list && app->player->onOrbit == false; list = list->next)
	{
		if (list->data == theVoid)
			continue;

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
}

void Scene::UpdatePauseMenu()
{
	//Update Pause Menu
}


//SETTERS

void Scene::SetScene(SceneType changeScene)
{
	//CleanUp();

	scene = changeScene;

	if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == LEVEL_SELECTOR) SetLevelSelector();
	else if (scene == LEVEL_1) SetLevel1();
	else if (scene == LEVEL_2) SetLevel2();
	else if (scene == LEVEL_3) SetLevel3();
	else if (scene == PAUSE_MENU) SetPauseMenu();

}

void Scene::SetMainMenu()
{
	mainMenuBackground = app->tex->Load("Assets/textures/MAIN_MENU_TEMP_BACKGROUND.jpg");

	mainMenuArrow.arrowTex = app->tex->Load("Assets/textures/SELECTOR_ARROW_TEMP.png");
	mainMenuArrow.selection = 1;

	//LOAD IMAGE TITLE
	//LOAD IMAGE PLAY
}

void Scene::SetLevelSelector()
{
	levelSelectBackground = app->tex->Load("Assets/textures/LEVEL_SELECTION_TEMP_BACKGROUND.png");

	levelSelectArrow.arrowTex = app->tex->Load("Assets/textures/SELECTOR_ARROW_TEMP.png");
	levelSelectArrow.selection = 1;

	//LOAD IMAGE LEVEL 1
	//LOAD IMAGE LEVEL 2
	//LOAD IMAGE LEVEL 3
}

void Scene::SetLevel1()
{
	Collider* groundColl = new Collider({ 0,0,1920,20 });
	Body* groundBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, { 0,1060 }, NULL, groundColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	bodies.add(groundBody);
	groundBody->mass = 10;

	Collider* roofColl = new Collider({ 0,0,1920,20 });
	Body* roofBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::GROUND, { 0,0 }, NULL, roofColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	bodies.add(roofBody);
	roofBody->mass = 10;

	Collider* leftWallColl = new Collider({ 0,0,20,1080 });
	Body* leftWallBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::WALL, { 0,0 }, NULL, leftWallColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	bodies.add(leftWallBody);
	leftWallBody->mass = 10;

	Collider* rightWallColl = new Collider({ 0,0,20,1080 });
	Body* rightWallBody = app->physics->CreateBody(BodyType::STATIC_BODY, ColliderType::WALL, { 1900,0 }, NULL, rightWallColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	bodies.add(rightWallBody);
	rightWallBody->mass = 10;

	AddPlanet(CircleCollider(150, 150, 100), 10);
	AddPlanet(CircleCollider(600, 600, 100), 10);
	AddPlanet(CircleCollider(900, 300, 100), 10);
	AddPlanet(CircleCollider(1400, 600, 100), 10);

	theVoid = AddPlanet(CircleCollider(1920 / 2, 1500, 800), 600);

	app->player->theVoidPos = theVoid->planetBody->position;

	app->player->playerBody->position = { 120, 20 };
	app->player->playerBody->velocity = { 0, 0 };

	//Set Planet/Orbit Gravity

	ListItem<Planet*>* list;
	for (list = planets.start; list != NULL; list = list->next)
	{
		if (list->data == theVoid)
			continue;

		float force = 1.0f;

		fPoint directionGravity = theVoid->planetBody->position - list->data->planetBody->position;

		float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

		directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
		directionGravity = { directionGravity.x * force, directionGravity.y * force };

		list->data->planetBody->SetGravityAcceleration(directionGravity); //WRONG
		list->data->orbitBody->SetGravityAcceleration(directionGravity);
	}

}

void Scene::SetLevel2()
{
	// SET LEVEL 2, VOID APPEARS
}

void Scene::SetLevel3()
{
	// SET LEVEL 3, VOID APPEARS FASTLY
}

void Scene::SetPauseMenu()
{
	// LOAD IMAGE BACKGROUND
	// LOAD IMAGE EXTRAS (Ship images, fire images, logo image...) [Optional]
	// LOAD IMAGE OPTIONS
		// RETURN MAIN MENU
		// RETURN LEVEL SELECTOR
		// RETURN GAME
	// LOAD IMAGE SELECTOR TRIANGLE
}
