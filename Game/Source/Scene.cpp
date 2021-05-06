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
#include <stdlib.h>

#include "Defs.h"
#include "Log.h"

#define FORCE 20.0f
#define TURN_ANGLE 2.0f

Scene::Scene() : Module()
{
	name.Create("scene");

	//ARROW ANIMATION
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

	//VOID ANIMATION
	theVoidAnim.PushBack({ 0,0,794,285 });
	theVoidAnim.PushBack({ 823,0,802,285 });
	theVoidAnim.PushBack({ 1678,0,792,285 });
	theVoidAnim.PushBack({ 823,0,802,285 });
	theVoidAnim.loop = true;
	theVoidAnim.speed = 0.1f;

	//PLAY MAIN MENU ANIMATION
	playAnim.PushBack({ 22, 106, 550, 146 }); //1
	playAnim.PushBack({ 22, 312, 550, 146 }); //2
	playAnim.PushBack({ 22, 518, 550, 146 }); //3
	playAnim.PushBack({ 22, 724, 550, 146 }); //4
	playAnim.PushBack({ 22, 930, 550, 146 }); //5
	playAnim.PushBack({ 22, 1136, 550, 146 }); //6
	playAnim.PushBack({ 22, 1342, 550, 146 }); //7
	playAnim.PushBack({ 22, 106, 550, 146 }); //1
	playAnim.loop = false;
	playAnim.speed = 0.3f;

	//QUIT MAIN MENU ANIMATION
	quitAnim.PushBack({ 764, 106, 550, 146 }); //1
	quitAnim.PushBack({ 764, 312, 550, 146 }); //2
	quitAnim.PushBack({ 764, 518, 550, 146 }); //3
	quitAnim.PushBack({ 764, 724, 550, 146 }); //4
	quitAnim.PushBack({ 764, 930, 550, 146 }); //5
	quitAnim.PushBack({ 764, 1136, 550, 146 }); //6
	quitAnim.PushBack({ 764, 1342, 550, 146 }); //7
	quitAnim.PushBack({ 764, 106, 550, 146 }); //1
	quitAnim.loop = false;
	quitAnim.speed = 0.3f;

	//TITLE MAIN MENU ANIMATION
	titleAnim.PushBack({ 0, 0, 550, 500 }); //1
	titleAnim.PushBack({ 550, 0, 550, 500 }); //2
	titleAnim.PushBack({ 1101, 0, 550, 500 }); //3
	titleAnim.PushBack({ 1651, 0, 550, 500 }); //4
	titleAnim.PushBack({ 2202, 0, 550, 500 }); //5
	titleAnim.PushBack({ 2752, 0, 550, 500 }); //6
	titleAnim.loop = false;
	titleAnim.speed = 0.6f;

	//LVL1 Level Selector ANIMATION
	selectLvl1Anim.PushBack({ 0, 76, 194, 123 }); //1
	selectLvl1Anim.PushBack({ 0, 199, 194, 123 }); //2
	selectLvl1Anim.PushBack({ 0, 322, 194, 123 }); //3
	selectLvl1Anim.PushBack({ 0, 444, 194, 123 }); //4
	selectLvl1Anim.PushBack({ 0, 568, 194, 123 }); //5
	selectLvl1Anim.PushBack({ 0, 76, 194, 123 }); //1
	selectLvl1Anim.loop = false;
	selectLvl1Anim.speed = 0.3f;

	//LVL2 Level Selector ANIMATION
	selectLvl2Anim.PushBack({ 195, 76, 194, 123 }); //1
	selectLvl2Anim.PushBack({ 195, 199, 194, 123 }); //2
	selectLvl2Anim.PushBack({ 195, 322, 194, 123 }); //3
	selectLvl2Anim.PushBack({ 195, 444, 194, 123 }); //4
	selectLvl2Anim.PushBack({ 195, 568, 194, 123 }); //5
	selectLvl2Anim.PushBack({ 195, 76, 194, 123 }); //1
	selectLvl2Anim.loop = false;
	selectLvl2Anim.speed = 0.3f;

	//LVL3 Level Selector ANIMATION
	selectLvl3Anim.PushBack({ 388, 76, 194, 123 }); //1
	selectLvl3Anim.PushBack({ 388, 199, 194, 123 }); //2
	selectLvl3Anim.PushBack({ 388, 322, 194, 123 }); //3
	selectLvl3Anim.PushBack({ 388, 444, 194, 123 }); //4
	selectLvl3Anim.PushBack({ 388, 568, 194, 123 }); //5
	selectLvl3Anim.PushBack({ 388, 76, 194, 123 }); //1
	selectLvl3Anim.loop = false;
	selectLvl3Anim.speed = 0.3f;

	//Level Selector PAUSE MENU ANIMATION
	pauseLS.PushBack({ 0, 314, 320, 72 }); //1
	pauseLS.PushBack({ 0, 386, 320, 72 }); //2
	pauseLS.PushBack({ 0, 458, 320, 72 }); //3
	pauseLS.PushBack({ 0, 531, 320, 72 }); //4
	pauseLS.PushBack({ 0, 602, 320, 72 }); //5
	pauseLS.PushBack({ 0, 674, 320, 72 }); //6
	pauseLS.PushBack({ 0, 314, 320, 72 }); //1
	pauseLS.loop = false;
	pauseLS.speed = 0.3f;

	//Resume PAUSE MENU ANIMATION
	pauseResume.PushBack({ 320, 314, 206, 72 }); //1
	pauseResume.PushBack({ 320, 386, 206, 72 }); //2
	pauseResume.PushBack({ 320, 458, 206, 72 }); //3
	pauseResume.PushBack({ 320, 531, 206, 72 }); //4
	pauseResume.PushBack({ 320, 602, 206, 72 }); //5
	pauseResume.PushBack({ 320, 674, 206, 72 }); //6
	pauseResume.PushBack({ 320, 314, 206, 72 }); //1
	pauseResume.loop = false;
	pauseResume.speed = 0.3f;

	//Menu PAUSE MENU ANIMATION
	pauseMM.PushBack({ 527, 314, 126, 72 }); //1
	pauseMM.PushBack({ 527, 386, 126, 72 }); //2
	pauseMM.PushBack({ 527, 458, 126, 72 }); //3
	pauseMM.PushBack({ 527, 531, 126, 72 }); //4
	pauseMM.PushBack({ 527, 602, 126, 72 }); //5
	pauseMM.PushBack({ 527, 314, 126, 72 }); //1
	pauseMM.loop = false;
	pauseMM.speed = 0.3f;

	theRingAnim.PushBack({ 10,0,97,200 });
	theRingAnim.PushBack({ 126,0,97,200 });
	theRingAnim.PushBack({ 244,0,97,200 });
	theRingAnim.PushBack({ 363,0,97,200 });
	theRingAnim.PushBack({ 480,0,97,200 });
	theRingAnim.PushBack({ 363,0,97,200 });
	theRingAnim.PushBack({ 244,0,97,200 });
	theRingAnim.PushBack({ 126,0,97,200 });
	theRingAnim.loop = true;
	theRingAnim.speed = 0.1f;

	level1StartAnim.PushBack({ 72,35,196,106 });
	level1StartAnim.PushBack({ 267,24,200,106 });
	level1StartAnim.PushBack({ 467,15,200,106 });
	level1StartAnim.PushBack({ 667,7,202,106 });
	level1StartAnim.PushBack({ 869,1,217,106 });
	level1StartAnim.pingpong = true;
	level1StartAnim.speed = 0.1f;

	level2StartAnim.PushBack({ 67,114,200,106 });
	level2StartAnim.PushBack({ 268,114,200,106 });
	level2StartAnim.PushBack({ 480,114,200,106 });
	level2StartAnim.PushBack({ 681,114,220,106 });
	level2StartAnim.PushBack({ 900,114,220,106 });
	level2StartAnim.pingpong = true;
	level2StartAnim.speed = 0.1f;

	level3StartAnim.PushBack({ 67,238,197,106 });
	level3StartAnim.PushBack({ 263,238,200,106 });
	level3StartAnim.PushBack({ 480,238,200,106 });
	level3StartAnim.PushBack({ 692,238,213,106 });
	level3StartAnim.PushBack({ 906,238,220,106 });
	level3StartAnim.pingpong = true;
	level3StartAnim.speed = 0.1f;

	winnerAnim.PushBack({ 0,377,155,61 });
	winnerAnim.PushBack({ 295,377,260,61 });
	winnerAnim.PushBack({ 582,377,260,61 });
	winnerAnim.pingpong = true;
	winnerAnim.speed = 0.1f;
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
	srand(_getpid());
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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;
	if (skip == true) ret = false;

	return ret;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (meteorTexture != nullptr && meteorTexture != NULL)
		app->tex->UnLoad(meteorTexture);

	if (levelsBackgroundTex != nullptr && levelsBackgroundTex != NULL)
		app->tex->UnLoad(levelsBackgroundTex);

	if (mainMenuBackgroundTex != nullptr && mainMenuBackgroundTex != NULL)
		app->tex->UnLoad(mainMenuBackgroundTex);

	if (mainMenuArrow.arrowTex != nullptr && mainMenuArrow.arrowTex != NULL)
		app->tex->UnLoad(mainMenuArrow.arrowTex);

	if (levelSelectArrow.arrowTex != nullptr && levelSelectArrow.arrowTex != NULL)
		app->tex->UnLoad(levelSelectArrow.arrowTex);

	if (levelSelectBackgroundTex != nullptr && levelSelectBackgroundTex != NULL)
		app->tex->UnLoad(levelSelectBackgroundTex);

	if (pauseMenuTex != nullptr && pauseMenuTex != NULL)
		app->tex->UnLoad(pauseMenuTex);

	if (pauseMenuArrow.arrowTex != nullptr && pauseMenuArrow.arrowTex != NULL)
		app->tex->UnLoad(pauseMenuArrow.arrowTex);

	if (pauseMenuGradientTex != nullptr && pauseMenuGradientTex != NULL)
		app->tex->UnLoad(pauseMenuGradientTex);

	if (rockPlanetTexture != nullptr && rockPlanetTexture != NULL)
		app->tex->UnLoad(rockPlanetTexture);

	if (waterPlanetTexture != nullptr && waterPlanetTexture != NULL)
		app->tex->UnLoad(waterPlanetTexture);

	if (orbitTexture != nullptr && orbitTexture != NULL)
		app->tex->UnLoad(orbitTexture);

	if (theVoidTexture != nullptr && theVoidTexture != NULL)
		app->tex->UnLoad(theVoidTexture);

	if (levelSelectionSpritesheet != nullptr && levelSelectionSpritesheet != NULL)
		app->tex->UnLoad(levelSelectionSpritesheet);

	if (mainMenuSpritesheet != nullptr && mainMenuSpritesheet != NULL)
		app->tex->UnLoad(mainMenuSpritesheet);

	if (mainMenuTitle != nullptr && mainMenuTitle != NULL)
		app->tex->UnLoad(mainMenuTitle);

	if (ringAndMeteorTexture != nullptr && ringAndMeteorTexture != NULL)
		app->tex->UnLoad(ringAndMeteorTexture);

	if (levelDifficultyTexture != nullptr && levelDifficultyTexture != NULL)
		app->tex->UnLoad(levelDifficultyTexture);

	ListItem<Planet*>* listPlanet;
	for (listPlanet = planets.start; listPlanet != NULL; listPlanet = listPlanet->next)
	{
		RELEASE(listPlanet->data);
		delete listPlanet->data;
		planets.del(listPlanet);
	}
	planets.clear();

	ListItem<Meteor*>* listMeteors;
	for (listMeteors = meteors.start; listMeteors != NULL; listMeteors = listMeteors->next)
	{
		RELEASE(listMeteors->data);
		delete listMeteors->data;
		meteors.del(listMeteors);
	}
	meteors.clear();

	app->player->Disable();
	app->physics->CleanUp();

	theVoid = nullptr;
	theRing = nullptr;

	return true;
}

