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

#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreBitesConfigDialog.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreCameraMan.h>
#include <OgreTrays.h>

#include <iostream>

using namespace Ogre;
using namespace OgreBites;

class BaseApplication : public ApplicationContext, public InputListener, public TrayListener
{
public:
	BaseApplication(void);
    virtual ~BaseApplication(void);

	virtual void go(void);

protected:
	virtual void setup(void);
	virtual bool configure(void);
	virtual void createSceneManager(void);
	virtual void createCamera(void);
	virtual void createTray(void);
	virtual void createScene(void) = 0; // Override me!
	virtual void destroyScene(void);

	virtual bool frameRenderingQueued(const FrameEvent &evt);

	// InputListener
	virtual bool keyPressed(const KeyboardEvent &evt);
	virtual bool keyReleased(const KeyboardEvent &evt);
	virtual bool mouseMoved(const MouseMotionEvent &evt);
	virtual bool mousePressed(const MouseButtonEvent &evt);
	virtual bool mouseReleased(const MouseButtonEvent &evt);

	Root* mRoot;
	Camera* mCamera;
	SceneNode* mCameraNode;
	SceneManager* mSceneMgr;
	RenderWindow* mWindow;

	TrayManager* mTrayMgr;
	CameraMan* mCameraMan;		// Basic camera controller
	ParamsPanel* mDetailsPanel;	// Sample details panel
	bool mCursorWasVisible;		// Was cursor visible before dialog appeared
	bool mShutDown;

	RTShader::ShaderGenerator* mShaderGen;
};

#endif // __BaseApplication_h_
