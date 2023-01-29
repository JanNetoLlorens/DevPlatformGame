#include "Heart.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Heart::Heart() : Entity(EntityType::ITEM)
{
	name.Create("heart");
}

Heart::~Heart() {}


bool Heart::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Heart::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircleSensor(position.x + 16, position.y + 16, 16, bodyType::STATIC);

	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::HEART;

	isPicked = false;

	return true;
}

bool Heart::Update()
{
	if (isPicked)
	{
		pbody->body->SetActive(false);
	}

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Heart::CleanUp()
{
	app->tex->UnLoad(texture);
	texturePath = nullptr;
	return true;
}

void Heart::OnCollision(PhysBody* physA, PhysBody* physB)
{

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision Unknown");
		break;
	case ColliderType::PLAYER:
		isPicked = true;
		LOG("Collision PLAYER");
		break;
	}
}