//UPDATERS

void Scene::UpdateMainMenu()
{
	arrowAnim.Update();

	app->render->DrawTexture(mainMenuBackgroundTex, 0, 0);

	app->render->DrawTexture(mainMenuTitle, 1920/2 - titleAnim.GetCurrentFrame().w/2, 100, &titleAnim.GetCurrentFrame());
	titleAnim.Update();

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

	//Options Main Menu Animations
	if (mainMenuArrow.selection == 1)
	{
		playAnim.Update();
		quitAnim.Reset();
	}
	else if (mainMenuArrow.selection == 2)
	{
		quitAnim.Update();
		playAnim.Reset();
	}

	app->render->DrawTexture(mainMenuSpritesheet, 725, 675, &playAnim.GetCurrentFrame());
	app->render->DrawTexture(mainMenuSpritesheet, 725, 850, &quitAnim.GetCurrentFrame());
}

void Scene::UpdateLevelSelector()
{
	arrowAnim.Update();

	app->render->DrawTexture(levelSelectBackgroundTex, 0, 0);

	app->render->DrawTexture(levelSelectArrow.arrowTex, levelSelectArrow.position[levelSelectArrow.selection].x, levelSelectArrow.position[levelSelectArrow.selection].y, &arrowAnim.GetCurrentFrame());

	app->render->DrawTexture(levelSelectionSpritesheet, 1920/2 - lvlSelect.w/2, 150, &lvlSelect);

	app->render->DrawTexture(levelSelectionSpritesheet, 300 + 190, 840, &back);

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

	//Options Level Selector Animations
	if (levelSelectArrow.selection == 1)
	{
		selectLvl1Anim.Update();
		selectLvl2Anim.Reset();
		selectLvl3Anim.Reset();
	}
	else if (levelSelectArrow.selection == 2)
	{
		selectLvl2Anim.Update();
		selectLvl1Anim.Reset();
		selectLvl3Anim.Reset();
	}
	else if (levelSelectArrow.selection == 3)
	{
		selectLvl3Anim.Update();
		selectLvl1Anim.Reset();
		selectLvl2Anim.Reset();
	}

	app->render->DrawTexture(levelSelectionSpritesheet, 300 - 150 + 190, 540, &selectLvl1Anim.GetCurrentFrame());
	app->render->DrawTexture(levelSelectionSpritesheet, 825 - 150 + 190, 540, &selectLvl2Anim.GetCurrentFrame());
	app->render->DrawTexture(levelSelectionSpritesheet, 1350 - 150 + 190, 540, &selectLvl3Anim.GetCurrentFrame());
}

