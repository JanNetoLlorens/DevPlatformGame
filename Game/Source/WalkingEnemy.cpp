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



	return true;
}

bool WalkingEnemy::Update()
{


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