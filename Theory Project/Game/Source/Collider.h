#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"
#include "List.h"

struct SDL_Rect;

class Collider : public Module
{
public:

	Collider();

	virtual ~Collider();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	bool CheckCollision(SDL_Rect r1, SDL_Rect r2) const;

	void ClearColliders();

	void DebugDraw();

private:
	bool debug = false;
};

#endif
