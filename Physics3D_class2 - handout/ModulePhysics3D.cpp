#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"
//#include "glmath.h"

#define RELEASE (toDelete) \
	{							 \
		if (toDelete != NULL) 			\
		{ 									\
			delete toDelete;            \
			toDelete = NULL;            \
		}                                \
	}
// TODO 1: ...and the 3 libraries based on how we compile (Debug or Release)
// use the _DEBUG preprocessor define
#ifdef _DEBUG
#pragma comment (lib, "Bullet/bin/BulletDynamics_vs2010_debug.lib")   
#pragma comment (lib, "Bullet/bin/BulletCollision_vs2010_debug.lib")   
#pragma comment (lib, "Bullet/bin/LinearMath_vs2010_debug.lib")   
#else
#pragma comment (lib, "Bullet/bin/BulletDynamics_vs2010.lib")   
#pragma comment (lib, "Bullet/bin/BulletCollision_vs2010.lib")   
#pragma comment (lib, "Bullet/bin/LinearMath_vs2010.lib")   
#endif

#define GRAVITY {0.0f,-10.0f,0.0f}

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug_draw = NULL;
	debug = true;

	// TODO 2: Create collision configuration, dispacher
	// broad _phase and solver
	collisionConfiguration = new btDefaultCollisionConfiguration;
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	pairCache = new btDbvtBroadphase;
	constraintSolver = new btSequentialImpulseConstraintSolver;

	
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	//RELEASE!
	delete debug_draw;
	// TODO 2: and destroy them!
	delete dispatcher;
	dispatcher = NULL;
	delete pairCache;
	pairCache = NULL;
	delete constraintSolver;
	constraintSolver = NULL;
	delete collisionConfiguration;
	collisionConfiguration = NULL;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	// TODO 3: Create the world and set default gravity
	// Have gravity defined in a macro!

	world = new btDiscreteDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration);
	//Should be changed
	btVector3 gravity (GRAVITY);
	world->setGravity(gravity);
	debug_draw = new DebugDrawer();
	world->setDebugDrawer(debug_draw);


	{
		btCollisionShape* colShape = new btBoxShape(btVector3(100.0f, 1.0f, 100.0f));

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);
		btRigidBody* body = new btRigidBody(rbInfo);		world->addRigidBody(body);
		// TODO 5: Create a big rectangle as ground
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	// TODO 4: step the world
	world->stepSimulation(dt);
	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();
		
		if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			//Fer matriu nostra a camera, passar-li a btTransform->OpenGL
			mat4x4 identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, App->camera->Position.x, App->camera->Position.y, App->camera->Position.z, 1);



			btCollisionShape* colShape = new btSphereShape(2.0f);
			btTransform transform;
			transform.setFromOpenGLMatrix(identity.M);
			btDefaultMotionState* myMotionState = new btDefaultMotionState();
			myMotionState->setWorldTransform(transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, myMotionState, colShape);
			btRigidBody* body = new btRigidBody(rbInfo);			world->addRigidBody(body);
			// TODO 6: Create a Solid Sphere when pressing 1 on camera position
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// TODO 3: ... and destroy the world here!
	delete world;
	return true;
}

// =============================================

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
