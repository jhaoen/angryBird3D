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
	collectionConfig = new btDefaultCollisionConfiguration(); // �i�H�ݧ@�O�I���˴��t�Ϊ��֤߰t�m�A�w�q�p��B�z���P�������I����H
	dispatcher = new btCollisionDispatcher(collectionConfig); // �C�����z��H(ex:�n-����B����-����)�ݭn�i��I�����ծɡA�������|�եξA���I���˴��t��k�C
	overlappingPairCache = new btDbvtBroadphase(); // �s�ۦ�t��k�A�t�d�ֳt�z��i��|�I������H��C �ʺA�����n��-dbvt(Dynamic Bounding Volume Tree) => �s�ۦ�t��k�����۴�ֶi�J���ۦ��˴�����H�ƶq�A�����ʯ�
	solver = new btSequentialImpulseConstraintSolver(); // �t�d�ѪR���z�������������]�Ҧp�I���B�����B���`�������^�C
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collectionConfig); // ���X�W�z�Ҧ��ե�]�I���t�m�B�������B�s�ۦ�B�����D�Ѿ��^�ӹ�{���㪺���z����
}

void Physics::createScene()
{
	Plane plane(Vector3::UNIT_Y, 0);
	MeshPtr planePtr = MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Entity* entGround = mSceneMgr->createEntity("GroundEntity","ground");
	SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("groundNode");
	groundNode->attachObject(entGround);

	btTransform groundTransform;
	groundTransform.setIdentity(); // ��l���ܴ������x�}�A��ܪ���L�������Υ����C
	groundTransform.setOrigin(btVector3(0, -50, 0)); // �N�a�O�����ߦ�m�]�m�� (0, -50, 0)

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
}