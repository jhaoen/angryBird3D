#ifndef __PhysicSystem_h_
#define __PhysicSystem_h_

#include <Ogre.h>
#include "OgreBullet.h" // For Use Bullet Physic Engine
#include <vector>
#include <map>

using namespace std;
using namespace Ogre;

class Physics
{
	btDefaultCollisionConfiguration* collectionConfig;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	vector<btCollisionShape*> collisionShapes;
	map<string, btRigidBody*> pyhsicsAccessors;
	SceneManager* mSceneMgr;
public:
	void initSystem(Ogre::SceneManager* SourceMgr);
	void createScene();
	void stepSimulation(btScalar step);
	Physics();
	~Physics();
};

#endif