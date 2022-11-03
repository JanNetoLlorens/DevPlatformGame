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
	

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

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
			float impulse = pbody->body->GetMass() * 6.0f;
			pbody->body->ApplyLinearImpulse(b2Vec2(0, -impulse), pbody->body->GetWorldCenter(), true);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}
		
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		moveState = MS_LEFT;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		moveState = MS_STOP;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		moveState = MS_RIGHT;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		moveState = MS_STOP;
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

	app->render->DrawTexture(texture, position.x, position.y);

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
			numJumps = 0;
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			app->audio->PlayFx(pickCoinFxId);
			deathCollider = true;
			break;
		case ColliderType::WIN:
			LOG("Collision WIN");
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
	


}
