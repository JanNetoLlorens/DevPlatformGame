#include "Player.h"
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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	// idle
	idleAnim.PushBack({ 0, 0, 29, 39 });
	//idleAnim.PushBack({ 29, 0, 29, 39 });
	//idleAnim.PushBack({ 58, 0, 29, 39 });
	idleAnim.PushBack({ 87, 0, 29, 39 });
//	idleAnim.PushBack({ 116, 0, 29, 39 });
	idleAnim.PushBack({ 145, 0, 29, 39 });
	idleAnim.speed = 0.1f;
	
	// right walk
	walkRightAnim.PushBack({ 0, 39, 33, 36 });
	walkRightAnim.PushBack({ 33, 39, 33, 36 });
	walkRightAnim.PushBack({ 66, 39, 33, 36 });
	walkRightAnim.PushBack({ 99, 39, 33, 36 });
	walkRightAnim.PushBack({ 132, 39, 33, 36 });
	walkRightAnim.PushBack({ 165, 39, 33, 36 });
	walkRightAnim.speed = 0.1f;

	// left walk
	walkLeftAnim.PushBack({ 0, 78, 33, 36 });
	walkLeftAnim.PushBack({ 33, 78, 33, 36 });
	walkLeftAnim.PushBack({ 66, 78, 33, 36 });
	walkLeftAnim.PushBack({ 99, 78, 33, 36 });
	walkLeftAnim.PushBack({ 132, 78, 33, 36 });
	walkLeftAnim.PushBack({ 165, 78, 33, 36 });
	walkLeftAnim.speed = 0.1f;

	// jump right
	jumpRightAnim.PushBack({ 0, 111, 35, 44 });
	jumpRightAnim.PushBack({ 35, 111, 35, 44 });
	jumpRightAnim.PushBack({ 70, 111, 35, 44 });
	jumpRightAnim.PushBack({ 105, 111, 35, 44 });
	jumpRightAnim.speed = 0.1f;

	// jump left
	jumpLeftAnim.PushBack({ 105, 155, 35, 44 });
	jumpLeftAnim.PushBack({ 70, 155, 35, 44 });
	jumpLeftAnim.PushBack({ 35, 155, 35, 44 });
	jumpLeftAnim.PushBack({ 0, 155, 35, 44 });
	jumpLeftAnim.speed = 0.1f;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	dead = false;

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update()
{

	// L07 DONE 5: Add physics to the player - updated player position using physics

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) 
	{
		if (enableJump == true || numJumps < 2 )
		{
			numJumps++;
			enableJump = false;
			hasJumped = true;
			float impulse = pbody->body->GetMass() * 6.0f;
			pbody->body->ApplyLinearImpulse(b2Vec2(0, -impulse), pbody->body->GetWorldCenter(), true);
		}
	}

	if (hasJumped && movingRight)
		currentAnimation = &jumpRightAnim;
	else if (hasJumped && !movingRight)
		currentAnimation = &jumpLeftAnim;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}
		
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		moveState = MS_LEFT;
		if (!hasJumped)
		{
			currentAnimation = &walkLeftAnim;
		}
		movingRight = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		moveState = MS_STOP;
		//currentAnimation = &idleAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		moveState = MS_RIGHT;
		if (!hasJumped)
		{
			currentAnimation = &walkRightAnim;
		}
		movingRight = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		moveState = MS_STOP;
		//currentAnimation = &idleAnim;
	}

	b2Vec2 vel = pbody->body->GetLinearVelocity();
	float desiredVel = 0;

	switch (moveState)
	{
	case MS_LEFT:  desiredVel = -5; break;
	case MS_STOP:  desiredVel = 0; break;
	case MS_RIGHT: desiredVel = 5; break;
	}

	//Set the velocity of the pbody of the player
	//pbody->body->SetLinearVelocity(vel);
	float velChange = desiredVel - vel.x;
	float impulse = pbody->body->GetMass() * velChange; //disregard time factor
	pbody->body->ApplyLinearImpulse(b2Vec2(impulse, 0), pbody->body->GetWorldCenter(), true);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(parameters.attribute("x").as_int()), PIXEL_TO_METERS(parameters.attribute("y").as_int())), 0);
	}

	if (!dead)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}
	else
	{
		app->fade->FadeToBlack((Module*)app->scene, (Module*)app->endingScreen, 0);
		app->entityManager->Disable();
		app->map->CleanUp();
	}
	
	currentAnimation->Update();

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			enableJump = true;
			hasJumped = false;
			currentAnimation = &idleAnim;
			numJumps = 0;
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			dead = true;
			break;
		case ColliderType::WIN:
			LOG("Collision WIN");
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
	
	
}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();
	b2Vec2 pos(position.x, position.y);
	pbody->body->SetTransform(PIXEL_TO_METERS(pos), 0);

	return true;
}
bool Player::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");

	play.append_attribute("x") = position.x;
	play.append_attribute("y") = position.y;

	return true;
}
