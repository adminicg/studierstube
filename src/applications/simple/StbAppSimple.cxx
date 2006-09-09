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

#include "StbAppSimple.h"

#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/SceneManager.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <stb/components/event/event.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/starlight/starlight.h>


#include <cstdio>
#include <iostream>

CREATE_COMPONENT_FUNC(SimpleApp)

using namespace stb;

SimpleApp::SimpleApp()
{
    using namespace std;
    //cerr << "Ctor simple app ..." << endl;
   isInit=false;
   needEvent=false;
   needViewer=false;
   needStarlight=false;


}

SimpleApp::~SimpleApp()
{
    //nil
}

void 
SimpleApp::setParameter(stb::string key, std::string value)
{
    if(key=="sceneFile")
    {
        sceneFile=value;
	std::cerr << "sceneFile = " << sceneFile << std::endl;
    }
   else if(key=="needEvent" && value=="true")
   {
        needEvent=true;
	std::cerr << "needEvent = " << needEvent << std::endl;
   }
   else if(key=="needViewer" && value=="true")
   {
        needViewer=true;
	std::cerr << "needViewer = " << needViewer << std::endl;
   }
   else if(key=="needStarlight" && value=="true")
   {
        needStarlight=true;
	std::cerr << "needStarlight = " << needStarlight << std::endl;
   }
}


/// Called before the application is destructed.
bool 
SimpleApp::init()
{
  std::cerr << "init" << std::endl;
    if(isInit)
        return true;
 
     //get viewer's parameter
     retrieveParameter();

#ifdef WIN32
#pragma message(">>> daniel2all: having the application to check for all components is not beautiful...")
#endif
    if(needViewer){
        Viewer* viewer=(Viewer*)(Kernel::getInstance()->getComponentManager()->load("Viewer"));
        if(!viewer)
            return false;
    }
    if(needStarlight)
    {
        Starlight* starlight=(Starlight*)(stb::Kernel::getInstance()->getComponentManager()->load("Starlight"));
        if(!starlight)
        {
            return false;
        }
    }
    if(needEvent)
    {
        Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
        if(!event)
            return false;
    }


    //load iv file 
    if(sceneFile.size()<1)
        return false;
    SoInput::addDirectoryFirst("./");
    SoInput myinput;
    if (!myinput.openFile(sceneFile.c_str())) {

      //stb::Kernel::getInstance()->log("STB_ERROR: can not open file:" + sceneFile + "\n");
      std::cerr << "problem opening file..." << std::endl;
        SoInput::removeDirectory("./");
        return false;
    }
    root=SoDB::readAll(&myinput);
    myinput.closeFile();
    SoInput::removeDirectory("./");
    if (root==NULL) 
    {
//        stb::Kernel::getInstance()->log("STB_ERROR: problem reading file: " + sceneFile + "\n");
        return false;
    }
    
    //register scene 
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
//	Kernel::getInstance()->logDebug("update simple app\n");
}

void 
SimpleApp::registerScene()
{
    stb::Kernel::getInstance()->getSceneManager()->registerApp(root);
}
