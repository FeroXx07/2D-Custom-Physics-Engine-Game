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

	arrowAnim.PushBack({ 0,42,179,135 });// 1
	arrowAnim.PushBack({ 220,42,179,135 });// 2
	arrowAnim.PushBack({ 442,42,179,135 });// 3
	arrowAnim.PushBack({ 663,42,179,135 });// 4
	arrowAnim.PushBack({ 884,42,179,135 });// 5
	arrowAnim.PushBack({ 1105,42,179,135 });// 6
	arrowAnim.PushBack({ 1328,42,179,135 });// 7
	arrowAnim.PushBack({ 1105,42,179,135 });// 6
	arrowAnim.PushBack({ 884,42,179,135 });// 5
	arrowAnim.PushBack({ 663,42,179,135 });// 4
	arrowAnim.PushBack({ 442,42,179,135 });// 3
	arrowAnim.PushBack({ 220,42,179,135 });// 2

	arrowAnim.loop = true;
	arrowAnim.speed = 0.2f;
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
	//Set Initial Scene
	SetScene(MAIN_MENU);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate(float dt)
{
	//if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	//	app->fade->FadeToBlack(this, this);

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	/*if (scene == MAIN_MENU) LOG("MAIN_MENU");
	else if (scene == LEVEL_SELECTOR) LOG("LEVEL_SELECTOR");
	else if (scene == LEVEL_1) LOG("LEVEL_1");
	else if (scene == LEVEL_2) LOG("LEVEL_2");
	else if (scene == LEVEL_3) LOG("LEVEL_3");
	else if (scene == PAUSE_MENU) LOG("PAUSE_MENU");*/

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

	if (meteorTexture != nullptr && meteorTexture != NULL)
		app->tex->UnLoad(meteorTexture);

	if (levelsBackground != nullptr && levelsBackground != NULL)
		app->tex->UnLoad(levelsBackground);

	if (mainMenuBackground != nullptr && mainMenuBackground != NULL)
		app->tex->UnLoad(mainMenuBackground);

	if (mainMenuArrow.arrowTex != nullptr && mainMenuArrow.arrowTex != NULL)
		app->tex->UnLoad(mainMenuArrow.arrowTex);

	if (levelSelectArrow.arrowTex != nullptr && levelSelectArrow.arrowTex != NULL)
		app->tex->UnLoad(levelSelectArrow.arrowTex);

	if (levelSelectBackground != nullptr && levelSelectBackground != NULL)
		app->tex->UnLoad(levelSelectBackground);

	if (pauseMenu != nullptr && pauseMenu != NULL)
		app->tex->UnLoad(pauseMenu);

	if (pauseMenuArrow.arrowTex != nullptr && pauseMenuArrow.arrowTex != NULL)
		app->tex->UnLoad(pauseMenuArrow.arrowTex);

	if (pauseMenuGradient != nullptr && pauseMenuGradient != NULL)
		app->tex->UnLoad(pauseMenuGradient);

	ListItem<Planet*>* listPlanet;
	for (listPlanet = planets.start; listPlanet != NULL; listPlanet = listPlanet->next)
	{
		delete listPlanet->data;
		planets.del(listPlanet);
	}
	planets.clear();

	ListItem<Meteor*>* listMeteors;
	for (listMeteors = meteors.start; listMeteors != NULL; listMeteors = listMeteors->next)
	{
		delete listMeteors->data;
		meteors.del(listMeteors);
	}
	meteors.clear();

	app->player->Disable();
	app->physics->CleanUp();
	theVoid = nullptr;

	return true;
}

//UPDATERS

void Scene::UpdateMainMenu()
{
	arrowAnim.Update();

	app->render->DrawTexture(mainMenuBackground, 0, 0);

	app->render->DrawTexture(mainMenuArrow.arrowTex, mainMenuArrow.position[mainMenuArrow.selection].x, mainMenuArrow.position[mainMenuArrow.selection].y, &arrowAnim.GetCurrentFrame());

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->audio->PlayFx(SFxSelectOption);
		if (mainMenuArrow.selection == 1) SetScene(LEVEL_SELECTOR);
		else if (mainMenuArrow.selection == 2) skip = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && mainMenuArrow.selection == 2)
	{
		app->audio->PlayFx(SFxChangeOption);
		mainMenuArrow.selection = 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && mainMenuArrow.selection == 1)
	{
		app->audio->PlayFx(SFxChangeOption);
		mainMenuArrow.selection = 2;
	}
}

