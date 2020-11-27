#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Physics::Physics() : Module()
{
	name.Create("physics");
}

// Destructor
Physics::~Physics()
{}


bool Physics::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}


bool Physics::Start()
{
	
	return true;
}

bool Physics::Update(float dt)
{
	Step(dt);

	return true;
}


bool Physics::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	// Collisions part: 

	// Debug part:
	DebugDraw();
	return ret;
}

bool Physics::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Physics::DebugDraw()
{
	if (debug)
	{
		ListItem<Body*>* list;

		for (list = bodyList.start; list != NULL; list = list->next)
		{
			// Integrate
			Draw(list->data);
		}
	}
}

void Physics::Integrate(DynamicBody* item, float dt)
{
	// Sum of all acceleration in both axis
	// sum of all forces
	// second law newton (divide by mass) and gett total acceleration
	float sumAccelerationY = (item->acceleration.y + item->gravity.y);


	item->position.x += item->velocity.x * dt + 0.5f * item->acceleration.x * dt * dt;
	item->velocity.x += item->acceleration.x * dt;

	item->position.y += item->velocity.y * dt + 0.5f * sumAccelerationY * dt * dt;
	item->velocity.y += sumAccelerationY * dt;
}

void Physics::Step(float dt)
{
	// Integrate
	ListItem<Body*>* list;

	for (list = bodyList.start; list != NULL; list = list->next)
	{
		// Integrate
		if (list->data->bodyType == BodyType::DYNAMIC_BODY)
		{
			Integrate((DynamicBody*)list->data, dt);
			list->data->collider->SetPos(list->data->position.x, list->data->position.y);
		}
	}

	//Set collider correctly
}

void Physics::Draw(Body* body)
{
	if (body->collider != NULL)
	{
		app->render->DrawRectangle(body->collider->r1, 0, 100, 0);
	}
}

Body* Physics::CreateBody(BodyType bodyType_, ColliderType colliderType_ , SDL_Texture* texture_ , Collider* collider_ , fPoint velocity_ , fPoint gravity_ , fPoint acceleration_ , uint mass_)
{
	if (bodyType_ == STATIC_BODY)
	{
		Body* newBody = new StaticBody(colliderType_, texture_, collider_,mass_);
		bodyList.add(newBody);
		return newBody;
	}
	else if (bodyType_ == DYNAMIC_BODY)
	{
		Body* newBody = new DynamicBody(velocity_, gravity_, acceleration_,colliderType_, texture_, collider_,mass_);
		bodyList.add(newBody);
		return newBody;
	}
}

void DynamicBody::ApplyForce(iPoint newtons)
{
	// F = ma
	this->acceleration.x += newtons.x / this->mass;
	this->acceleration.y += newtons.y / this->mass;
}

void DynamicBody::ApplyForce(int newtonsX, int newtonsY)
{
	// F = ma
	this->acceleration.x += newtonsX / this->mass;
	this->acceleration.y += newtonsY / this->mass;
}
