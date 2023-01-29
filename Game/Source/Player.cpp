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
#include "Debug.h"
#include "Hud.h"

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
	walkRightAnim.PushBack({ 0, 78, 33, 36 });
	walkRightAnim.PushBack({ 33, 78, 33, 36 });
	walkRightAnim.PushBack({ 66, 78, 33, 36 });
	walkRightAnim.PushBack({ 99, 78, 33, 36 });
	walkRightAnim.PushBack({ 132, 78, 33, 36 });
	walkRightAnim.PushBack({ 165, 78, 33, 36 });
	walkRightAnim.speed = 0.1f;

	// left walk
	walkLeftAnim.PushBack({ 165, 114, 33, 36 });
	walkLeftAnim.PushBack({ 132, 114, 33, 36 });
	walkLeftAnim.PushBack({ 99, 114, 33, 36 });
	walkLeftAnim.PushBack({ 66, 114, 33, 36 });
	walkLeftAnim.PushBack({ 33, 114, 33, 36 });
	walkLeftAnim.PushBack({ 0, 114, 33, 36 });
	walkLeftAnim.speed = 0.1f;

	// jump right
	jumpRightAnim.PushBack({ 0, 150, 35, 44 });
	jumpRightAnim.PushBack({ 35, 150, 35, 44 });
	jumpRightAnim.PushBack({ 70, 150, 35, 44 });
	jumpRightAnim.PushBack({ 105, 150, 35, 44 });
	jumpRightAnim.speed = 0.1f;

	// jump left
	jumpLeftAnim.PushBack({ 105, 198, 35, 44 });
	jumpLeftAnim.PushBack({ 70, 198, 35, 44 });
	jumpLeftAnim.PushBack({ 35, 198, 35, 44 });
	jumpLeftAnim.PushBack({ 0, 198, 35, 44 });
	jumpLeftAnim.speed = 0.1f;

	// die
	dieAnim.PushBack({ 0, 240, 18, 32 });
	dieAnim.PushBack({ 28, 240, 18, 32 });
	dieAnim.PushBack({ 56, 240, 18, 32 });
	dieAnim.PushBack({ 84, 240, 18, 32 });
	dieAnim.PushBack({ 110, 240, 18, 32 });
	dieAnim.PushBack({ 136, 240, 18, 32 });
	dieAnim.loop = false;
	dieAnim.speed = 0.1f;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	invincibility = 0;
	invincible = false;

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 18, bodyType::DYNAMIC);
	//pbody->body->SetFixedRotation(true);

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
	if (!app->debug->godMode)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (enableJump == true || numJumps < 2)
			{
				numJumps++;
				enableJump = false;
				hasJumped = true;
				float impulse = pbody->body->GetMass() * 5.0f;
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
			currentAnimation = &idleAnim;
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
			currentAnimation = &idleAnim;
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
	}
	else if (app->debug->godMode)
	{
		b2Vec2 desiredVel;

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
		{
			moveState = MS_UP;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		{
			moveState = MS_STOP;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			moveState = MS_LEFT;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			moveState = MS_STOP;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			moveState = MS_DOWN;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		{
			moveState = MS_STOP;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			moveState = MS_RIGHT;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			moveState = MS_STOP;
		}

		switch (moveState)
		{
		case MS_UP:		desiredVel.x = 0;	desiredVel.y = -5; break;
		case MS_LEFT:	desiredVel.x = -5;	desiredVel.y = 0; break;
		case MS_DOWN:	desiredVel.x = 0;	desiredVel.y = 5; break;
		case MS_RIGHT:	desiredVel.x = 5;	desiredVel.y = 0; break;
		case MS_STOP:	desiredVel.x = 0;	desiredVel.y = 0; break;
		}

		pbody->body->SetLinearVelocity(desiredVel);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(parameters.attribute("x").as_int()), PIXEL_TO_METERS(parameters.attribute("y").as_int())), 0);
	}

	if (app->hud->timeCount_ > 100)
		dead = true;

	if (invincible)
		invincibility++;

	LOG("INVINCIBILITY: %d", invincibility);

	if (invincibility == 100 || invincibility > 100)
		invincible = false;

	//Player or Enemy death
	if(mightKillWE)
	{
		if (posPlayerToKill.y < posWalkingEnToKill.y)
		{
			app->scene->walkingEn->Disable();
			mightKillWE = false;
		}
		else if (!invincible)
		{
			mightKillWE = false;
			app->hud->heartsCount--;
			invincible = true;
			if (app->hud->heartsCount == 0 && app->scene->checkpoint->isPicked)
			{
				app->hud->heartsCount = 1;
				pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(app->scene->checkpoint->position.x), PIXEL_TO_METERS(app->scene->checkpoint->position.x)), 0);
			}
			else if (app->hud->heartsCount == 0 && !app->scene->checkpoint->isPicked)
				dead = true;
			
		}
	}
	if (mightKillFE)
	{
		if (posPlayerToKill.y < posFlyingEnToKill.y)
		{
			app->scene->flyingEn->Disable();
			mightKillFE = false;
		}
		else if(!invincible)
		{
			mightKillFE = false;
			app->hud->heartsCount--;
			invincible = true;
			if (app->hud->heartsCount == 0 && app->scene->checkpoint->isPicked)
			{
				app->hud->heartsCount = 1;
				pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(app->scene->checkpoint->position.x), PIXEL_TO_METERS(app->scene->checkpoint->position.x)), 0);
			}
			else if (app->hud->heartsCount == 0 && !app->scene->checkpoint->isPicked)
				dead = true;
		}
	}

	if (dead)
	{
		currentAnimation = &dieAnim;
		if (dieAnim.HasFinished() == true)
		{
			app->fade->FadeToBlack((Module*)app->scene, (Module*)app->lose, 15);
			app->entityManager->Disable();
		}
	}

	if (waterDeath)
	{
		if (app->scene->checkpoint->isPicked)
		{
			waterDeath = false;
			app->hud->heartsCount = 1;
			pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(app->scene->checkpoint->position.x), PIXEL_TO_METERS(app->scene->checkpoint->position.x)), 0);
		}
		else
		{
			currentAnimation = &dieAnim;
			if (dieAnim.HasFinished() == true)
			{
				app->fade->FadeToBlack((Module*)app->scene, (Module*)app->lose, 15);
				app->entityManager->Disable();
			}
		}
	}

	if (win)
	{
		app->fade->FadeToBlack((Module*)app->scene, (Module*)app->winScreen, 0);
		app->entityManager->Disable();
	}
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	
	currentAnimation->Update();

	app->render->DrawTexture(texture, position.x, position.y, &rect);

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
			app->hud->coinsCount++;
			break;
		case ColliderType::HEART:
			LOG("Collision ITEM");
			//app->audio->PlayFx(pickCoinFxId);
			if(app->hud->heartsCount < 4)
				app->hud->heartsCount++;
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			enableJump = true;
			hasJumped = false;
			currentAnimation = &idleAnim;
			numJumps = 0;
			break;
		case ColliderType::WALL:
			LOG("Collision PLATFORM");
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			if(!app->debug->godMode)
				waterDeath = true;
			break;
		case ColliderType::WIN:
			LOG("Collision WIN");
			win = true;
			break;
		case ColliderType::WALKING_ENEMY:
			LOG("Walking enemy kills you");
			mightKillWE = true;
			posPlayerToKill = iPoint(app->scene->player->position.x, app->scene->player->position.y);
			posWalkingEnToKill = iPoint(app->scene->walkingEn->position.x, app->scene->walkingEn->position.y);
			break;
		case ColliderType::FLYING_ENEMY:
			LOG("Flying enemy kills you");
			mightKillFE = true;
			posPlayerToKill = iPoint(app->scene->player->position.x, app->scene->player->position.y);
			posFlyingEnToKill = iPoint(app->scene->flyingEn->position.x, app->scene->flyingEn->position.y);
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
