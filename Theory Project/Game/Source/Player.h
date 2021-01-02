#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Physics.h"
#include "List.h"
#include "Animation.h"

struct SDL_Texture;
struct Planet;

class Player : public Module
{
public:

	Player(bool isEnabled);

	// Destructor
	virtual ~Player();

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

public:
	SDL_Texture* img;
	
public:
	void Input();
	bool OutOfBounds();

	DynamicBody* playerBody;
	fPoint theVoidPos;
	bool onOrbit = false;
	float attractionForce = 105.0f;

public:
	Animation spaceshipAnim;
	CircleCollider* previousOrbitCollider = nullptr;

};

#endif // __PLAYER_H__