void Scene::UpdateLevels()
{
	// Draw background
	app->render->DrawTexture(levelsBackgroundTex, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && pause == false)
	{
		SetPauseMenu();
	}

	// Update game
	if (!pause)
	{
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			SetScene(GetScene());

		// STARTING ANIMATIONS FOR EACH LEVEL
		switch (GetScene())
		{
		case LEVEL_1:
		{
			counter -= 1.0 / 60.0f;
			if (counter >= 0.0f)
			{
				level1StartAnim.Update();
				app->render->DrawTexture(levelDifficultyTexture, 700, 50, &level1StartAnim.GetCurrentFrame());
			}
			break;
		}
		case LEVEL_2:
		{
			counter -= 1.0 / 60.0f;
			if (counter >= 0.0f)
			{
				level2StartAnim.Update();
				app->render->DrawTexture(levelDifficultyTexture, 900, 200, &level2StartAnim.GetCurrentFrame());
			}
			break;
		}
		case LEVEL_3:
		{
			counter -= 1.0 / 60.0f;
			if (counter >= 0.0f)
			{
				level3StartAnim.Update();
				app->render->DrawTexture(levelDifficultyTexture, 750, 50, &level3StartAnim.GetCurrentFrame());
			}
			break;
		}
		default:
			break;
		}


		// WIN CONDITION FOR LEVEL 3
		if (GetScene() == LEVEL_3)
		{
			winCounter += 1.0 / 60.0f;
			counterSpawn += 1.0 / 60.0f;

			if (winnner)
			{
				winnerAnim.Update();
				app->render->DrawTexture(levelDifficultyTexture, 750, 50, &winnerAnim.GetCurrentFrame());
				if (winCounter >= 5.0f)
				{
					SetScene(MAIN_MENU);
				}
			}

			if (winCounter >= 30.0f)
			{
				winnner = true;
				winCounter = 0.0f;
				//SetScene(MAIN_MENU);
			}

			if (counterSpawn >= 2.0f)
			{
				counterSpawn = 0.0f;
				int random = rand() % 5;
				if (previousRandom != random)
				{
					previousRandom = random;
					AddPlanet(listRandom[random], 10);
				}

				ListItem<Planet*>* list;
				for (list = planets.start; list != NULL; list = list->next)
				{
					if (list->data == theVoid)
						continue;

					float multipliyer = 2.5f;

					fPoint directionGravity = theVoid->planetBody->position - list->data->planetBody->position;

					float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

					directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
					directionGravity = { directionGravity.x * multipliyer, directionGravity.y * multipliyer };

					list->data->planetBody->SetGravityAcceleration(directionGravity); 
					list->data->orbitBody->SetGravityAcceleration(directionGravity);
				}
			}
		}

		app->player->onOrbit = false;

		IteratePlanets();
		IterateMeteors();


		theVoidAnim.Update();
		theRingAnim.Update();

		// Draw player
		app->render->DrawTexture(app->player->img, METERS_TO_PIXELS(app->player->playerBody->position.x) - 10, METERS_TO_PIXELS(app->player->playerBody->position.y) - 2, NULL, 0.0f, (app->player->playerBody->rotation));
		
		for (ListItem<Planet*>* list = planets.start; list; list = list->next)
		{
			CircleCollider currentPlanet = list->data->planet;
			CircleCollider currentOrbit = list->data->orbit;

			if (app->physics->debug)
			{
				app->render->DrawCircle(METERS_TO_PIXELS(currentPlanet.x), METERS_TO_PIXELS(currentPlanet.y), currentPlanet.radius, 123, 104, 238);
				app->render->DrawCircle(METERS_TO_PIXELS(currentOrbit.x), METERS_TO_PIXELS(currentOrbit.y), currentOrbit.radius, 0, 191, 255);
			}

			if (list->data == theRing)
			{
				app->render->DrawTexture(ringAndMeteorTexture, theRing->planetBody->position.x - 50, theRing->planetBody->position.y - 100, &theRingAnim.GetCurrentFrame());
				continue;
			}

			// Draw void
			if (list->data == theVoid)
			{
				app->render->DrawTexture(theVoidTexture, theVoid->planetBody->position.x - 400, theVoid->planetBody->position.y - 700, &theVoidAnim.GetCurrentFrame());
				continue;
			}

			// Draw planet & orbit
			if (list->data->type == ROCK)
				app->render->DrawTexture(rockPlanetTexture, currentPlanet.x - 10, currentPlanet.y - 10);
			else if (list->data->type == WATER)
				app->render->DrawTexture(waterPlanetTexture, currentPlanet.x - 10, currentPlanet.y - 10);

			app->render->DrawTexture(orbitTexture, currentOrbit.x - 100, currentOrbit.y - 100);
		}

	}
	else
	{
		UpdatePauseMenu();
	}


}

