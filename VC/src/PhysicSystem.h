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
	SceneManager* mSceneMgr;
public:
	map<string, btRigidBody*> physicsAccessors;
	void initSystem(Ogre::SceneManager* SourceMgr);
	void createScene();
	void stepSimulation(btScalar step);
	void addEntity( // add ogre entity to bullet physical world
		Entity* entity, 
		SceneNode* node,
		const btVector3& shapeSize,
		btScalar mass, 
		const btVector3& startPosition, 
		const btQuaternion& startRotation
	);
	bool giveObjectVelocity(string objName, btVector3 velocity);
	Physics();
	~Physics();
};

#endif