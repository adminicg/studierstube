/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/* @author Denis Kalkofen
*
* $Id: SimpleApp.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "SimpleApp.h"
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/SceneManager.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <stb/components/event/event.h>
//#include <stb/components/event/SoTrackedArtifactKit.h>
#include <cstdio>

CREATE_COMPONENT_FUNC(SimpleApp)

using namespace stb;

SimpleApp::SimpleApp()
{
   //nil
}

SimpleApp::~SimpleApp()
{
      printf("delete SimpleApp\n");
}

/// Called before the application is destructed.
bool 
SimpleApp::init()
{
    printf("init SimpleApp\n");
    if(isInit)
        return true;
     isInit=true;
    //need tracking --> check if componentmanager has loaded the event component.
    if(!Kernel::getInstance()->getComponentManager()->load("Viewer"))
    {
        isInit=false;
        return false;
    }
    Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
    if(!event)
    {
        isInit=true;
        return false;
    }
   
    printf("\n------------- SimpleApp loaded deps ----------\n");

    root = new SoSeparator();
    SoTransform *tran=new SoTransform;
    tran->translation.setValue(0.0,0.0,-6.0);
    root->addChild(tran);
    root->addChild(new SoSphere);
    SoTrakEngine* tre= event->createSoTrakEngine();
    if(!tre)
    {
        isInit=false;
        return false;
    }
    ////SoTrakEngine *tre=new SoTrakEngine;
    //tre->key.set1Value(0,"blabla");
    //tre->value.set1Value(0,"hi");
    //SoTransform *trans=new SoTransform;
    //trans->translation.connectFrom(&tre->translation);
    //trans->rotation.connectFrom(&tre->rotation);
    //SoSeparator *treRoot=new SoSeparator;
    //treRoot->addChild(trans);
    //treRoot->addChild(new SoCube);
    //root->addChild(treRoot);
    //// test SoTrackedArtifactKit
    //SoTrackedArtifactKit *trak=new SoTrackedArtifactKit;
    //trak->key.set1Value(0,"blabla");
    //trak->value.set1Value(0,"hi");

    //trak->setGeometry(new SoCone);
    //SoSeparator *trakRoot=new SoSeparator;
    //trakRoot->addChild(trak);
    //root->addChild(trakRoot);

    registerScene();

    isInit=true;
	return isInit;
}

/// Called before the application is destructed.
void 
SimpleApp::shutDown()
{
    root->unref();
}

void 
SimpleApp::update()
{
	printf("update simple app\n");
}

void 
SimpleApp::registerScene()
{
    stb::Kernel::getInstance()->getSceneManager()->registerApp(root);
}
