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
	if (dt >= 1.0f / 30.0f)
	{
		dt = 1.0f / 30.0f;
	}

	if (app->scene->GetScene() == PAUSE_MENU) Step(dt/10000);
	else { Step(dt); }

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
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		if (list->data->collider != NULL)
			delete list->data->collider;

		if (list->data->circlecollider != NULL)
			delete list->data->collider;

		delete list->data;
		bodyList.del(list);
	}
	bodyList.clear();
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
	ListItem<Body*>* list;

	for (list = bodyList.start; list != NULL; list = list->next)
	{
		// Correct angles rotation
		list->data->rotation = list->data->ToPositiveAngle(list->data->rotation);

		if (list->data->bodyType == BodyType::DYNAMIC_BODY)
		{
			DynamicBody* dynamicBody = (DynamicBody*)list->data;
			if (dynamicBody->name == SString("player") && dynamicBody->buoyancyActive)
			{
				int a = 0;
			}

			dynamicBody->ApplyHidroDrag();
			dynamicBody->ApplyBuoyancy();
			dynamicBody->ApplyAeroDrag();
			
			//dynamicBody->ApplyAeroLift();

			// Second law newton
			dynamicBody->SecondNewton();

			// Applying gravity acceleration post second law newton
			dynamicBody->acceleration += dynamicBody->gravityAcceleration;

			// Integrate
			Integrate(dynamicBody, dt);

			dynamicBody->acceleration = { 0.0f,0.0f };
		}

		// Setting rect colliders position
		if (list->data->collider != nullptr)
			list->data->collider->SetPos((int)list->data->position.x, (int)list->data->position.y);

		// Setting circle colliders position
		if (list->data->circlecollider != nullptr)
			list->data->circlecollider->SetPos((int)list->data->position.x, (int)list->data->position.y);

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
			// Green
			app->render->DrawRectangle({ METERS_TO_PIXELS(body->collider->r1.x),METERS_TO_PIXELS(body->collider->r1.y),METERS_TO_PIXELS(body->collider->r1.w),METERS_TO_PIXELS(body->collider->r1.h) }, 0, 100, 0);
			break;
		case GROUND:
			// Sky blue
			app->render->DrawRectangle({ METERS_TO_PIXELS(body->collider->r1.x),METERS_TO_PIXELS(body->collider->r1.y),METERS_TO_PIXELS(body->collider->r1.w),METERS_TO_PIXELS(body->collider->r1.h) }, 0, 191, 255);
			break;
		case WALL:
			// Mediumslateblue
			app->render->DrawRectangle({ METERS_TO_PIXELS(body->collider->r1.x),METERS_TO_PIXELS(body->collider->r1.y),METERS_TO_PIXELS(body->collider->r1.w),METERS_TO_PIXELS(body->collider->r1.h) }, 123, 104, 238);
			break;
		default:
			app->render->DrawRectangle({ METERS_TO_PIXELS(body->collider->r1.x),METERS_TO_PIXELS(body->collider->r1.y),METERS_TO_PIXELS(body->collider->r1.w),METERS_TO_PIXELS(body->collider->r1.h) }, 139, 69, 19);
			break;
		}
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
			DynamicBody* dynamicBody = (DynamicBody*)list->data;
			dynamicBody->gravityAcceleration = acceleration;
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
			if (bodyList1->data != bodyList2->data && bodyList1->data->collider != nullptr && bodyList2->data->collider != nullptr)
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

Body* Physics::CreateBody(BodyType bodyType_, ColliderType colliderType_ , fPoint position_ ,SDL_Texture* texture_ , Collider* collider_ , fPoint velocity_ , fPoint gravity_ , fPoint acceleration_ , uint mass_)
{
	if (bodyType_ == STATIC_BODY)
	{
		Body* newBody = new StaticBody(position_, colliderType_, texture_, collider_,mass_);
		bodyList.add(newBody);
		newBody->name = "noName";
		return newBody;
	}
	else if (bodyType_ == DYNAMIC_BODY)
	{
		Body* newBody = new DynamicBody(position_,velocity_, gravity_, acceleration_,colliderType_, texture_, collider_,mass_);
		bodyList.add(newBody);
		newBody->name = "noName";
		return newBody;
	}
}

Body* Physics::CreateBody(BodyType bodyType_, ColliderType colliderType_, fPoint position_ ,SDL_Texture* texture_, CircleCollider* collider_, fPoint velocity_, fPoint gravity_, fPoint acceleration_, uint mass_)
{
	position_ = { PIXEL_TO_METERS(position_.x),PIXEL_TO_METERS(position_.y) };

	if (bodyType_ == STATIC_BODY)
	{
		Body* newBody = new StaticBody(position_,colliderType_, texture_, collider_, mass_);
		bodyList.add(newBody);
		newBody->name = "noName";
		return newBody;
	}
	else if (bodyType_ == DYNAMIC_BODY)
	{
		Body* newBody = new DynamicBody(position_,velocity_, gravity_, acceleration_, colliderType_, texture_, collider_, mass_);
		bodyList.add(newBody);
		newBody->name = "noName";
		return newBody;
	}
}

void DynamicBody::SetGravityAcceleration(fPoint& gravity)
{
	this->gravityAcceleration = gravity;
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

void DynamicBody::ApplyBuoyancy()
{
	if (buoyancyActive)
	{
		fPoint buoyancyForce = this->gravityAcceleration;

		float magnitude = sqrt(pow(this->gravityAcceleration.x, 2) + pow(this->gravityAcceleration.y, 2));

		buoyancyForce = { buoyancyForce.x / magnitude, buoyancyForce.y / magnitude };
		buoyancyForce.Negate();

		fPoint buoyancyForceMagnitude = { 0,0 };
		buoyancyForceMagnitude.x = mass * this->gravityAcceleration.x * velocity.x - mass * this->gravityAcceleration.x;
		buoyancyForceMagnitude.y = mass * this->gravityAcceleration.y * velocity.y - mass * this->gravityAcceleration.y;

		buoyancyForce.x = buoyancyForce.x * buoyancyForceMagnitude.x;
		buoyancyForce.y = buoyancyForce.y * buoyancyForceMagnitude.y;

		forces.PushBack(buoyancyForce);
	}
}

void DynamicBody::ApplyHidroDrag()
{
	if (buoyancyActive)
	{
		fPoint hidroDrag = velocity;

		float magnitude = sqrt(pow(this->velocity.x, 2) + pow(this->velocity.y, 2));

		hidroDrag = { hidroDrag.x / magnitude, hidroDrag.y / magnitude };
		hidroDrag.Negate();

		fPoint hidroDragMagnitude = { 0,0 };
		hidroDragMagnitude.x =velocity.x * this->hydroControlParameter;
		hidroDragMagnitude.y =velocity.y * this->hydroControlParameter;

		hidroDrag.x = hidroDrag.x * hidroDragMagnitude.x;
		hidroDrag.y = hidroDrag.y * hidroDragMagnitude.y;

		forces.PushBack(hidroDrag);
	}
}

void Body::SolveCollision(Body &body)
{
	this->DeClipper(body); // First declipp then do anything 
}

void Body::DeClipper(Body &body)
{
	if (this->bodyType == DYNAMIC_BODY && body.isCollidable == true)
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

double Body::ToPositiveAngle(double angle)
{
	angle = fmod(angle, 360);
	while (angle < 0) { //pretty sure this comparison is valid for doubles and floats
		angle += 360.0;
	}

	return angle;
}