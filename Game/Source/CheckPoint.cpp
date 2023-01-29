#include "CheckPoint.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Hud.h"

CheckPoint::CheckPoint() : Entity(EntityType::ITEM)
{
	name.Create("Checkpoint");
}

CheckPoint::~CheckPoint() {}


bool CheckPoint::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	checkpoint_.PushBack({ 0, 0, 19, 30 });
	checkpoint_.PushBack({ 19, 0, 19, 30 });
	checkpoint_.PushBack({ 38, 0, 19, 30 });
	checkpoint_.speed = 0.1f;

	checkpointPick_.PushBack({ 0, 30, 19, 30 });
	checkpointPick_.PushBack({ 19, 30, 19, 30 });
	checkpointPick_.PushBack({ 38, 30, 19, 30 });
	checkpointPick_.speed = 0.1f;

	return true;
}

bool CheckPoint::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	currentAnimation = &checkpoint_;

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 19, 30, bodyType::STATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::CHECKPOINT;

	isPicked = false;

	return true;
}

bool CheckPoint::Update()
{
	if (isPicked)
	{
		currentAnimation = &checkpointPick_;
	}
	/*if (app->scene->player->dead && isPicked)
	{
		app->hud->heartsCount = 1;
		app->scene->player->dead = false;
	}*/

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	SDL_Rect rectAnim = currentAnimation->GetCurrentFrame();

	currentAnimation->Update();

	app->render->DrawTexture(texture, position.x, position.y, &rectAnim);

	return true;
}

bool CheckPoint::CleanUp()
{
	app->tex->UnLoad(texture);
	texturePath = nullptr;
	return true;
}

void CheckPoint::OnCollision(PhysBody* physA, PhysBody* physB)
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