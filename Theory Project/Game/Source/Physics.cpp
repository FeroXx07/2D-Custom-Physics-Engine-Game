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

			//currentBody->ApplyAeroDrag();
			//currentBody->ApplyAeroLift();

			currentBody->SecondNewton();

			currentBody->acceleration += currentBody->gravityAcceleration;

			Integrate(currentBody, dt);

			currentBody->acceleration = { 0.0f,0.0f };
		}
		list->data->collider->SetPos((int)list->data->position.x, (int)list->data->position.y);

		// Check Colls
		CheckCollisions();
	}

	//Set collider correctly
}

void Physics::Draw(Body* body)
{
	if (body->collider != NULL)
	{
		switch (body->colliderType)
		{
		case PLAYER:
			app->render->DrawRectangle(body->collider->r1, 0, 100, 0); // Green
			break;
		case GROUND:
			app->render->DrawRectangle(body->collider->r1, 0, 191, 255); // Sky blue
			break;
		case WALL:
			app->render->DrawRectangle(body->collider->r1, 123, 104, 238); // Mediumslateblue
			break;
		default:
			break;
		}
		//app->render->DrawRectangle({METERS_TO_PIXELS(body->collider->r1.x),METERS_TO_PIXELS(body->collider->r1.y),METERS_TO_PIXELS(body->collider->r1.w),METERS_TO_PIXELS(body->collider->r1.h)}, 0, 100, 0);
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
	ListItem<Body*>* bodyList1;
	ListItem<Body*>* bodyList2;
	for (bodyList1 = bodyList.start; bodyList1 != NULL; bodyList1 = bodyList1->next)
	{
		for (bodyList2 = bodyList.start; bodyList2 != NULL; bodyList2 = bodyList2->next)
		{
			if (bodyList1->data != bodyList2->data)
			{
				if ((bodyList1->data->collider->CheckCollision(bodyList1->data->collider->r1, bodyList2->data->collider->r1))) // True if collided
				{
					bodyList1->data->SolveCollision(*bodyList2->data);
					bodyList2->data->SolveCollision(*bodyList1->data);
				}
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

	sumForces = { 0.0f,0.0f };
}

void DynamicBody::ApplyAeroDrag()
{
	if (dragAeroActive)
	{
		iPoint direction;

		if (velocity.x > 0)
			direction.x = -1;
		else if (velocity.x < 0 )
			direction.x = 1;

		if (velocity.y > 0)
			direction.y = -1;
		else if (velocity.y < 0)
			direction.y = 1;

		fPoint dragForce;
		dragForce.x = 0.5f * mass * velocity.x * velocity.x * coeficientAeroDrag.x * direction.x;
		dragForce.y = 0.5f * mass * velocity.y * velocity.y * coeficientAeroDrag.y * direction.y;

		if (dragForce.y <= -40.0f)
		{
			LOG("BIG");
		}

		forces.PushBack(dragForce);
	}
}

void DynamicBody::ApplyAeroLift()
{
	fPoint liftForce;
	liftForce.x = 0.0f;
	liftForce.y = -0.5f * mass * velocity.y * velocity.y * coeficientAeroLift;

	forces.PushBack(liftForce);
}

void Body::SolveCollision(Body &body)
{
	this->DeClipper(body); // First declipp then do anything 
}

void Body::DeClipper(Body &body)
{
	if (this->bodyType == DYNAMIC_BODY)
	{
		DynamicBody* currentBody = (DynamicBody*)this;

		if (currentBody->position.y + currentBody->collider->r1.h >= body.position.y && !(currentBody->position.y >= body.position.y) && (currentBody->position.y + currentBody->collider->r1.h <= body.position.y + body.collider->r1.h))
		{
			// Ground
			currentBody->position.y = body.collider->r1.y - currentBody->collider->r1.h - 1;
			currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
		}
		else if ((currentBody->position.y <= body.position.y + body.collider->r1.h) && currentBody->position.y >= body.position.y && !(currentBody->position.y + currentBody->collider->r1.h <= body.position.y + body.collider->r1.h))
		{
			// Top
			currentBody->position.y = body.collider->r1.y + body.collider->r1.h + 1;
			currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
		}

		if ((currentBody->position.x <= body.position.x + body.collider->r1.w) && currentBody->position.x >= body.position.x && !(currentBody->position.x + currentBody->collider->r1.w <= body.position.x + body.collider->r1.w))
		{
			// Left wall
			currentBody->position.x = body.collider->r1.x + body.collider->r1.w + 1;
			currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;

		}
		else if ((currentBody->position.x + currentBody->collider->r1.h >= body.position.x) && !(currentBody->position.x >= body.position.x))
		{
			// Right wall
			currentBody->position.x = body.collider->r1.x - currentBody->collider->r1.h - 1;
			currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
		}

		currentBody->collider->SetPos((int)currentBody->position.x, (int)currentBody->position.y);
	}
}
