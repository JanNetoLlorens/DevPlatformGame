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



	return true;
}

bool FlyingEnemy::Update()
{


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