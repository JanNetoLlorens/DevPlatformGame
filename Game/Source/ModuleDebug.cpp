#include "ModuleDebug.h"

#include "App.h"
#include "Input.h"
#include "Physics.h"
#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

#include <chrono>
using namespace std::chrono;

#include <string>

ModuleDebug::ModuleDebug(bool startEnabled) : Module(startEnabled)
{
}

ModuleDebug::~ModuleDebug() {}

bool ModuleDebug::Start()
{
	drawDebug = false;
	return true;
}

bool ModuleDebug::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		drawPhysics = !drawPhysics;


	
	

	
	return true;
}

bool ModuleDebug::PostUpdate()
{
	if (drawDebug)
		DrawDebug();

	if (drawPhysics)
		DrawPhysics();

	return true;
}

void ModuleDebug::DrawDebug()
{


	//if (variables)
	//{
	//	//Ball x, y
	//	std::string string = std::string("BALL.X = ") + std::to_string(App->scene_intro->ball->position.x);
	//	App->fonts->BlitText(debugX + 16, debugY + 220, fontId, string.c_str());

	//	string = std::string("BALL.Y = ") + std::to_string(App->scene_intro->ball->position.y);
	//	App->fonts->BlitText(debugX + 16, debugY + 240, fontId, string.c_str());

	//	//Spring force
	//	if (App->scene_intro->springForce == 420)
	//		string = std::string("SPRING.F = ") + std::to_string(App->scene_intro->springForce) + "  X)";
	//	else
	//		string = std::string("SPRING.F = ") + std::to_string(App->scene_intro->springForce);
	//	App->fonts->BlitText(debugX + 16, debugY + 260, fontId, string.c_str());
	//}	
}

void ModuleDebug::DrawPhysics()
{
	
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = app->physics->world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				app->render->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				// test if the current body contains mouse position
				b2Vec2 p = { PIXEL_TO_METERS(app->input->GetMouseX()), PIXEL_TO_METERS(app->input->GetMouseY()) };
				if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
				{

					// If a body was selected we will attach a mouse joint to it
					// so we can pull it around

					// TODO 2: If a body was selected, create a mouse joint
					// using mouse_joint class property

					// NOTE: you do TODO2 here or also in the original handout's location. 
					// It doesn't matter unless you are triggering several objects at once;
					// I leave it to you to add safety checks to avoid re-defining several mouse joints.

					// The variable "b2Body* mouse_body;" is defined in the header ModulePhysics.h 
					// We need to keep this body throughout several game frames; you cannot define it as a local variable here. 
					app->physics->mouse_body = b;

					// Get current mouse position
					b2Vec2 mousePosition;
					mousePosition.x = p.x;
					mousePosition.y = p.y;

					// Define new mouse joint
					b2MouseJointDef def;
					def.bodyA = app->physics->ground; // First body must be a static ground
					def.bodyB = app->physics->mouse_body; // Second body will be the body to attach to the mouse
					def.target = mousePosition; // The second body will be pulled towards this location
					def.dampingRatio = 0.5f; // Play with this value
					def.frequencyHz = 2.0f; // Play with this value
					def.maxForce = 200.0f * app->physics->mouse_body->GetMass(); // Play with this value

					// Add the new mouse joint into the World
					app->physics->mouse_joint = (b2MouseJoint*)app->physics->world->CreateJoint(&def);
				}
			}
		}
	}



	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (app->physics->mouse_body != nullptr && app->physics->mouse_joint != nullptr)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			// Get new mouse position and re-target mouse_joint there
			b2Vec2 mousePosition;
			mousePosition.x = PIXEL_TO_METERS(app->input->GetMouseX());
			mousePosition.y = PIXEL_TO_METERS(app->input->GetMouseY());
			app->physics->mouse_joint->SetTarget(mousePosition);

			// Draw a red line between both anchor points
			app->render->DrawLine(METERS_TO_PIXELS(app->physics->mouse_body->GetPosition().x), METERS_TO_PIXELS(app->physics->mouse_body->GetPosition().y), app->input->GetMouseX(), app->input->GetMouseY(), 255, 0, 0);
		}
	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if (app->physics->mouse_body != nullptr && app->physics->mouse_joint != nullptr)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			// Tell Box2D to destroy the mouse_joint
			app->physics->world->DestroyJoint(app->physics->mouse_joint);

			// DO NOT FORGET THIS! We need it for the "if (mouse_body != nullptr && mouse_joint != nullptr)"
			app->physics->mouse_joint = nullptr;
			app->physics->mouse_body = nullptr;
		}
	}
}