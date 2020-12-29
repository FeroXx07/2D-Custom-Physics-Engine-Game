#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Physics.h"
#include "List.h"

struct SDL_Texture;
struct Planet;

class Player : public Module
{
public:

	Player();

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

private:
	SDL_Texture* img;
public:
	void Input();

	DynamicBody* playerBody;
	fPoint theVoidPos;
	bool onOrbit = false;

};

#endif // __PLAYER_H__