void Scene::UpdatePauseMenu()
{
	arrowAnim.Update();

	app->render->DrawTexture(pauseMenuGradientTex, 0, 0);

	app->render->DrawTexture(pauseMenuTex, 667, 383, &pauseRect);

	app->render->DrawTexture(pauseMenuArrow.arrowTex, pauseMenuArrow.position[pauseMenuArrow.selection].x, pauseMenuArrow.position[pauseMenuArrow.selection].y, &arrowAnim.GetCurrentFrame());

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		pause = false;

		if (pauseMenuArrow.selection == 1) scene = currentScene;
		else if (pauseMenuArrow.selection == 2) SetScene(LEVEL_SELECTOR);
		else if (pauseMenuArrow.selection == 3) SetScene(MAIN_MENU);
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && pauseMenuArrow.selection != 1) pauseMenuArrow.selection--;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && pauseMenuArrow.selection != 3) pauseMenuArrow.selection++;

	if (pauseMenuArrow.selection == 1)
	{
		pauseResume.Update();
		pauseLS.Reset();
		pauseMM.Reset();
	}
	else if (pauseMenuArrow.selection == 2)
	{
		pauseLS.Update();
		pauseResume.Reset();
		pauseMM.Reset();
	}
	else if (pauseMenuArrow.selection == 3)
	{
		pauseMM.Update();
		pauseResume.Reset();
		pauseLS.Reset();
	}

	app->render->DrawTexture(pauseMenuTex, 856, 461 + 15, &pauseResume.GetCurrentFrame());
	app->render->DrawTexture(pauseMenuTex, 800, 540, &pauseLS.GetCurrentFrame());
	app->render->DrawTexture(pauseMenuTex, 897, 618 - 2, &pauseMM.GetCurrentFrame());
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
	playAnim.Reset();
	quitAnim.Reset();
	mainMenuBackgroundTex = app->tex->Load("Assets/textures/MAIN_MENU_TEMP_BACKGROUND.jpg");

	mainMenuSpritesheet = app->tex->Load("Assets/textures/title_options.png");

	mainMenuTitle = app->tex->Load("Assets/textures/logo_screen.png");
	titleAnim.Reset();

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
	selectLvl1Anim.Reset();
	selectLvl2Anim.Reset();
	selectLvl3Anim.Reset();

	levelSelectBackgroundTex = app->tex->Load("Assets/textures/LEVEL_SELECTION_TEMP_BACKGROUND.png");

	levelSelectArrow.arrowTex = app->tex->Load("Assets/textures/Arrow_Spritesheet.png");
	levelSelectArrow.selection = 1;


	levelSelectionSpritesheet = app->tex->Load("Assets/textures/LEVEL_SELECTION.png");

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

	levelsBackgroundTex = app->tex->Load("Assets/textures/backgroundMod.jpg");
	meteorTexture = app->tex->Load("Assets/textures/MeteorTexture2.jpg");
	rockPlanetTexture = app->tex->Load("Assets/textures/ROCK_PLANET.png");
	waterPlanetTexture = app->tex->Load("Assets/textures/WATER_PLANET.png");
	orbitTexture = app->tex->Load("Assets/textures/ORBIT.png");
	theVoidTexture = app->tex->Load("Assets/textures/void.png");
	ringAndMeteorTexture = app->tex->Load("Assets/textures/final_ring_and_obstacle.png");
	levelDifficultyTexture = app->tex->Load("Assets/textures/difficulty_levels.png");

	app->audio->PlayMusic("Assets/audio/Music/GAME_MUSIC.ogg", 0);
	SFxOrbitEnter = app->audio->LoadFx("Assets/audio/fx/ORBIT_ENTER_FX.wav");
	SFxDestroyed = app->audio->LoadFx("Assets/audio/fx/CRASH_SHIP_FX.wav");

	AddPlanet(CircleCollider(150, 150, 100), 10,"waterPlanet")->type = WATER;
	AddPlanet(CircleCollider(150, 500, 100), 10,"waterPlanet")->type = WATER;

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

	counter = 5.0f;
}

