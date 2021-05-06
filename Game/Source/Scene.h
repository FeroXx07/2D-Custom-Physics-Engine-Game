#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Physics.h"
#include "List.h"
#include "Animation.h"

struct SDL_Texture;

enum SceneType
{
	MAIN_MENU = -1,
	LEVEL_SELECTOR,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	PAUSE_MENU
};

enum PlanetType
{
	ROCK,
	WATER
};
struct Planet
{
public:
	Planet(CircleCollider& planet_, CircleCollider& orbit_) : planet(planet_), orbit(orbit_) 
	{
		orbit.x = planet.x;
		orbit.y = planet.y;
	}
	DynamicBody* planetBody = nullptr;
	DynamicBody* orbitBody = nullptr;

	CircleCollider planet = { 0,0,0 };
	CircleCollider orbit = { 0,0,0 };
	PlanetType type = ROCK;
};

struct Meteor
{
public:
	Meteor(Collider& meteor_) : colliderRect(meteor_) {}
	DynamicBody* meteorBody = nullptr;
	Collider colliderRect = { {0,0,0,0}, };
};

struct MMSelectorArrow
{
	MMSelectorArrow() : position{ { -300, 0 }, {525, 675}, {525, 850} }{}
	SDL_Texture* arrowTex;
	const iPoint position[3]; //position[0] == OUT OF WINDOW // position[1] == PLAY GAME // position[2] == SKIP GAME
	uint selection;
};

struct LSSelectorArrow
{
	LSSelectorArrow() : position{ { -300, 0 }, {300 - 150, 540}, {825 - 150, 540}, {1350 - 150, 540}, {300, 810} } {}
	SDL_Texture* arrowTex;
	const iPoint position[5]; //position[0] == OUT OF WINDOW // position[1] == LEVEL 1 // position[2] == LEVEL 2 // position[3] == LEVEL 3 // position[4] == BACK TO MAIN MENU
	uint selection;
};

struct PMSelectorArrow
{
	PMSelectorArrow() : position{ { -300, 0 }, {650,440}, {600, 510}, {700, 580} } {}
	SDL_Texture* arrowTex;
	const iPoint position[4]; //position[0] == OUT OF WINDOW // position[1] == BACK TO GAME // position[2] == BACK TO LVL SELECT // position[3] == BACK TO MAIN MENU
	uint selection;
};

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	SceneType GetScene() const
	{
		return scene;
	}

	void SetScene(SceneType changeScene);

private:

	bool skip = false;
	bool pause = false;

	SceneType scene;
	SceneType currentScene;

	void UpdateMainMenu();
	void UpdateLevelSelector();
	void UpdateLevels();
	void UpdatePauseMenu();

	void SetMainMenu();
	void SetLevelSelector();
	void SetLevel1();
	void SetLevel2();
	void SetLevel3();
	void SetPauseMenu();

	void IterateMeteors();
	void IteratePlanets();

private:
	SDL_Texture* mainMenuBackgroundTex;
	SDL_Texture* levelSelectBackgroundTex;
	SDL_Texture* pauseMenuTex;
	SDL_Texture* pauseMenuGradientTex;
	SDL_Texture* levelsBackgroundTex;
	SDL_Texture* meteorTexture;
	SDL_Texture* rockPlanetTexture;
	SDL_Texture* waterPlanetTexture;
	SDL_Texture* orbitTexture;
	SDL_Texture* theVoidTexture;
	SDL_Texture* levelSelectionSpritesheet;
	SDL_Texture* mainMenuSpritesheet;
	SDL_Texture* mainMenuTitle;
	SDL_Texture* ringAndMeteorTexture;
	SDL_Texture* levelDifficultyTexture;

	const SDL_Rect lvlSelect = { 0, 0, 506, 76 };
	const SDL_Rect back = { 506, 0, 306, 76 };
	const SDL_Rect pauseRect = { 0, 0, 585, 314 };

	MMSelectorArrow mainMenuArrow;
	LSSelectorArrow levelSelectArrow;
	PMSelectorArrow pauseMenuArrow;

	Animation arrowAnim;
	Animation playAnim;
	Animation quitAnim;
	Animation titleAnim;
	Animation theVoidAnim;
	Animation selectLvl1Anim;
	Animation selectLvl2Anim;
	Animation selectLvl3Anim;
	Animation pauseResume;
	Animation pauseLS;
	Animation pauseMM;
	Animation theRingAnim;

	Animation level1StartAnim;
	Animation level2StartAnim;
	Animation level3StartAnim;
	Animation winnerAnim;

	uint SFxOrbitEnter;
	uint SFxDestroyed;

	uint SFxSelectOption;
	uint SFxChangeOption;

	float counterSpawn;
	float winCounter;
	bool winnner = false;
	CircleCollider listRandom[5];
	int previousRandom;

	float counter;
public:
	List<Planet*> planets;
	List<Body*> bodies;
	List<Meteor*> meteors;

	Planet* theVoid = nullptr;
	Planet* theRing = nullptr;
	Planet* AddPlanet(CircleCollider& orbit, int planetRadius, SString name = "rockPlanet")
	{
		CircleCollider planet = orbit;
		planet.radius = planetRadius;
		
		Planet* p = new Planet(planet, orbit);
		p->planetBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::UNDEFINED, { orbit.x,orbit.y }, NULL, &(p->orbit), { 0.0f,0.0f }, { 0.0f,0.0f });
		p->planetBody->mass = 1000.0f;
		p->planetBody->name.Create(name.GetString());

		p->orbitBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::UNDEFINED, { orbit.x,orbit.y }, NULL, &(p->planet), { 0.0f,0.0f }, { 0.0f,0.0f });
		p->orbitBody->mass = 1000.0f;
		p->orbitBody->name.Create("orbit");

		planets.add(p);
		return p;
	}
	Meteor* AddMeteor(Collider& meteor)
	{
		Meteor* m = new Meteor(meteor);
		Collider* rect = new Collider(meteor);

		m->meteorBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::UNDEFINED, { (float)(rect->r1.x),(float)(rect->r1.y) }, NULL, rect, { 0.0f,0.0f }, { 0.0f,0.0f });
		m->meteorBody->mass = 800.0f;
		m->meteorBody->isCollidable = false;
		m->meteorBody->name.Create("meteor");
		meteors.add(m);
		return m;
	}
};

#endif // __SCENE_H__