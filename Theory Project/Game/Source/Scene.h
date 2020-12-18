#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Physics.h"
#include "List.h"

struct SDL_Texture;

struct Planet
{
public:
	Planet(CircleCollider& planet_, CircleCollider& orbit_) : planet(planet_), orbit(orbit_) 
	{
		orbit.x = planet.x;
		orbit.y = planet.y;
	}
	CircleCollider planet;
	CircleCollider orbit;
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

private:
	SDL_Texture* img;
	DynamicBody* theSquareBody;
public:
	List<Planet*> planets;
	Planet* AddPlanet(CircleCollider& planet, CircleCollider& orbit)
	{
		Planet* p = new Planet(planet, orbit);
		planets.add(p);
		return p;
	}
};

#endif // __SCENE_H__