void Scene::SetLevel2()
{
	// SET LEVEL 2, VOID APPEARS

	if (app->player->IsEnabled() == false)
	{
		app->player->Enable();
	}

	levelsBackgroundTex = app->tex->Load("Assets/textures/backgroundMod.jpg");
	meteorTexture = app->tex->Load("Assets/textures/MeteorTexture2.jpg");
	rockPlanetTexture = app->tex->Load("Assets/textures/ROCK_PLANET.png");
	waterPlanetTexture = app->tex->Load("Assets/textures/WATER_PLANET.png");
	orbitTexture = app->tex->Load("Assets/textures/ORBIT.png");
	theVoidTexture = app->tex->Load("Assets/textures/void.png");
	ringAndMeteorTexture = app->tex->Load("Assets/textures/final_ring_and_obstacle.png");
	levelDifficultyTexture = app->tex->Load("Assets/textures/difficulty_levels.png");

	app->audio->PlayMusic("Assets/audio/Music/GAME_MUSIC.ogg", 0);
	SFxOrbitEnter = app->audio->LoadFx("Assets/audio/fx/ORBIT_ENTER_FX.wav");
	SFxDestroyed = app->audio->LoadFx("Assets/audio/fx/CRASH_SHIP_FX.wav");

	AddPlanet(CircleCollider(150, 150, 100), 10);
	AddPlanet(CircleCollider(450, 0, 100), 10);
	AddPlanet(CircleCollider(250, 500, 100), 10);

	AddMeteor(Collider({ 450, 150, 20, 220 }));
	AddMeteor(Collider({ 450, 600, 20, 220 }));

	AddPlanet(CircleCollider(700, 200, 100), 10);
	AddPlanet(CircleCollider(700, 450, 100), 10);

	AddMeteor(Collider({ 700, 0, 20, 50 }));
	AddMeteor(Collider({ 800, 300, 220, 20 }));

	AddPlanet(CircleCollider(1000, 0, 100), 10);
	AddPlanet(CircleCollider(1000, 700, 100), 10);

	AddPlanet(CircleCollider(1300, 400, 100), 10);
	AddPlanet(CircleCollider(1400, 50, 100), 5);

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

	counter = 5.0f;
}

