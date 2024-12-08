#include "PhysicSystem.h"
#include "OgreBullet.h" // For Use Bullet Physic Engine

Physics::Physics()
{
	collectionConfig = new btDefaultCollisionConfiguration(); // 可以看作是碰撞檢測系統的核心配置，定義如何處理不同類型的碰撞對象
	dispatcher = new btCollisionDispatcher(collectionConfig); // 每當有物理對象(ex:軟-剛體、剛體-剛體)需要進行碰撞測試時，分派器會調用適當的碰撞檢測演算法。
	overlappingPairCache = new btDbvtBroadphase(); // 廣相位演算法，負責快速篩選可能會碰撞的對象對。 動態邊界體積樹-dbvt(Dynamic Bounding Volume Tree) => 廣相位演算法能夠顯著減少進入窄相位檢測的對象數量，提高性能
	solver = new btSequentialImpulseConstraintSolver(); // 負責解析物理模擬中的約束（例如碰撞、摩擦、關節約束等）。
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collectionConfig); // 結合上述所有組件（碰撞配置、分派器、廣相位、約束求解器）來實現完整的物理模擬
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
	groundTransform.setIdentity(); // 初始化變換為單位矩陣，表示物體無任何旋轉或平移。
	groundTransform.setOrigin(btVector3(0, 0, 0)); // 將地板的中心位置設置為 (0, -50, 0)

	btScalar groundMass(0.); // 設為 0 表示地板是靜態物體（不可移動的 )
	btVector3 localGroundInertia(0, 0, 0); // 靜態物體不需要計算慣性，因此慣性向量設為 (0, 0, 0)

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