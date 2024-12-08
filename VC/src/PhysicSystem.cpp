#include "PhysicSystem.h"
#include "OgreBullet.h" // For Use Bullet Physic Engine

Physics::Physics()
{
	collectionConfig = new btDefaultCollisionConfiguration(); // �i�H�ݧ@�O�I���˴��t�Ϊ��֤߰t�m�A�w�q�p��B�z���P�������I����H
	dispatcher = new btCollisionDispatcher(collectionConfig); // �C������z��H(ex:�n-����B����-����)�ݭn�i��I�����ծɡA�������|�եξA����I���˴��t��k�C
	overlappingPairCache = new btDbvtBroadphase(); // �s�ۦ�t��k�A�t�d�ֳt�z��i��|�I������H��C �ʺA�����n��-dbvt(Dynamic Bounding Volume Tree) => �s�ۦ�t��k�����۴�ֶi�J���ۦ��˴�����H�ƶq�A�����ʯ�
	solver = new btSequentialImpulseConstraintSolver(); // �t�d�ѪR���z������������]�Ҧp�I���B�����B���`������^�C
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collectionConfig); // ���X�W�z�Ҧ��ե�]�I���t�m�B�������B�s�ۦ�B����D�Ѿ��^�ӹ�{���㪺���z����
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
	groundTransform.setOrigin(btVector3(0, 0, 0)); 

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
	pyhsicsAccessors[entity->getName()] = body;
}