void Scene::SetLevel3()
{
	// SET LEVEL 3, VOID APPEARS FASTLY
	if (app->player->IsEnabled() == false)
	{
		app->player->Enable();
	}

	levelsBackgroundTex = app->tex->Load("Assets/textures/backgroundMod.jpg");
	meteorTexture = app->tex->Load("Assets/textures/MeteorTexture2.jpg");
	rockPlanetTexture = app->tex->Load("Assets/textures/ROCK_PLANET.png");
	waterPlanetTexture = app->tex->Load("Assets/textures/WATER_PLANET.png");
	orbitTexture = app->tex->Load("Assets/textures/ORBIT.png");
	theVoidTexture = app->tex->Load("Assets/textures/void.png");
	ringAndMeteorTexture = app->tex->Load("Assets/textures/final_ring_and_obstacle.png");
	levelDifficultyTexture = app->tex->Load("Assets/textures/difficulty_levels.png");

	app->audio->PlayMusic("Assets/audio/Music/GAME_MUSIC.ogg", 0);
	SFxOrbitEnter = app->audio->LoadFx("Assets/audio/fx/ORBIT_ENTER_FX.wav");
	SFxDestroyed = app->audio->LoadFx("Assets/audio/fx/CRASH_SHIP_FX.wav");

	AddPlanet(CircleCollider(150, 150, 100), 10);
	AddPlanet(CircleCollider(150, 500, 100), 10);

	AddPlanet(CircleCollider(540, 150, 100), 10);
	AddPlanet(CircleCollider(480, 700, 100), 10);
	AddPlanet(CircleCollider(800, 400, 100), 10);

	AddPlanet(CircleCollider(1100, 700, 100), 10);
	AddPlanet(CircleCollider(1600, 600, 100), 10);
	AddPlanet(CircleCollider(1100, 200, 100), 10);

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
	listRandom[0] = CircleCollider(150, -100, 100);
	listRandom[1] = CircleCollider(540, -100, 100);
	listRandom[2] = CircleCollider(800, -100, 100);
	listRandom[3] = CircleCollider(1100, -100, 100);
	listRandom[4] = CircleCollider(1600, -100, 100);

	counterSpawn = 0.0f;
	winCounter = 0.0f;
	counter = 5.0f;
	winnner = false;

	app->physics->Step(1.0f / 60.0f);
}

