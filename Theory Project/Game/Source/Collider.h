#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"
#include "List.h"

struct SDL_Rect;

class Collider
{
public:

	Collider();

	Collider(SDL_Rect rectangle, Module* listener = nullptr) : r1(rectangle), listener(listener){}

	~Collider();

	bool Start();

	bool CleanUp();

	void ClearColliders();

public:

	//Variables
	SDL_Rect r1;
	bool pendingToDelete = false;
	Module* listener = nullptr;

	bool CheckCollision(SDL_Rect r1, SDL_Rect r2) const
	{
		if ((r1.x > r2.x + r2.w) || (r1.x + r1.w < r2.x) || (r1.y > r2.y + r2.h) || (r1.y + r1.h < r2.y))
		{
			return false;
		}

		return true;
	}

	void SetPos(int x, int y)
	{
		r1.x = x;
		r1.y = y;
	}
};

#endif
