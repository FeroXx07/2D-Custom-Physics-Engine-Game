#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Physics.h"
#include "List.h"

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

struct SelectorArrow
{
	SelectorArrow() : position{ { -300, 0 }, {400, 600}, {400, 775} }{}
	SDL_Texture* arrowTex;
	const iPoint position[3]; //position[0] == OUT OF WINDOW // position[1] == PLAY GAME // position[2] == SKIP GAME
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
	SceneType scene;

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
	SDL_Texture* img;
	SDL_Texture* mainMenuBackground;
	SelectorArrow mainMenuArrow;
	SelectorArrow LevelSelectArrow;

public:
	List<Planet*> planets;
	List<Body*> bodies;


	Planet* theVoid = nullptr;
	Planet* AddPlanet(CircleCollider& orbit, int planetRadius)
	{
		CircleCollider planet = orbit;
		planet.radius = planetRadius;
		Planet* p = new Planet(planet, orbit);
		p->planetBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::UNDEFINED, { orbit.x,orbit.y }, NULL, &(p->orbit), { 0.0f,0.0f }, { 0.0f,0.0f });
		p->planetBody->mass = 1000.0f;
		
		p->orbitBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::UNDEFINED, { orbit.x,orbit.y }, NULL, &(p->planet), { 0.0f,0.0f }, { 0.0f,0.0f });
		p->orbitBody->mass = 1000.0f;

		planets.add(p);
		return p;
	}
};

#endif // __SCENE_H__