void Scene::SetPauseMenu()
{
	currentScene = scene;
	pause = true;
	scene = PAUSE_MENU;
	

	pauseMenuTex = app->tex->Load("Assets/textures/pause_menu.png");
	pauseMenuGradientTex = app->tex->Load("Assets/textures/MENU_GRADIENT.png");

	pauseMenuArrow.arrowTex = app->tex->Load("Assets/textures/Arrow_Spritesheet.png");
	pauseMenuArrow.selection = 1;


	// LOAD IMAGE EXTRAS (Ship images, fire images, logo image...) [Optional]
	// LOAD IMAGE OPTIONS
		// RETURN MAIN MENU
		// RETURN LEVEL SELECTOR
		// RETURN GAME
	// LOAD IMAGE SELECTOR TRIANGLE
}

void Scene::IterateMeteors()
{
	int i = 0;
	int count = meteors.count();
	while (i < count)
	{
		Meteor* listMeteors = meteors.At(i)->data;

		SDL_Rect toDraw = { 414,643, listMeteors->colliderRect.r1.w, listMeteors->colliderRect.r1.h };

		app->render->DrawTexture(ringAndMeteorTexture, listMeteors->meteorBody->position.x, listMeteors->meteorBody->position.y, &toDraw);

		if (listMeteors->colliderRect.CheckCollision(listMeteors->colliderRect.r1, app->player->playerBody->collider->r1))
		{
			app->audio->PlayFx(SFxDestroyed);
			SetScene(GetScene());
		}

		++i;
	}
}

