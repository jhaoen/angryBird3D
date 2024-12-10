#include "PhysicSystem.h"
#include "OgreBullet.h" // For Use Bullet Physic Engine
#include <iostream>

Physics::Physics()
{
	collectionConfig = new btDefaultCollisionConfiguration(); // provide collision basic config include collision detect algorithm
	dispatcher = new btCollisionDispatcher(collectionConfig); // use config to deal with diff type collision
	overlappingPairCache = new btDbvtBroadphase(); // increase performence by use （Dynamic Bounding Volume Tree, DBVT） to detect potential collision
	solver = new btSequentialImpulseConstraintSolver(); // to simullate collision react or Friction
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collectionConfig); // build the phyic world depands on those component
}

Physics::~Physics() {
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collectionConfig;
	for (btCollisionShape* shape : collisionShapes) {
		delete shape;
	}
	collisionShapes.clear();
}

void Physics::initSystem(SceneManager* SourceMgr)
{
	mSceneMgr = SourceMgr;
	
}

void Physics::createScene()
{
	btTransform groundTransform;
	groundTransform.setIdentity(); 
	groundTransform.setOrigin(btVector3(0, 4, 0)); 

	btScalar groundMass(0.); 
	btVector3 localGroundInertia(0, 0, 0); 

	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(750.), btScalar(1.), btScalar(750.)));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	collisionShapes.push_back(groundShape);

	groundShape->calculateLocalInertia(groundMass, localGroundInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
	btRigidBody* groundBody = new btRigidBody(groundRBInfo);
	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(groundBody);

}

void Physics::stepSimulation(btScalar step)
{
	dynamicsWorld->stepSimulation(step);
	for (int i = 0; i < collisionShapes.size(); i++) 
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState()) 
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			void* userPointer = body->getUserPointer();
			if (userPointer) 
			{
				btQuaternion orientation = trans.getRotation();
				Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(userPointer);
				sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
				sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
			}
		}
	}
}
void Physics::addEntity(Entity* entity, SceneNode* node, const btVector3& shapeSize, btScalar mass, const btVector3& startPosition, const btQuaternion& startRotation)
{
	// Create a collision shape for the entity  
	btCollisionShape* shape = new btBoxShape(shapeSize);
	collisionShapes.push_back(shape);

	// Set the initial transform for the rigid body  
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(startPosition);
	startTransform.setRotation(startRotation);

	// Calculate the local inertia for the rigid body  
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.0f) {
		shape->calculateLocalInertia(mass, localInertia);
	}

	// Create the motion state for the rigid body  
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

	// Create the rigid body construction info  
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);

	// Create the rigid body  
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set restitution (bounciness) and friction (optional)  
	body->setRestitution(0.5f); // Example: 50% bounciness  
	body->setFriction(0.5f);    // Example: 50% friction  

	// Associate the rigid body with the scene node  
	body->setUserPointer(node);

	// Add the rigid body to the dynamics world  
	dynamicsWorld->addRigidBody(body);

	// Store the rigid body in the map for future access  
	physicsAccessors[entity->getName()] = body;

	/*for (const auto& T : physicsAccessors)
		cout << T.first << '\n';*/
}


bool Physics::giveObjectVelocity(string objName, btVector3 velocity)
{
	if (physicsAccessors.find(objName) == physicsAccessors.end())
		return false;
	btRigidBody* body = physicsAccessors[objName];
	/*btVector3 direction(0, 0.0, 1);
	direction.normalize();
	float speed = 20.0;*/
	body->setLinearVelocity(velocity);
	body->activate();
	//float impulseStrength = 50.0;
	//btVector3 impulse = direction * impulseStrength;
	body->applyCentralImpulse(velocity);
	body->activate();
	return true;
}