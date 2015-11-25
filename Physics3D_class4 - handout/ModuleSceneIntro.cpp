#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(0.0f, 5.0f, .00f));
	App->camera->LookAt(vec3(0, 0, 0));

	// TODO 2: Chain few N spheres together to form a snake
	s_snake[0].SetPos(0, 0, 0);
	s_snake[1].SetPos(1, 2, 0);
	s_snake[2].SetPos(2, 3, 0);
	s_snake[3].SetPos(3, 4, 0);
	s_snake[4].SetPos(4, 5, 0);
	pb_snake[0] = App->physics->AddBody(s_snake[0]);
	pb_snake[1] = App->physics->AddBody(s_snake[1]);
	pb_snake[2] = App->physics->AddBody(s_snake[2]);
	pb_snake[3] = App->physics->AddBody(s_snake[3]);
	pb_snake[4] = App->physics->AddBody(s_snake[4]);

	btVector3 pivotA{ 10, 4, 3 };
	btVector3 pivotB{ 3, 4, 3 };
	App->physics->Point2Constraint(*pb_snake[0], *pb_snake[1], pivotA, pivotB);
	App->physics->Point2Constraint(*pb_snake[1], *pb_snake[2], pivotA, pivotB);
	App->physics->Point2Constraint(*pb_snake[2], *pb_snake[3], pivotA, pivotB);
	App->physics->Point2Constraint(*pb_snake[3], *pb_snake[4], pivotA, pivotB);
	// TODO 4: Chain few N spheres together to form a a bike's sphere
	// Be sure to put the right axis

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	/* Uncomment when ready
	for(int i = 0; i < MAX_SNAKE; ++i)
	{
		pb_snake[i]->GetTransform(&(s_snake[i].transform));
		s_snake[i].Render();
	}

	for(int i = 0; i < MAX_SNAKE; ++i)
	{
		pb_snake2[i]->GetTransform(&(s_snake2[i].transform));
		s_snake2[i].Render();
	}*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

