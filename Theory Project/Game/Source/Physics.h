#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Collider.h"
#include "List.h"
#include "Log.h"
#include "Point.h"
#include "Collider.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

struct SDL_Texture;

enum BodyType
{
	NONE = -1,
	STATIC_BODY,
	DYNAMIC_BODY
};

enum ColliderType
{
	UNDEFINED = -1,
	PLAYER,
	ENEMY,
	//Win...
};

class Body
{
public:

	Body():bodyType(BodyType::NONE),texture(NULL),collider(NULL){} // Default constructor

	Body(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, SDL_Texture * texture_ = NULL, Collider * collider_ = NULL) // Constructor with body type and collider type
	{
		bodyType = bodyType_;
		colliderType = colliderType_;
		texture = texture_;
		collider = collider_;
	}

	// App->physics->CreateBody(......).-> Will call the constructor and save it's pointer in dynamic memory (PhysBody)

public:
	Collider* collider;
	SDL_Texture* texture;
	
	BodyType bodyType;
	ColliderType colliderType;

	iPoint position;
	fPoint rotation;

};

class StaticBody : public Body
{
public:
	StaticBody() :Body(BodyType::STATIC_BODY) {}
};

class DynamicBody : public Body
{
public:
	DynamicBody() :Body(BodyType::DYNAMIC_BODY) {}

public:
	fPoint velocity;
	fPoint gravity;
	fPoint acceleration;

public:
	void ChangeGravity();
	void ApplyForce();
	void ApplyTorque();
	void Rotate();
	//...
};

// ------------------------------------------------------------------------------------------------------------------------------

class Physics : public Module
{
public:

	Physics();

	virtual ~Physics();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();
	
	bool CleanUp();

public:
	List<Body*> bodyList;
	
public:
	Body* CreateBody(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, SDL_Texture* texture_ = NULL, Collider* collider_ = NULL);
	void Step();

	//... Apply forces/impulses functions for example
	void Integrate(DynamicBody*item,float dt);
	//... Getters functions for example
private:
	// Debug 
	void DebugDraw();
	bool debug = false;
};





#endif