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

	CircleCollider planet;
	CircleCollider orbit;
};

struct Meteor
{
public:
	Meteor(Collider& meteor_) : colliderRect(meteor_) {}
	DynamicBody* meteorBody = nullptr;
	Collider colliderRect;
};

struct MMSelectorArrow
{
	MMSelectorArrow() : position{ { -300, 0 }, {525, 675}, {525, 800} }{}
	SDL_Texture* arrowTex;
	const iPoint position[3]; //position[0] == OUT OF WINDOW // position[1] == PLAY GAME // position[2] == SKIP GAME
	uint selection;
};

struct LSSelectorArrow
{
	LSSelectorArrow() : position{ { -300, 0 }, {300, 540}, {825, 540}, {1350, 540}, {300, 810} } {}
	SDL_Texture* arrowTex;
	const iPoint position[5]; //position[0] == OUT OF WINDOW // position[1] == LEVEL 1 // position[2] == LEVEL 2 // position[3] == LEVEL 3 // position[4] == BACK TO MAIN MENU
	uint selection;
};

struct PMSelectorArrow
{
	PMSelectorArrow() : position{ { -300, 0 }, {260,733}, {260, 772}, {260, 810} } {}
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

private:
	SDL_Texture* mainMenuBackground;
	SDL_Texture* levelSelectBackground;
	SDL_Texture* pauseMenu;
	SDL_Texture* pauseMenuGradient;
	SDL_Texture* levelsBackground;
	SDL_Texture* meteorTexture;

	MMSelectorArrow mainMenuArrow;
	LSSelectorArrow levelSelectArrow;
	PMSelectorArrow pauseMenuArrow;

	Animation arrowAnim;

	uint SFxOrbitEnter;
	uint SFxDestroyed;

	uint SFxSelectOption;
	uint SFxChangeOption;
public:
	List<Planet*> planets;
	List<Body*> bodies;
	List<Meteor*> meteors;

	Planet* theVoid = nullptr;
	Planet* theRing = nullptr;
	Planet* AddPlanet(CircleCollider& orbit, int planetRadius)
	{
		CircleCollider planet = orbit;
		planet.radius = planetRadius;

		Planet* p = new Planet(planet, orbit);
		p->planetBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::UNDEFINED, { orbit.x,orbit.y }, NULL, &(p->orbit), { 0.0f,0.0f }, { 0.0f,0.0f });
		p->planetBody->mass = 1000.0f;
		p->planetBody->name.Create("planet");

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