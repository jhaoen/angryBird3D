#include "PhysicSystem.h"
#include "OgreBullet.h" // For Use Bullet Physic Engine

Physics::Physics()
{
	collectionConfig = new btDefaultCollisionConfiguration(); // �i�H�ݧ@�O�I���˴��t�Ϊ��֤߰t�m�A�w�q�p��B�z���P�������I����H
	dispatcher = new btCollisionDispatcher(collectionConfig); // �C�����z��H(ex:�n-����B����-����)�ݭn�i��I�����ծɡA�������|�եξA���I���˴��t��k�C
	overlappingPairCache = new btDbvtBroadphase(); // �s�ۦ�t��k�A�t�d�ֳt�z��i��|�I������H��C �ʺA�����n��-dbvt(Dynamic Bounding Volume Tree) => �s�ۦ�t��k�����۴�ֶi�J���ۦ��˴�����H�ƶq�A�����ʯ�
	solver = new btSequentialImpulseConstraintSolver(); // �t�d�ѪR���z�������������]�Ҧp�I���B�����B���`�������^�C
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collectionConfig); // ���X�W�z�Ҧ��ե�]�I���t�m�B�������B�s�ۦ�B�����D�Ѿ��^�ӹ�{���㪺���z����
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
	Plane plane = Plane(Vector3::UNIT_Y, 0);
	MeshPtr planePtr = MeshManager::getSingleton().createPlane(
		"ground",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		1000, 1000, 1, 1,
		true,
		1, 10, 10,
		Vector3::UNIT_Z);
	Entity* entGround = mSceneMgr->createEntity("GroundEntity","ground");
	SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("groundNode");
	groundNode->attachObject(entGround);
	entGround->setMaterialName("GrassFloor");
	entGround->setCastShadows(false);

	btTransform groundTransform;
	groundTransform.setIdentity(); // ��l���ܴ������x�}�A��ܪ���L�������Υ����C
	groundTransform.setOrigin(btVector3(0, 0, 0)); // �N�a�O�����ߦ�m�]�m�� (0, -50, 0)

	btScalar groundMass(0.); // �]�� 0 ��ܦa�O�O�R�A����]���i���ʪ� )
	btVector3 localGroundInertia(0, 0, 0); // �R�A���餣�ݭn�p��D�ʡA�]���D�ʦV�q�]�� (0, 0, 0)

	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(750.), btScalar(1.), btScalar(750.)));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	collisionShapes.push_back(groundShape);

	groundShape->calculateLocalInertia(groundMass, localGroundInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
	btRigidBody* groundBody = new btRigidBody(groundRBInfo);
	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(groundBody);

	MeshPtr birdMesh = MeshManager::getSingleton().load("angry_bird.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).staticCast<Mesh>();
	Entity* birdPhy = mSceneMgr->createEntity(birdMesh);
	SceneNode* birdPhyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	birdPhyNode->setScale(3, 3, 3);
	//birdPhyNode->yaw(Degree(180));
	birdPhyNode->attachObject(birdPhy);

	//create the new shape, and tell the physics that is a Box
	btCollisionShape* birdRigidShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	collisionShapes.push_back(birdRigidShape);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1.0f, 0.0f, 0.0f, 0));

	btScalar mass = 0.1f;
	btVector3 localInertia(0, 0, 0);

	startTransform.setOrigin(btVector3(10,20,0));
	birdRigidShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, birdRigidShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setRestitution(1);
	body->setUserPointer(birdPhyNode);

	dynamicsWorld->addRigidBody(body);
	pyhsicsAccessors["bird"] = body;
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