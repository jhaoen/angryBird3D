#include "PhysicSystem.h"

Physics::Physics()
{

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
	collectionConfig = new btDefaultCollisionConfiguration(); // 可以看作是碰撞檢測系統的核心配置，定義如何處理不同類型的碰撞對象
	dispatcher = new btCollisionDispatcher(collectionConfig); // 每當有物理對象(ex:軟-剛體、剛體-剛體)需要進行碰撞測試時，分派器會調用適當的碰撞檢測演算法。
	overlappingPairCache = new btDbvtBroadphase(); // 廣相位演算法，負責快速篩選可能會碰撞的對象對。 動態邊界體積樹-dbvt(Dynamic Bounding Volume Tree) => 廣相位演算法能夠顯著減少進入窄相位檢測的對象數量，提高性能
	solver = new btSequentialImpulseConstraintSolver(); // 負責解析物理模擬中的約束（例如碰撞、摩擦、關節約束等）。
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collectionConfig); // 結合上述所有組件（碰撞配置、分派器、廣相位、約束求解器）來實現完整的物理模擬
}

void Physics::createScene()
{
	Plane plane(Vector3::UNIT_Y, 0);
	MeshPtr planePtr = MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Entity* entGround = mSceneMgr->createEntity("GroundEntity","ground");
	SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("groundNode");
	groundNode->attachObject(entGround);

	btTransform groundTransform;
	groundTransform.setIdentity(); // 初始化變換為單位矩陣，表示物體無任何旋轉或平移。
	groundTransform.setOrigin(btVector3(0, -50, 0)); // 將地板的中心位置設置為 (0, -50, 0)

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
}