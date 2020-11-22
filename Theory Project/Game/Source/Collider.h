#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"
#include "List.h"

struct SDL_Rect;

class Collider
{
public:

	Collider();

	virtual ~Collider();

	bool Start();

	bool CleanUp();

	bool CheckCollision(SDL_Rect r1, SDL_Rect r2) const;

	void ClearColliders();
};

#endif
