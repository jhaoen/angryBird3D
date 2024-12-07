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

#include "BaseApplication.h"


//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication() : ApplicationContext("OgreBasicApp"),
	mRoot(nullptr),
	mCameraNode(nullptr),
	mCamera(nullptr),
	mSceneMgr(nullptr),
	mWindow(nullptr),
	mCameraMan(nullptr),
	mTrayMgr(nullptr),
	mDetailsPanel(nullptr),
	mCursorWasVisible(false),
	mShutDown(false)
{
}


//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;
}


//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog(getNativeConfigDialog()))
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to get the default rendering window
		mWindow = getRenderWindow();

		return true;
	}
	else
	{
		return false;
	}
}


//-------------------------------------------------------------------------------------
void BaseApplication::createSceneManager(void)
{
	// Create the SceneManager, in this case a default one
	mSceneMgr = mRoot->createSceneManager();
}


//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
	// Create the camera
	mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CameraNode");
	mCamera = mSceneMgr->createCamera("myCam");

	mCamera->setAutoAspectRatio(true);
	mCameraNode->attachObject(mCamera);


	mCameraNode->setPosition(20, 30, 100);
	mCameraNode->lookAt(Vector3(0, 0, -100), Node::TS_WORLD);
	mCamera->setNearClipDistance(5);

	mCameraMan = new CameraMan(mCameraNode);   // create a default camera controller
}


//-------------------------------------------------------------------------------------
void BaseApplication::createTray(void)
{
	// Create the camera
	mTrayMgr = new TrayManager("InterfaceName", mWindow, this);
	mTrayMgr->showFrameStats(TL_BOTTOMLEFT);
	mTrayMgr->showLogo(TL_BOTTOMRIGHT);
	mTrayMgr->hideCursor();

	// Create a params panel for displaying sample details
	StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("");
	items.push_back("Filtering");
	items.push_back("Poly Mode");

	mDetailsPanel = mTrayMgr->createParamsPanel(TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(9, "Bilinear");
	mDetailsPanel->setParamValue(10, "Solid");
	mDetailsPanel->hide();
}


//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}


//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
	mRoot->startRendering();

	// Clean up
	destroyScene();
}


//-------------------------------------------------------------------------------------
void BaseApplication::setup(void)
{
    // Do not forget to call the base first
    ApplicationContext::setup();
    addInputListener(this);

    // Get a pointer to the already created root
	mRoot = getRoot();

	// Create the scene manager
	createSceneManager();

	// Register our scene with the RTSS
	mShaderGen = RTShader::ShaderGenerator::getSingletonPtr();
	mShaderGen->addSceneManager(mSceneMgr);

	// Create the camera
	createCamera();

    // Render into the main window
	if (!configure()) return;
	mWindow->addViewport(mCamera)->setBackgroundColour(ColourValue(0, 0, 0));

	// Create the tray
	mSceneMgr->addRenderQueueListener(getOverlaySystem()); // Enable drawing of Overlays
	createTray();

	// Create the scene
	createScene();
}


//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const FrameEvent &evt)
{
	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;

	// Need to capture/update each device
	mTrayMgr->frameRendered(evt);

	if (!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRendered(evt);   // If dialog isn't up, then update the camera
		if (mDetailsPanel->isVisible())   // If details panel is visible, then update its contents
		{
			mDetailsPanel->setParamValue(0, StringConverter::toString(mCameraNode->getPosition()[0]));
			mDetailsPanel->setParamValue(1, StringConverter::toString(mCameraNode->getPosition()[1]));
			mDetailsPanel->setParamValue(2, StringConverter::toString(mCameraNode->getPosition()[2]));
			mDetailsPanel->setParamValue(4, StringConverter::toString(mCameraNode->getOrientation()[3]));
			mDetailsPanel->setParamValue(5, StringConverter::toString(mCameraNode->getOrientation()[0]));
			mDetailsPanel->setParamValue(6, StringConverter::toString(mCameraNode->getOrientation()[1]));
			mDetailsPanel->setParamValue(7, StringConverter::toString(mCameraNode->getOrientation()[2]));
		}
	}

	return ApplicationContext::frameRenderingQueued(evt);
}


//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed(const KeyboardEvent &evt)
{
	if (mTrayMgr->isDialogVisible()) return true;   // Don't process any more keys if dialog is up
	
	switch (evt.keysym.sym)
	{
	// Toggle visibility of advanced frame stats
	case 'f':
		mTrayMgr->toggleAdvancedFrameStats();
		break;

	// Toggle visibility of even rarer debugging details
	case 'g':
		if (mDetailsPanel->getTrayLocation() == TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
		break;

	// Cycle texture filtering mode
	case 't':
		{
			String newVal;
			TextureFilterOptions tfo;
			unsigned int aniso;

			switch (mDetailsPanel->getParamValue(9)[0])
			{
			case 'B':
				newVal = "Trilinear";
				tfo = TFO_TRILINEAR;
				aniso = 1;
				break;

			case 'T':
				newVal = "Anisotropic";
				tfo = TFO_ANISOTROPIC;
				aniso = 8;
				break;

			case 'A':
				newVal = "None";
				tfo = TFO_NONE;
				aniso = 1;
				break;

			default:
				newVal = "Bilinear";
				tfo = TFO_BILINEAR;
				aniso = 1;
			}

			MaterialManager& matMgr = MaterialManager::getSingleton();
			matMgr.setDefaultTextureFiltering(tfo);
			matMgr.setDefaultAnisotropy(aniso);
			mDetailsPanel->setParamValue(9, newVal);
		}
		break;

	// Cycle polygon rendering mode
	case 'r':
		{
			String newVal;
			PolygonMode pm;

			switch (mCamera->getPolygonMode())
			{
			case PM_SOLID:
				newVal = "Wireframe";
				pm = PM_WIREFRAME;
				break;
			case PM_WIREFRAME:
				newVal = "Points";
				pm = PM_POINTS;
				break;
			default:
				newVal = "Solid";
				pm = PM_SOLID;
			}

			mCamera->setPolygonMode(pm);
			mDetailsPanel->setParamValue(10, newVal);
		}
		break;

	// Refresh all textures
	case SDLK_F5:
		TextureManager::getSingleton().reloadAll();
		break;

	// Take a screenshot
	case SDLK_PRINTSCREEN:
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
		break;

	case SDLK_ESCAPE:
		mShutDown = true;
		mRoot->queueEndRendering();
		break;
	}

	return mCameraMan->keyPressed(evt);
}


//-------------------------------------------------------------------------------------
bool BaseApplication::keyReleased(const KeyboardEvent &evt)
{
	return mCameraMan->keyReleased(evt);
}


//-------------------------------------------------------------------------------------
bool BaseApplication::mouseMoved(const MouseMotionEvent &evt)
{
	if (mTrayMgr->mouseMoved(evt)) return true;
	return mCameraMan->mouseMoved(evt);
}


//-------------------------------------------------------------------------------------
bool BaseApplication::mousePressed(const MouseButtonEvent &evt)
{
	if (mTrayMgr->mousePressed(evt)) return true;
	return mCameraMan->mousePressed(evt);
}


//-------------------------------------------------------------------------------------
bool BaseApplication::mouseReleased(const MouseButtonEvent &evt)
{
	if (mTrayMgr->mouseReleased(evt)) return true;
	return mCameraMan->mouseReleased(evt);
}
