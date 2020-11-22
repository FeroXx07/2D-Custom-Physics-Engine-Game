#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Collider.h"
#include "List.h"
#include "Log.h"
#include "Point.h"
#include "Collider.h"

struct SDL_Texture;

struct Static : public Body
{

};

struct Dynamic : public Body
{

};

class Body : public Module
{
public:

	Body()
	{
		type = NONE;
	}

	Body(const char* type_)
	{
		if (type_ == "STATIC_BODY") 
		{
			type = STATIC_BODY;
		}
		else if (type_ == "DYNAMIC_BODY")
		{
			type = DYNAMIC_BODY;
		}
		else
		{
			LOG("IT HAS NOT BEEN ASIGNED A BODY TYPE ASIGNED WITH THE CONSTRUCTOR USED. BE SURE TO WRITE 'DYNAMIC_BODY' OR 'STATIC_BODY'");
		}
	}


public:
	Dynamic dynamicBody;
	Static staticBody;
	enum Type
	{
		NONE = -1,
		STATIC_BODY,
		DYNAMIC_BODY
	};

	Collider collider;
	SDL_Texture* texture;

private:
	Type type;
	iPoint position;

};

#endif