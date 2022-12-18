#include "WalkingEnemy.h"

#include "Entity.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "ModuleFadeToBlack.h"
#include "EntityManager.h"
#include "Map.h"
#include "Pathfinding.h"

WalkingEnemy::WalkingEnemy() : Enemy(EntityType::WALKING_ENEMY)
{
	name.Create("WalkingEnemy");
}

WalkingEnemy::~WalkingEnemy() {

}

bool WalkingEnemy::Awake() {


	return true;
}

bool WalkingEnemy::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	pathfindingTexturePath = parameters.attribute("pathfinding_texturepath").as_string();


	// right walk
	walkRightAnim.PushBack({ 0, 53, 35, 42 });
	walkRightAnim.PushBack({ 35, 53, 35, 42 });
	walkRightAnim.PushBack({ 70, 53, 35, 42 });
	walkRightAnim.PushBack({ 105, 53, 35, 42 });
	walkRightAnim.PushBack({ 140, 53, 35, 42 });
	walkRightAnim.speed = 0.1f;

	// left walk
	walkLeftAnim.PushBack({ 0, 0, 35, 42 });
	walkLeftAnim.PushBack({ 35, 0, 35, 42 });
	walkLeftAnim.PushBack({ 70, 0, 35, 42 });
	walkLeftAnim.PushBack({ 105, 0, 35, 42 });
	walkLeftAnim.PushBack({ 140, 0, 35, 42 });
	walkLeftAnim.speed = 0.1f;

	texture = app->tex->Load(texturePath);
	pathfindingTexture = app->tex->Load(pathfindingTexturePath);

	currentAnimation = &walkLeftAnim;

	pbody = app->physics->CreateCircle(position.x, position.y, 18, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::WALKING_ENEMY;

	velocity.x = 0; velocity.y = 0;

	pathType = PathType::AGRESSIVE;
	

	return true;
}

bool WalkingEnemy::Update()
{
	//pathfinding
	mapPos = app->map->WorldToMap(position.x, position.y);

	if (pathType == PathType::NEUTRAL && !hasPatroledLeft)
	{
		patrolLeft = app->map->WorldToMap(645, 330);
		app->pathfinding->CreatePath(mapPos, patrolLeft);

		if (pbody->body->GetLinearVelocity().x == 0)
		{
			app->pathfinding->ClearLastPath();
			hasPatroledLeft == true;
		}
	}
	else if (pathType == PathType::NEUTRAL && hasPatroledLeft)
	{
		patrolRight = app->map->WorldToMap(545, 330);
		app->pathfinding->CreatePath(mapPos, patrolRight);

		if (pbody->body->GetLinearVelocity().x == 0)
		{
			app->pathfinding->ClearLastPath();
			hasPatroledLeft == false;
		}
	}
	else if (pathType == PathType::AGRESSIVE)
	{
		playerDest = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
		app->pathfinding->CreatePath(mapPos, playerDest);
	}
	//Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(pathfindingTexture, pos.x, pos.y);
	}
	
	// L12: Debug pathfinding
	/*iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);*/

	//movement
	distFromPlayer.x = app->scene->player->position.x - position.x;
	distFromPlayer.y = app->scene->player->position.y - position.y;


	if (distFromPlayer.x > 0) {
		moveState = WalkingEnemyMoveState::MS_GE_RIGHT;
	}
	else if (distFromPlayer.x < 0) {
		moveState = WalkingEnemyMoveState::MS_GE_LEFT;
	}
	else if (distFromPlayer.x == 0) {
		moveState = WalkingEnemyMoveState::MS_GE_STOP;
	}

	switch (moveState)
	{
	case MS_GE_STOP:
		velocity.x = 0;
		break;
	case MS_GE_LEFT:
		//move to left
		velocity.x = -1.5f;
		break;
	case MS_GE_RIGHT:
		//move to left
		velocity.x = 1.5f;
		break;
	default:
		break;
	}
	
	//apply the vel to the enemy
	velocity.y = -GRAVITY_Y*0.7;
	pbody->body->SetLinearVelocity(velocity);
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//animation update + draw
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x-17.5, position.y-21, &rect);

	return true;
}

bool WalkingEnemy::CleanUp()
{
	return true;
}


void WalkingEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {


}

bool WalkingEnemy::LoadState(pugi::xml_node& data)
{
	/*position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();
	b2Vec2 pos(position.x, position.y);
	pbody->body->SetTransform(PIXEL_TO_METERS(pos), 0);*/

	return true;
}
bool WalkingEnemy::SaveState(pugi::xml_node& data)
{
	/*pugi::xml_node play = data.append_child("player");

	play.append_attribute("x") = position.x;
	play.append_attribute("y") = position.y;*/

	return true;
}

bool WalkingEnemy::MovePath()
{
	return true;
}