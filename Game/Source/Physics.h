#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Collider.h"
#include "List.h"
#include "Log.h"
#include "Point.h"
#include "Collider.h"
#include "DynArray.h"
#include "SString.h"

//#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
//#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !
//
#define PIXELS_PER_METER 1.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 1.0f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0 / M_PI)

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
	WALL,
	GROUND,
	//Win...
};

class Body
{
public:

	Body(BodyType bodyType_, int a):bodyType(bodyType_),texture(NULL),collider(NULL){} // Default constructor

	Body(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, fPoint position_ = { 0.0f,0.0f }, SDL_Texture * texture_ = NULL, Collider * collider_ = NULL, uint mass_ = 1) // Constructor with body type and collider type
	{
		name = "noname";
		bodyType = bodyType_;
		position = position_;
		colliderType = colliderType_;
		texture = texture_;
		collider = collider_;
		mass = mass_;
	}

	Body(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, fPoint position_ = { 0.0f,0.0f }, SDL_Texture* texture_ = NULL, CircleCollider* circleCollider_ = NULL, uint mass_ = 1) // Constructor with body type and collider type
	{
		name = "noname";
		bodyType = bodyType_;
		position = position_;
		colliderType = colliderType_;
		texture = texture_;
		circlecollider = circleCollider_;
		collider = NULL;
		mass = mass_;
	}

	void SolveCollision(Body &body);
	void DeClipper(Body &body);
	double ToPositiveAngle(double angle);
public:
	SString name;
	// TODO: Add collider as argument (SDL_Rectangl) that in the constructor creates a "new Collider(); and assign it to collider atribute"
	Collider* collider;
	CircleCollider* circlecollider;
	SDL_Texture* texture;
	
	BodyType bodyType;
	ColliderType colliderType;

	fPoint position;
	float rotation; 
	uint mass;
	bool isCollidable = true;
};

class StaticBody : public Body
{
public:
	StaticBody() :Body(BodyType::STATIC_BODY,0) {}
	StaticBody(fPoint position_ ,ColliderType colliderType_ ,SDL_Texture* texture_, Collider* collider_, uint mass_) :Body(BodyType::STATIC_BODY, colliderType_, position_,texture_, collider_, mass_) {}
	StaticBody(fPoint position_ ,ColliderType colliderType_, SDL_Texture* texture_, CircleCollider* collider_, uint mass_) :Body(BodyType::STATIC_BODY, colliderType_, position_,texture_, collider_, mass_) {}

};

class DynamicBody : public Body
{
public:
	DynamicBody() :Body(BodyType::DYNAMIC_BODY,0) {}
	DynamicBody(fPoint position_,fPoint velocity_, fPoint gravity_, fPoint acceleration_, ColliderType colliderType_, SDL_Texture* texture_, Collider* collider_, uint mass_) :Body(BodyType::DYNAMIC_BODY, colliderType_, position_, texture_, collider_, mass_)
	{
		velocity = velocity_;
		gravityAcceleration = gravity_;
		acceleration = acceleration_;
	}
	DynamicBody(fPoint position_,fPoint velocity_, fPoint gravity_, fPoint acceleration_, ColliderType colliderType_, SDL_Texture* texture_, CircleCollider* circleCollider_, uint mass_) :Body(BodyType::DYNAMIC_BODY, colliderType_, position_, texture_, circleCollider_, mass_)
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
	fPoint coeficientRestitution = { 1.0f,1.0f };
	fPoint coeficientAeroDrag = { 0.0f,0.0f };
	float hydroControlParameter = 0.0f;
	DynArray<fPoint> forces;

public:
	void SetGravityAcceleration(fPoint &gravity);
	void ApplyForce(fPoint Newtons);
	void ApplyForce(int NewtonsX, int NewtonsY = 0);
	void SecondNewton();
	void ApplyAeroDrag();
	void ApplyBuoyancy();
	void ApplyHidroDrag();
public:
	bool onGround, onTop, onWall;
	bool dragAeroActive = true;
	bool liftAeroActive = false;
	bool buoyancyActive = false;
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
	Body* CreateBody(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, fPoint position_ = { 0.0f,0.0f }, SDL_Texture* texture_ = NULL, Collider* collider_ = NULL, fPoint velocity_ = { 0.0f,0.0f }, fPoint gravity_ = { 0.0f,0.0f }, fPoint acceleration_ = { 0.0f,0.0f }, uint mass_ = 1);
	Body* CreateBody(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, fPoint position_ = { 0.0f,0.0f }, SDL_Texture* texture_ = NULL, CircleCollider* collider_ = NULL, fPoint velocity_ = { 0.0f,0.0f }, fPoint gravity_ = { 0.0f,0.0f }, fPoint acceleration_ = { 0.0f,0.0f }, uint mass_ = 1);
	void Step(float dt);

	//... Apply forces/impulses functions for example
	void Integrate(DynamicBody*item,float dt);
	void Draw(Body* body_);
	void ChangeGravityAcceleration(fPoint acceleration);

	void CheckCollisions();
	//... Getters functions for example
	bool debug = false;

private:
	// Debug 
	void DebugDraw();
};


#endif