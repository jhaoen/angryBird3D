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

    // create angry bird scene node & set position & add to physical world
    angryBird = mSceneMgr->createEntity("angryBird", "angry_bird.mesh");
    angryBirdNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("angryBirdNode", Vector3(0,5,0));
    angryBirdNode->setScale(3, 3, 3);


    btVector3 birdShapeSize(1.0f, 1.0f, 1.0f);
    btScalar birdMass = 0.1f;
    btVector3 birdStartPosition(0, 5, 0);
    btQuaternion birdStartRotation(btVector3(0, 1, 0), -SIMD_PI);

    physicSysyem.addEntity(angryBird, angryBirdNode, birdShapeSize, birdMass, birdStartPosition, birdStartRotation);

    // create slingshot scene node & set position & add to physical world
    slingShot = mSceneMgr->createEntity("slingShot", "slingshot.mesh");
    slingShotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("slingShotNode", Vector3(0, 7, 0));
    slingShotNode->setScale(70, 70, 70);
    slingShotNode->rotate(Vector3::UNIT_X, Degree(180));
    slingShotNode->yaw(Degree(180));

    btVector3 slingShotShapeSize(1.0f, 5.0f, 1.0f);
    btScalar slingShotMass = 10.0f;
    btVector3 slingShotStartPosition(0, 7, 0);
    btQuaternion slingShotStartRotation(1.0f, 0.0f, 0.0f, 0);

    physicSysyem.addEntity(slingShot, slingShotNode, slingShotShapeSize, slingShotMass, slingShotStartPosition, slingShotStartRotation);

    // create pig scene node & set position & add to physical world
    pig = mSceneMgr->createEntity("pig", "pig.mesh");
    pigNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("pigNode", Vector3(0, 20, -100));
    pigNode->resetOrientation();
    pigNode->setScale(0.25f, 0.25f, 0.25f);
    // pigNode->yaw(Degree(-90));

    btVector3 pigShapeSize(1.0f, 1.0f, 1.0f);
    btScalar pigMass = 5.f;
    btVector3 pigStartPosition(0, 20, -100);
    btQuaternion pigStartRotation(btVector3(0, 1, 0), SIMD_PI / 2);

    physicSysyem.addEntity(pig, pigNode, pigShapeSize, pigMass, pigStartPosition, pigStartRotation);
    
    // attach objects
    angryBirdNode->attachObject(angryBird);
    pigNode->attachObject(pig);
    slingShotNode->attachObject(slingShot);


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
