#include "FlyingEnemy.h"

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

FlyingEnemy::FlyingEnemy() : Enemy(EntityType::FLYING_ENEMY)
{
	name.Create("FlyingEnemy");
}

FlyingEnemy::~FlyingEnemy() {

}

bool FlyingEnemy::Awake() {


	return true;
}

bool FlyingEnemy::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	pathfindingTexturePath = parameters.attribute("pathfinding_texturepath").as_string();


	// idle
	idleAnim.PushBack({ 0, 0, 44, 42 });
	idleAnim.PushBack({ 44, 0, 44, 42 });
	idleAnim.PushBack({ 88, 0, 44, 42 });
	idleAnim.PushBack({ 132, 0, 44, 42 });
	idleAnim.PushBack({ 176, 0, 44, 42 });
	idleAnim.speed = 0.1f;

	texture = app->tex->Load(texturePath);
	pathfindingTexture = app->tex->Load(pathfindingTexturePath);

	currentAnimation = &idleAnim;

	pbody = app->physics->CreateRectangle(position.x, position.y, 44, 42, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::FLYING_ENEMY;

	velocity.x = 0; velocity.y = 0;

	pathType = PathType::AGRESSIVE;

	return true;
}

bool FlyingEnemy::Update()
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
		moveStateX = FlyingEnemyMoveStateX::MS_FE_RIGHT;
	}
	else if (distFromPlayer.x < 0) {
		moveStateX = FlyingEnemyMoveStateX::MS_FE_LEFT;
	}
	else if (distFromPlayer.x == 0) {
		moveStateX = FlyingEnemyMoveStateX::MS_FE_STOPX;
	}

	if (distFromPlayer.y > 0) {
		moveStateY = FlyingEnemyMoveStateY::MS_FE_UP;
	}
	else if (distFromPlayer.y < 0) {
		moveStateY = FlyingEnemyMoveStateY::MS_FE_DOWN;
	}
	else if (distFromPlayer.y == 0) {
		moveStateY = FlyingEnemyMoveStateY::MS_FE_STOPY;
	}

	switch (moveStateX)
	{
	case MS_FE_STOPX:
		velocity.x = 0;
		break;
	case MS_FE_LEFT:
		velocity.x = -1.5f;
		break;
	case MS_FE_RIGHT:
		velocity.x = 1.5f;
		break;
	default:
		break;
	}

	switch (moveStateY)
	{
	case MS_FE_STOPY:
		velocity.y = 0;
		break;
	case MS_FE_UP:
		velocity.y = +1.5f;
		break;
	case MS_FE_DOWN:
		velocity.y = -1.5f;
		break;
	default:
		break;
	}

	//apply the vel to the enemy
	//velocity.y = -GRAVITY_Y * 0.7;
	pbody->body->SetLinearVelocity(velocity);
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//animation update + draw
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y - 4, &rect);

	return true;
}

bool FlyingEnemy::CleanUp()
{
	return true;
}


void FlyingEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {


}

bool FlyingEnemy::LoadState(pugi::xml_node& data)
{
	/*position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();
	b2Vec2 pos(position.x, position.y);
	pbody->body->SetTransform(PIXEL_TO_METERS(pos), 0);*/

	return true;
}
bool FlyingEnemy::SaveState(pugi::xml_node& data)
{
	/*pugi::xml_node play = data.append_child("player");

	play.append_attribute("x") = position.x;
	play.append_attribute("y") = position.y;*/

	return true;
}

bool FlyingEnemy::MovePath()
{
	return true;
}