void Scene::IteratePlanets()
{
	bool inWater = false;
	int i = 0;
	int count = planets.count();
	while (i < count)
	{
		Planet* currentPlanet = planets.At(i)->data;
		// Check if reached loose condition
		if (currentPlanet == theVoid && winnner == false)
		{
			if (app->player->playerBody->position.y >= theVoid->planetBody->position.y)
			{
				app->audio->PlayFx(SFxDestroyed);
				SetScene(GetScene());
			}
			++i;
			continue;
		}

		// Check if reached win condition
		if (currentPlanet == theRing)
		{
			if (app->player->playerBody->collider->CheckCollision(currentPlanet->planet, app->player->playerBody->collider->r1))
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

		CircleCollider planet = currentPlanet->planet;
		CircleCollider orbit = currentPlanet->orbit;

		// Check collisions with planets
		if (app->player->playerBody->collider->CheckCollision(orbit, app->player->playerBody->collider->r1))
		{
			app->player->onOrbit = true;

			if (app->player->previousOrbitCollider != &currentPlanet->orbit)
			{
				app->audio->PlayFx(SFxOrbitEnter);
				app->player->previousOrbitCollider = &currentPlanet->orbit;
			}
			// Collision with orbit
			LOG("ORBIT!!!!");
			float force = 55.0f;

			fPoint directionGravity = fPoint({ planet.x,planet.y }) - app->player->playerBody->position;

			float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

			directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
			directionGravity = { directionGravity.x * force, directionGravity.y * force };

			app->player->playerBody->SetGravityAcceleration(directionGravity);
			app->player->playerBody->coeficientAeroDrag = { 0.1f, 0.1f };

		}

		if (app->player->playerBody->collider->CheckCollision(planet, app->player->playerBody->collider->r1))
		{
			if (currentPlanet->type == ROCK)
			{
				app->audio->PlayFx(SFxDestroyed);
				SetScene(GetScene());
			}
			if (currentPlanet->type == WATER)
			{
				app->player->playerBody->buoyancyActive = true;
				inWater = true;
			}
		}

		++i;
	}
	//for (ListItem<Planet*>* list = planets.start; list != NULL && list->data != NULL && app->player->onOrbit == false; list = list->next)
	//{
	//	// Check if reached loose condition
	//	if (list->data == theVoid && winnner == false)
	//	{
	//		if (app->player->playerBody->position.y >= theVoid->planetBody->position.y)
	//		{
	//			app->audio->PlayFx(SFxDestroyed);
	//			SetScene(GetScene());
	//		}
	//		continue;
	//	}

	//	// Check if reached win condition
	//	if (list->data == theRing)
	//	{
	//		if (app->player->playerBody->collider->CheckCollision(list->data->planet, app->player->playerBody->collider->r1))
	//		{
	//			// Collision with RING
	//			switch (GetScene())
	//			{
	//			case LEVEL_1:
	//				SetScene(LEVEL_2);
	//				break;
	//			case LEVEL_2:
	//				SetScene(LEVEL_3);
	//				break;
	//			case LEVEL_3:
	//				SetScene(MAIN_MENU);
	//				break;
	//			default:
	//				break;
	//			}
	//		}
	//	}

	//	CircleCollider planet = list->data->planet;
	//	CircleCollider orbit = list->data->orbit;

	//	// Check collisions with planets
	//	if (app->player->onOrbit = app->player->playerBody->collider->CheckCollision(orbit, app->player->playerBody->collider->r1))
	//	{
	//		if (app->player->previousOrbitCollider != &list->data->orbit)
	//		{
	//			app->audio->PlayFx(SFxOrbitEnter);
	//			app->player->previousOrbitCollider = &list->data->orbit;
	//		}
	//		// Collision with orbit
	//		LOG("ORBIT!!!!");
	//		float force = 55.0f;

	//		fPoint directionGravity = fPoint({ planet.x,planet.y }) - app->player->playerBody->position;

	//		float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

	//		directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
	//		directionGravity = { directionGravity.x * force, directionGravity.y * force };

	//		app->player->playerBody->SetGravityAcceleration(directionGravity);
	//		app->player->playerBody->coeficientAeroDrag = { 0.1f, 0.1f };

	//	}

	//	if (app->player->playerBody->collider->CheckCollision(planet, app->player->playerBody->collider->r1))
	//	{
	//		if (list->data->type == ROCK)
	//		{
	//			app->audio->PlayFx(SFxDestroyed);
	//			SetScene(GetScene());
	//		}
	//		if (list->data->type == WATER)
	//		{
	//			app->player->playerBody->buoyancyActive = true;
	//			inWater = true;
	//		}
	//	}
	//}

	if (inWater == false)
	{
		app->player->playerBody->buoyancyActive = false;
		app->player->playerBody->hydroControlParameter = 0.0f;
	}
	else
	{
		app->player->playerBody->hydroControlParameter = 10.0f;
	}
}
