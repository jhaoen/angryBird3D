/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/


Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

#include "TutorialApplication.h"
#include <btBulletDynamicsCommon.h>
#include <iostream>  

//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication():angryBirdNode(nullptr), angryBird(nullptr),pigNode(nullptr),
pig(nullptr),slingShotNode(nullptr),slingShot(nullptr),physicSysyem(Physics())
{
    
}


//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

Vector3 getEntityCenter(Entity* entity)
{
    AxisAlignedBox boundingBox = entity->getWorldBoundingBox();
    return boundingBox.getCenter();
}
Real getEntityHeight(Entity* entity)
{
    AxisAlignedBox boundingBox = entity->getWorldBoundingBox();

    Vector3 min = boundingBox.getMinimum(); // min point
    Vector3 max = boundingBox.getMaximum(); // max point
    return max.y - min.y;
}
Real getEntityWidth(Entity* entity)
{
    AxisAlignedBox boundingBox = entity->getWorldBoundingBox();

    Vector3 min = boundingBox.getMinimum(); // min point
    Vector3 max = boundingBox.getMaximum(); // max point
    return max.x - min.x;
}
Real getEntityDepth(Entity* entity)
{
    AxisAlignedBox boundingBox = entity->getWorldBoundingBox();

    Vector3 min = boundingBox.getMinimum(); // min point
    Vector3 max = boundingBox.getMaximum(); // max point
    return max.z - min.z;
}

void placeTop(Entity* entityA, Entity* entityB, SceneNode* nodeA, SceneNode* nodeB) {

    /*
    this function is to place entityB on entityA (in ogre and physical world)
    */
    Real heightA = getEntityHeight(entityA);
    Real heightB = getEntityHeight(entityB);

    Vector3 positionA = nodeA->getPosition();

    Vector3 newPositionB = Vector3(
        positionA.x,                      
        positionA.y + heightA + heightB, 
        positionA.z                
    );

    nodeB->setPosition(newPositionB);
}


//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{   

    physicSysyem.initSystem(mSceneMgr);
    physicSysyem.createScene();

   // create floor
    Plane plane = Plane(Vector3::UNIT_Y, 0);
    MeshPtr planePtr = MeshManager::getSingleton().createPlane(
        "ground",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane,
        1500, 1500, 20, 20,
        true, 1, 5, 5, 
        Ogre::Vector3::UNIT_Z);
    Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("groundNode");
    groundNode->attachObject(entGround);
    entGround->setMaterialName("GrassFloor");
    entGround->setCastShadows(false);

 
    // create sky plane
    mSceneMgr->setSkyBox(true, "SkyBox");

    // create entities
    angryBird = mSceneMgr->createEntity("angryBird", "angry_bird.mesh");
    slingShot = mSceneMgr->createEntity("slingShot", "slingshot.mesh");
    pig = mSceneMgr->createEntity("pig", "pig.mesh");
    woodbox = mSceneMgr->createEntity("woodbox", "wood_box.mesh");

    // create scene nodes
    angryBirdNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("angryBirdNode");
    slingShotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("slingShotNode");
    pigNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("pigNode");
    woodBoxNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("woodBoxNode");

    // set entities initial position
    Vector3 birdInitPos(-30, getEntityHeight(angryBird), 0);
    Vector3 slingshotInitPos(0, getEntityHeight(slingShot), 0);
    Vector3 pigInitPos(-20, getEntityHeight(pig), -100);
    Vector3 woodboxsInitPos(0, getEntityHeight(angryBird), -100);

    // set attributes
    angryBirdNode->setScale(3, 3, 3);
    angryBirdNode->setPosition(birdInitPos);
    angryBirdNode->yaw(Degree(180));
    btVector3 birdShapeSize(getEntityWidth(angryBird), getEntityHeight(angryBird), getEntityDepth(angryBird));
    btScalar birdMass = 5.f;
    physicSysyem.addEntity(angryBird, angryBirdNode, birdShapeSize, birdMass);
    physicSysyem.syncOgre2Bullet(angryBird, angryBirdNode);

    slingShotNode->setScale(100, 100, 100);
    slingShotNode->setPosition(slingshotInitPos);
    slingShotNode->yaw(Degree(180));
    slingShotNode->pitch(Degree(180));
    btVector3 slingShotShapeSize(getEntityWidth(slingShot), getEntityHeight(slingShot), getEntityDepth(slingShot));
    btScalar slingShotMass = 1.0f;
    physicSysyem.addEntity(slingShot, slingShotNode, slingShotShapeSize, slingShotMass);
    physicSysyem.syncOgre2Bullet(slingShot, slingShotNode);

    pigNode->setScale(0.4f, 0.4f, 0.4f);
    pigNode->setPosition(pigInitPos);
    btVector3 pigShapeSize(getEntityWidth(pig), getEntityHeight(pig), getEntityDepth(pig));
    btScalar pigMass = 1.f;
    physicSysyem.addEntity(pig, pigNode, pigShapeSize, pigMass);
    physicSysyem.syncOgre2Bullet(pig, pigNode);
    
    woodBoxNode->setPosition(woodboxsInitPos);
    Real woobBoxScaleRatio = 0.3;
    woodBoxNode->setScale(Vector3(0.1*woobBoxScaleRatio, 35* woobBoxScaleRatio, 35* woobBoxScaleRatio));
    btVector3 woodBoxShapeSize(getEntityWidth(woodbox), getEntityHeight(woodbox), getEntityDepth(woodbox));
    btScalar woodBoxMass = 15.0f;
    physicSysyem.addEntity(woodbox, woodBoxNode, woodBoxShapeSize, woodBoxMass);
    physicSysyem.syncOgre2Bullet(woodbox, woodBoxNode);

    placeTop(woodbox, pig, woodBoxNode, pigNode);
    physicSysyem.syncOgre2Bullet(pig, pigNode);
    physicSysyem.syncOgre2Bullet(woodbox, woodBoxNode);
    cout << "=====================================" << endl;
    cout << "place pig on woodbox" << endl;
    cout << "woodbox Position" << woodBoxNode->getPosition() << endl;
    cout << "Pig Position" << pigNode->getPosition() << endl;
    cout << "=====================================" << endl;

    // attach objects
    angryBirdNode->attachObject(angryBird);
    pigNode->attachObject(pig);
    slingShotNode->attachObject(slingShot);
    woodBoxNode->attachObject(woodbox);

    mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    // Create a Light and set its position
    Light* light = mSceneMgr->createLight("MainLight");
    SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LightNode");
    lightNode->attachObject(light);
    lightNode->setPosition(200, 600, 500);

}

bool TutorialApplication::frameRenderingQueued(const FrameEvent& evt)
{
    physicSysyem.stepSimulation(1.0f/600.0f); //suppose you have 60 frames per second
    return BaseApplication::frameRenderingQueued(evt);
}

bool TutorialApplication::keyPressed(const KeyboardEvent& evt)
{
    if (mTrayMgr->isDialogVisible()) return true;

    bool moveSucess = true;
    switch (evt.keysym.sym)
    {
    case 'i':
        moveSucess = physicSysyem.giveObjectVelocity("angryBird", btVector3(0, 0, -5));
        if (!moveSucess)
            cout << "angryBird not found\n";
        cout << "execute successfully!!!\n";
        break;
    default:
        moveSucess = physicSysyem.giveObjectVelocity("angryBird", btVector3(0, 0, 0));
        if (!moveSucess)
            cout << "angryBird not found\n";
        break;
    }
    return BaseApplication::keyPressed(evt);
}

//-------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    try
    {
    	TutorialApplication app;
        app.initApp();
		app.go();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
