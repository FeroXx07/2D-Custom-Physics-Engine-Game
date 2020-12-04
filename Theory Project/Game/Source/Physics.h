#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Collider.h"
#include "List.h"
#include "Log.h"
#include "Point.h"
#include "Collider.h"
#include "DynArray.h"

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

	Body(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, SDL_Texture * texture_ = NULL, Collider * collider_ = NULL, uint mass_ = 1) // Constructor with body type and collider type
	{
		bodyType = bodyType_;
		colliderType = colliderType_;
		texture = texture_;
		collider = collider_;
		mass = mass_;
	}

public:
	Collider* collider;
	SDL_Texture* texture;
	
	BodyType bodyType;
	ColliderType colliderType;

	fPoint position;
	fPoint rotation;
	uint mass;
};

class StaticBody : public Body
{
public:
	StaticBody() :Body(BodyType::STATIC_BODY) {}
	StaticBody(ColliderType colliderType_ ,SDL_Texture* texture_, Collider* collider_, uint mass_) :Body(BodyType::STATIC_BODY, colliderType_, texture_, collider_, mass_) {}
};

class DynamicBody : public Body
{
public:
	DynamicBody() :Body(BodyType::DYNAMIC_BODY) {}
	DynamicBody(fPoint velocity_, fPoint gravity_, fPoint acceleration_, ColliderType colliderType_, SDL_Texture* texture_, Collider* collider_, uint mass_) :Body(BodyType::DYNAMIC_BODY, colliderType_, texture_, collider_, mass_)
	{
		velocity = velocity_;
		gravityAcceleration = gravity_;
		acceleration = acceleration_;
	}

public:
	fPoint velocity;
	fPoint gravityAcceleration;
	fPoint acceleration;
	fPoint sumForces = {0.0f, 0.0f};
	DynArray<fPoint> forces;

public:
	void ChangeGravity();
	void ApplyForce(fPoint Newtons);
	void ApplyForce(int NewtonsX, int NewtonsY = 0);
	void ApplyTorque();
	void Rotate();
	void SumTotalForces();
	void SecondNewton();
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
	Body* CreateBody(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, SDL_Texture* texture_ = NULL, Collider* collider_ = NULL, fPoint velocity_ = { 0.0f,0.0f }, fPoint gravity_ = { 0.0f,0.0f }, fPoint acceleration_ = { 0.0f,0.0f }, uint mass_ = 1);
	void Step(float dt);

	//... Apply forces/impulses functions for example
	void Integrate(DynamicBody*item,float dt);
	void Draw(Body* body_);
	void ChangeGravityAcceleration(fPoint acceleration);
	//... Getters functions for example
private:
	// Debug 
	void DebugDraw();
	bool debug = true;
};


#endif