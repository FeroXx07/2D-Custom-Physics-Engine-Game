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

	item->position.x += item->velocity.x * dt + 0.5f * item->acceleration.x * dt * dt;
	item->velocity.x += item->acceleration.x * dt;

	item->position.y += item->velocity.y * dt + 0.5f * item->acceleration.y * dt * dt;
	item->velocity.y += item->acceleration.y * dt;
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
			DynamicBody* currentBody = (DynamicBody*)list->data;
			currentBody->SecondNewton();

			currentBody->acceleration += currentBody->gravityAcceleration;

			Integrate(currentBody, dt);

			currentBody->acceleration = { 0.0f,0.0f };

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
		// TODO: CONVERT TO PIXELS
	}
}

void Physics::ChangeGravityAcceleration(fPoint acceleration)
{
	ListItem<Body*>* list;

	for (list = bodyList.start; list != NULL; list = list->next)
	{
		// Integrate
		if (list->data->bodyType == BodyType::DYNAMIC_BODY)
		{
			DynamicBody* temp = (DynamicBody*)list->data;
			temp->gravityAcceleration = acceleration;
		}
	}
}

void Physics::CheckCollisions()
{
	ListItem<Body*>* list;
	ListItem<Body*>* list2;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		for (list2 = bodyList.start; list2 != NULL; list2 = list2->next)
		{
			if (list->data != list2->data)
			{
				//if (collisions.OnCollision(list->data->collider, list2->data->collider)) // True if collided
				//{

				//}
			}
		}
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

void DynamicBody::ApplyForce(fPoint Newtons)
{
	forces.PushBack(Newtons);
}

void DynamicBody::ApplyForce(int NewtonsX, int NewtonsY)
{
	forces.PushBack({float(NewtonsX), float(NewtonsY)});
}

void DynamicBody::SecondNewton()
{
	for (int i = 0; i < forces.Count(); i++)
	{
		sumForces += *forces.At(i);
		forces.Pop(*forces.At(i));
	}
	forces.Clear();

	acceleration.x += sumForces.x / mass;
	acceleration.y += sumForces.y / mass;

	sumForces = { 0,0 };
}

//bool Collision::OnCollision(Collider* rectA, Collider* rectB)
//{
//	
//	return false;
//}