void Scene::UpdateLevelSelector()
{
	arrowAnim.Update();

	app->render->DrawTexture(levelSelectBackground, 0, 0);

	app->render->DrawTexture(levelSelectArrow.arrowTex, levelSelectArrow.position[levelSelectArrow.selection].x, levelSelectArrow.position[levelSelectArrow.selection].y, &arrowAnim.GetCurrentFrame());

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->audio->PlayFx(SFxSelectOption);
		if (levelSelectArrow.selection == 1) SetScene(LEVEL_1);
		else if (levelSelectArrow.selection == 2) SetScene(LEVEL_2);
		else if (levelSelectArrow.selection == 3) SetScene(LEVEL_3);
		else if (levelSelectArrow.selection == 4) SetScene(MAIN_MENU);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && levelSelectArrow.selection != 1 && levelSelectArrow.selection != 4)
	{
		levelSelectArrow.selection--;
		app->audio->PlayFx(SFxChangeOption);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && levelSelectArrow.selection != 3 && levelSelectArrow.selection != 4)
	{
		levelSelectArrow.selection++;
		app->audio->PlayFx(SFxChangeOption);
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && levelSelectArrow.selection == 4)
	{
		levelSelectArrow.selection = 1;
		app->audio->PlayFx(SFxChangeOption);
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && levelSelectArrow.selection != 4)
	{
		levelSelectArrow.selection = 4;
		app->audio->PlayFx(SFxChangeOption);
	}
}

void Scene::UpdateLevels()
{
	// Draw background
	app->render->DrawTexture(levelsBackground, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && pause == false)
	{
		SetPauseMenu();
	}

	// Update game
	if (!pause)
	{
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			SetScene(GetScene());

		app->player->onOrbit = false;

		for (ListItem<Planet*>* list = planets.start; list && app->player->onOrbit == false; list = list->next)
		{
			// Check if reached loose condition
			if (list->data == theVoid)
			{
				if (app->player->playerBody->position.y >= theVoid->planetBody->position.y)
				{
					app->audio->PlayFx(SFxDestroyed);
					SetScene(GetScene());
				}
				continue;
			}

			// Check if reached win condition
			if (list->data == theRing)
			{
				if (app->player->playerBody->collider->CheckCollision(list->data->planet, app->player->playerBody->collider->r1))
				{
					// Collision with RING
					switch (GetScene())
					{
					case LEVEL_1:
						SetScene(LEVEL_2);
						break;
					case LEVEL_2:
						SetScene(LEVEL_3);
						break;
					case LEVEL_3:
						SetScene(MAIN_MENU);
						break;
					default:
						break;
					}
				}
			}

			CircleCollider currentPlanet = list->data->planet;
			CircleCollider currentOrbit = list->data->orbit;

			// Check collisions with planets
			if (app->player->onOrbit = app->player->playerBody->collider->CheckCollision(currentOrbit, app->player->playerBody->collider->r1))
			{
				if (app->player->previousOrbitCollider != &list->data->orbit)
				{
					app->audio->PlayFx(SFxOrbitEnter);
					app->player->previousOrbitCollider = &list->data->orbit;
				}
				// Collision with orbit
				LOG("ORBIT!!!!");
				float force = 55.0f;

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
				app->audio->PlayFx(SFxDestroyed);
				LOG("PLANET!!!!");
			}
		}

		// Draw & Check Collisions with meteors
		for (ListItem<Meteor*>* listMeteors = meteors.start; listMeteors; listMeteors = listMeteors->next)
		{
			SDL_Rect toDraw = { 0,0, listMeteors->data->colliderRect.r1.w, listMeteors->data->colliderRect.r1.h };
		
			app->render->DrawTexture(meteorTexture, listMeteors->data->meteorBody->position.x, listMeteors->data->meteorBody->position.y, &toDraw);

			if (listMeteors->data->colliderRect.CheckCollision(listMeteors->data->colliderRect.r1, app->player->playerBody->collider->r1))
			{
				app->audio->PlayFx(SFxDestroyed);
				SetScene(GetScene());
			}
		}
	}
	else
	{
		UpdatePauseMenu();
	}
}

