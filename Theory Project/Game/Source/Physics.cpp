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
	ListItem<Body*>* list;

	for (list = bodyList.start; list != NULL; list = list->next)
	{
		// Integrate
		if (list->data->bodyType == BodyType::DYNAMIC_BODY)
			Integrate((DynamicBody*)list->data, dt);
	}

	return true;
}


bool Physics::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	// Collisions part: 

	// Debug part:

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

	}
}

void Physics::Integrate(DynamicBody* item, float dt)
{
	item->position.x += item->velocity.x * dt + 0.5f * item->acceleration.x * dt * dt;
	item->velocity.x += item->acceleration.x * dt;

	item->position.y += item->velocity.y * dt + 0.5f * item->acceleration.y * dt * dt;
	item->velocity.y += item->acceleration.y * dt;
}

Body* Physics::CreateBody(BodyType bodyType_, ColliderType colliderType_ = ColliderType::UNDEFINED, SDL_Texture* texture_ = NULL, Collider* collider_ = NULL)
{
	if (bodyType_ == STATIC_BODY)
	{
		Body* newBody = new StaticBody(bodyType_, colliderType_, texture_, collider_);
		return newBody;
	}
	else if (bodyType_ == DYNAMIC_BODY)
	{
		Body* newBody = new StaticBody(bodyType_, colliderType_, texture_, collider_);
		return newBody;
	}
}