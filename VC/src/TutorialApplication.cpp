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
   // 為了測試把草地先換成有Phyic的模式
   /* Plane groundPlane = Plane(Vector3::UNIT_Y, 0);
    MeshManager::getSingleton().createPlane(
        "ground",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        groundPlane, 
        1000, 1000, 1, 1,
        true,
        1, 10, 10, 
        Vector3::UNIT_Z);

    Entity* ground = mSceneMgr->createEntity("GroundEntity", "ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ground);
    ground->setMaterialName("GrassFloor"); 
    ground->setCastShadows(false);*/

    // create angry bird scene node and set position
    angryBirdNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("angryBirdNode", Vector3(0,5,0));
    angryBirdNode->setScale(3, 3, 3);
    angryBirdNode->yaw(Degree(180));

    angryBird = mSceneMgr->createEntity("angryBird", "angry_bird.mesh");

    // create slingshot scene node and set position
    slingShotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("slingShotNode", Vector3(0, 7, 0));
    slingShotNode->setScale(70, 70, 70);
    slingShotNode->rotate(Vector3::UNIT_X, Degree(180));
    slingShotNode->yaw(Degree(180));

    slingShot = mSceneMgr->createEntity("slingShot", "slingshot.mesh");

    // create pig scene node and set position
    pigNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("pigNode", Vector3(0, 20, -100));
    pigNode->resetOrientation();
    pigNode->setScale(0.25f, 0.25f, 0.25f);

    pigNode->yaw(Degree(90));

    pig = mSceneMgr->createEntity("pig", "pig.mesh");

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