void Scene::UpdatePauseMenu()
{
	app->render->DrawTexture(pauseMenuGradient, 0, 0);

	app->render->DrawTexture(pauseMenu, 340, 760);

	app->render->DrawTexture(pauseMenuArrow.arrowTex, pauseMenuArrow.position[pauseMenuArrow.selection].x, pauseMenuArrow.position[pauseMenuArrow.selection].y);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		pause = false;

		if (pauseMenuArrow.selection == 1) scene = currentScene;
		else if (pauseMenuArrow.selection == 2) SetScene(LEVEL_SELECTOR);
		else if (pauseMenuArrow.selection == 3) SetScene(MAIN_MENU);
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && pauseMenuArrow.selection != 1) pauseMenuArrow.selection--;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && pauseMenuArrow.selection != 3) pauseMenuArrow.selection++;
}


//SETTERS

void Scene::SetScene(SceneType changeScene)
{
	CleanUp();

	scene = changeScene;

	if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == LEVEL_SELECTOR) SetLevelSelector();
	else if (scene == LEVEL_1) SetLevel1();
	else if (scene == LEVEL_2) SetLevel2();
	else if (scene == LEVEL_3) SetLevel3();

}

void Scene::SetMainMenu()
{
	mainMenuBackground = app->tex->Load("Assets/textures/MAIN_MENU_TEMP_BACKGROUND.jpg");

	mainMenuArrow.arrowTex = app->tex->Load("Assets/textures/Arrow_Spritesheet.png");
	mainMenuArrow.selection = 1;

	app->audio->PlayMusicInterpolate("Assets/audio/Music/MAIN_MENU_MUSIC.ogg", 0, 24, 1);
	SFxChangeOption = app->audio->LoadFx("Assets/audio/fx/CHANGE_OPTION_FX.wav");
	SFxSelectOption = app->audio->LoadFx("Assets/audio/fx/SELECT_OPTION_FX.wav");
	//LOAD IMAGE TITLE
	//LOAD IMAGE PLAY
}

void Scene::SetLevelSelector()
{
	levelSelectBackground = app->tex->Load("Assets/textures/LEVEL_SELECTION_TEMP_BACKGROUND.png");

	levelSelectArrow.arrowTex = app->tex->Load("Assets/textures/Arrow_Spritesheet.png");
	levelSelectArrow.selection = 1;

	app->audio->PlayMusicInterpolate("Assets/audio/Music/LEVEL_SELECTOR_MUSIC.ogg", 0, 24, 2);
	SFxChangeOption = app->audio->LoadFx("Assets/audio/fx/CHANGE_OPTION_FX.wav");
	SFxSelectOption = app->audio->LoadFx("Assets/audio/fx/SELECT_OPTION_FX.wav");
	//LOAD IMAGE LEVEL 1
	//LOAD IMAGE LEVEL 2
	//LOAD IMAGE LEVEL 3
}

void Scene::SetLevel1()
{
	if (app->player->IsEnabled() == false)
	{
		app->player->Enable();
	}

	levelsBackground = app->tex->Load("Assets/textures/backgroundMod.jpg");
	meteorTexture = app->tex->Load("Assets/textures/MeteorTexture2.jpg");

	app->audio->PlayMusic("Assets/audio/Music/GAME_MUSIC.ogg", 0);
	SFxOrbitEnter = app->audio->LoadFx("Assets/audio/fx/ORBIT_ENTER_FX.wav");
	SFxDestroyed = app->audio->LoadFx("Assets/audio/fx/CRASH_SHIP_FX.wav");

	AddPlanet(CircleCollider(150, 150, 100), 10);
	AddPlanet(CircleCollider(150, 500, 100), 10);

	AddMeteor(Collider({ 300,60,20,180 }));
	AddMeteor(Collider({ 300,360,20,220 }));
	
	AddPlanet(CircleCollider(540, 150, 100), 10);
	AddPlanet(CircleCollider(480, 700, 100), 10);
	AddPlanet(CircleCollider(800, 400, 100), 10);

	AddMeteor(Collider({ 700,570,20,250 }));

	AddMeteor(Collider({ 920,100,20,300 }));
	AddMeteor(Collider({ 920,420,300,20 }));

	AddPlanet(CircleCollider(1100, 700, 100), 10);
	AddPlanet(CircleCollider(1600, 600, 100), 10);
	AddPlanet(CircleCollider(1100, 200, 100), 10);

	AddMeteor(Collider({ 1500,280,400,20 }));

	theVoid = AddPlanet(CircleCollider(1920 / 2, 1500, 800), 600);

	theRing = AddPlanet(CircleCollider(1700, 120, 100), 80);

	app->player->theVoidPos = theVoid->planetBody->position;

	app->player->playerBody->position = { 120, 20 };
	app->player->playerBody->velocity = { 0, 0 };

	// To adjust colliders to their correct positions, only once in the start
	app->physics->Step(1.0f / 60.0f);
}

