#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#define FORCE 17.0f
#define TURN_ANGLE 2.0f

Player::Player() : Module()
{
	name.Create("player");
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Player::Start()
{
	img = app->tex->Load("Assets/textures/SpaceshipLow.png");

	Collider* theSquareColl = new Collider({ 0,0,10,10 });
	playerBody = (DynamicBody*)app->physics->CreateBody(BodyType::DYNAMIC_BODY, ColliderType::PLAYER, { 120,20 }, NULL, theSquareColl, { 0.0f,0.0f }, { 0.0f,0.0f });
	playerBody->mass = 10;
	playerBody->coeficientRestitution = { 1.0f,0.8f };
	playerBody->coeficientAeroDrag = { 0.01f, 0.01f };
	playerBody->coeficientAeroLift = 0.5f;

	return true;
}

// Called each loop iteration
bool Player::PreUpdate(float dt)
{
	// L02: DONE 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	// L08: TODO 6: Make the camera movement independent of framerate
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= ceil(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += ceil(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= ceil(200.0f * dt);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += ceil(200.0f * dt);

	if (onOrbit == true)
	{
		Input();
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		playerBody->rotation += -TURN_ANGLE;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		playerBody->rotation += TURN_ANGLE;
	}

	onOrbit = false;
	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	if (onOrbit == false)
	{
		// Return to default
		playerBody->coeficientAeroDrag = { 0.0001f, 0.0001f };

		float force = 25.0f;

		fPoint directionGravity = theVoidPos - playerBody->position;

		float magnitude = sqrt(pow(directionGravity.x, 2) + pow(directionGravity.y, 2));

		directionGravity = { directionGravity.x / magnitude, directionGravity.y / magnitude };
		directionGravity = { directionGravity.x * force, directionGravity.y * force };

		playerBody->SetGravityAcceleration(directionGravity);
	}

	LOG("ROTATION = %f", playerBody->rotation);
	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, METERS_TO_PIXELS(playerBody->position.x), METERS_TO_PIXELS(playerBody->position.y), NULL, 0.0f, (playerBody->rotation));
	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(img);
	return true;
}

void Player::Input()
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		float angleRadians = DEGREES_TO_RADIANS(playerBody->rotation);
		fPoint direction = { 0.0f,0.0f };

		if (angleRadians >= 0 && angleRadians <= 90)
		{
			direction.x = playerBody->position.x * cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * sin(angleRadians) * FORCE;
		}
		else if (angleRadians > 90 && angleRadians <= 180)
		{
			direction.x = playerBody->position.x * cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * -sin(angleRadians) * FORCE;
		}
		else if (angleRadians > 180 && angleRadians <= 270)
		{
			direction.x = playerBody->position.x * -cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * -sin(angleRadians) * FORCE;
		}
		else if (angleRadians > 270 && angleRadians <= 360)
		{
			direction.x = playerBody->position.x * -cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * sin(angleRadians) * FORCE;
		}

		playerBody->ApplyForce(direction);
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		float angleRadians = DEGREES_TO_RADIANS(playerBody->rotation);
		fPoint direction = { 0.0f,0.0f };

		if (angleRadians >= 0 && angleRadians <= 90)
		{
			direction.x = playerBody->position.x * cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * sin(angleRadians) * FORCE;
		}
		else if (angleRadians > 90 && angleRadians <= 180)
		{
			direction.x = playerBody->position.x * cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * -sin(angleRadians) * FORCE;
		}
		else if (angleRadians > 180 && angleRadians <= 270)
		{
			direction.x = playerBody->position.x * -cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * -sin(angleRadians) * FORCE;
		}
		else if (angleRadians > 270 && angleRadians <= 360)
		{
			direction.x = playerBody->position.x * -cos(angleRadians) * FORCE;
			direction.y = playerBody->position.y * sin(angleRadians) * FORCE;
		}
		direction.Negate();
		playerBody->ApplyForce(direction);
	}
}
