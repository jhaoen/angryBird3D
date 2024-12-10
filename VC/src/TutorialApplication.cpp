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
    AxisAlignedBox boundingBox = entity->getBoundingBox();
    return boundingBox.getCenter();
}
Real getEntityHeight(Entity* entity)
{
    AxisAlignedBox boundingBox = entity->getBoundingBox();

    Vector3 min = boundingBox.getMinimum(); // min point
    Vector3 max = boundingBox.getMaximum(); // max point
    return max.y - min.y;
}
Real getEntityWidth(Entity* entity)
{
    AxisAlignedBox boundingBox = entity->getBoundingBox();

    Vector3 min = boundingBox.getMinimum(); // min point
    Vector3 max = boundingBox.getMaximum(); // max point
    return max.x - min.x;
}
Real getEntityDepth(Entity* entity)
{
    AxisAlignedBox boundingBox = entity->getBoundingBox();

    Vector3 min = boundingBox.getMinimum(); // min point
    Vector3 max = boundingBox.getMaximum(); // max point
    return max.z - min.z;
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
        1000, 1000, 1, 1,
        true,
        1, 10, 10,
        Vector3::UNIT_Z);
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

    // set attributes and layout

    //angryBirdNode->setScale(3, 3, 3);
    angryBirdNode->setPosition(getEntityCenter(angryBird));
    btVector3 birdShapeSize(getEntityWidth(angryBird), getEntityHeight(angryBird), getEntityDepth(angryBird));
    btScalar birdMass = 0.1f;
    btVector3 birdStartPosition(0, getEntityHeight(angryBird) + getEntityHeight(woodbox), 0); // bird on woodbox
    btQuaternion birdStartRotation(btVector3(0, 1, 0), -SIMD_PI);
    physicSysyem.addEntity(angryBird, angryBirdNode, birdShapeSize, birdMass, birdStartPosition, birdStartRotation);

    //slingShotNode->setScale(70, 70, 70);
    slingShotNode->rotate(Vector3::UNIT_X, Degree(180));
    slingShotNode->yaw(Degree(180));
    btVector3 slingShotShapeSize(getEntityWidth(slingShot), getEntityHeight(slingShot), getEntityDepth(slingShot));
    btScalar slingShotMass = 10.0f;
    btVector3 slingShotStartPosition(0, getEntityHeight(slingShot), 0);
    btQuaternion slingShotStartRotation(1.0f, 0.0f, 0.0f, 0);
    physicSysyem.addEntity(slingShot, slingShotNode, slingShotShapeSize, slingShotMass, slingShotStartPosition, slingShotStartRotation);

    pigNode->resetOrientation();
    //pigNode->setScale(0.25f, 0.25f, 0.25f);
    btVector3 pigShapeSize(getEntityWidth(pig), getEntityHeight(pig), getEntityDepth(pig));
    btScalar pigMass = 5.f;
    btVector3 pigStartPosition(0, getEntityHeight(pig), -100);
    btQuaternion pigStartRotation(btVector3(0, 1, 0), SIMD_PI / 2);
    physicSysyem.addEntity(pig, pigNode, pigShapeSize, pigMass, pigStartPosition, pigStartRotation);
    
    woodBoxNode->setPosition(getEntityCenter(woodbox));
    woodBoxNode->setScale(Vector3(0.1*0.1, 35*0.1, 35*0.1));
    btVector3 woodBoxShapeSize(getEntityWidth(woodbox), getEntityHeight(woodbox), getEntityDepth(woodbox));
    btScalar woodBoxMass = 15.0f;
    btVector3 woodBoxStartPosition(0, getEntityHeight(woodbox), 0);
    btQuaternion woodBoxStartRotation(1.0f, 0.0f, 0.0f, 0);
    physicSysyem.addEntity(woodbox, woodBoxNode, woodBoxShapeSize, woodBoxMass, woodBoxStartPosition, woodBoxStartRotation);

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