void Scene::SetLevel2()
{
	// SET LEVEL 2, VOID APPEARS

	if (app->player->IsEnabled() == false)
	{
		app->player->Enable();
	}

	levelsBackground = app->tex->Load("Assets/textures/backgroundMod.jpg");
	meteorTexture = app->tex->Load("Assets/textures/MeteorTexture2.jpg");

	app->audio->PlayMusic("Assets/audio/Music/GAME_MUSIC.ogg", 0);
	SFxOrbitEnter = app->audio->LoadFx("Assets/audio/fx/ORBIT_ENTER_FX.wav");
	SFxDestroyed = app->audio->LoadFx("Assets/audio/fx/CRASH_SHIP_FX.wav");

	AddPlanet(CircleCollider(150, 150, 80), 10);
	AddPlanet(CircleCollider(450, 0, 80), 10);
	AddPlanet(CircleCollider(250, 500, 80), 10);

	AddMeteor(Collider({ 450, 150, 20, 220 }));
	AddMeteor(Collider({ 450, 600, 20, 220 }));

	AddPlanet(CircleCollider(700, 200, 80), 10);
	AddPlanet(CircleCollider(700, 450, 80), 10);

	AddMeteor(Collider({ 700, 0, 20, 50 }));
	AddMeteor(Collider({ 800, 300, 220, 20 }));

	AddPlanet(CircleCollider(1000, 0, 80), 10);
	AddPlanet(CircleCollider(1000, 700, 80), 10);

	AddPlanet(CircleCollider(1300, 400, 80), 10);
	AddPlanet(CircleCollider(1400, 50, 80), 5);

	AddMeteor(Collider({ 1200, 0, 100, 20 }));
	AddMeteor(Collider({ 1280, 50, 20, 125 }));

	AddMeteor(Collider({ 1600, 150, 20, 500 }));

	theVoid = AddPlanet(CircleCollider(1920 / 2, 1500, 800), 600);

	theRing = AddPlanet(CircleCollider(1800, 120, 100), 80);

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

	//Set Meteors Gravity
	ListItem<Meteor*>* list2;
	for (list2 = meteors.start; list2 != NULL; list2 = list2->next)
	{
		float force = 1.0f;

		fPoint directionGravity = theVoid->planetBody->position - list2->data->meteorBody->position;

		float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

		directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
		directionGravity = { directionGravity.x * force, directionGravity.y * force };

		list2->data->meteorBody->SetGravityAcceleration(directionGravity); 
	}

	// To adjust colliders to their correct positions, only once in the start
	app->physics->Step(1.0f / 60.0f);
}

void Scene::SetLevel3()
{
	// SET LEVEL 3, VOID APPEARS FASTLY
	if (app->player->IsEnabled() == false)
	{
		app->player->Enable();
	}

	levelsBackground = app->tex->Load("Assets/textures/backgroundMod.jpg");
	meteorTexture = app->tex->Load("Assets/textures/MeteorTexture2.jpg");

	app->audio->PlayMusic("Assets/audio/Music/GAME_MUSIC.ogg", 0);
	SFxOrbitEnter = app->audio->LoadFx("Assets/audio/fx/ORBIT_ENTER_FX.wav");
	SFxDestroyed = app->audio->LoadFx("Assets/audio/fx/CRASH_SHIP_FX.wav");

	// To adjust colliders to their correct positions, only once in the start
	app->physics->Step(1.0f / 60.0f);
}

void Scene::SetPauseMenu()
{
	currentScene = scene;
	pause = true;
	scene = PAUSE_MENU;
	

	pauseMenu = app->tex->Load("Assets/textures/Menu.png");
	pauseMenuGradient = app->tex->Load("Assets/textures/MENU_GRADIENT.png");

	pauseMenuArrow.arrowTex = app->tex->Load("Assets/textures/SELECTOR_ARROW_TEMP.png");
	pauseMenuArrow.selection = 1;


	// LOAD IMAGE EXTRAS (Ship images, fire images, logo image...) [Optional]
	// LOAD IMAGE OPTIONS
		// RETURN MAIN MENU
		// RETURN LEVEL SELECTOR
		// RETURN GAME
	// LOAD IMAGE SELECTOR TRIANGLE
}
