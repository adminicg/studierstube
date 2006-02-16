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
* $Id: ComponentInfoBase.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/Kernel.h>

#include <tinyxml.h>


BEGIN_NAMESPACE_STB

ComponentInfo::ComponentInfo(){
    libHandle=NULL;
}

ComponentInfo::~ComponentInfo(){
    if(libHandle)
        os_FreeLibrary(libHandle);
}

void 
ComponentInfo::setHModule(hModule aLibHandle)
{
    if(libHandle)
        os_FreeLibrary(libHandle);
    libHandle = aLibHandle;
}

void 
ComponentInfo::parseConfiguration(TiXmlElement* element)
{
    TiXmlAttribute* attribute = element->FirstAttribute();
    while(attribute)
    {
        ///////////////// Library name /////////////////
        if(!stb::stricasecmp(attribute->Name(),"lib"))
        {
            libName = attribute->Value();
        }
        ///////////////// Logging.mode /////////////////
        else if(!stb::stricasecmp(attribute->Name(),"name"))
        {
            name = attribute->Value();
        }
        ///////////////// Logging. /////////////////
        //else if(!stricmp(attribute->Name(),"----"))
        //{		
        //}
        attribute = attribute->Next();
    }

    //  <Param key   ="configFile" 
    //       value ="viewer.iv" 
    //  />	
    TiXmlElement* child = element->FirstChildElement();
    while(child)
    {
        if(!stb::stricasecmp(child->Value(),"Param"))
        {
            stb::string key="";
            stb::string value="";
            attribute = child->FirstAttribute();
            while(attribute)
            {
                /////////////// Library name /////////////////
                if(!stb::stricasecmp(attribute->Name(),"key"))
                {
                    key = attribute->Value();
                }
                /////////////// Logging.mode /////////////////
                else if(!stb::stricasecmp(attribute->Name(),"value"))
                {
                    value = attribute->Value();
                }
                attribute = attribute->Next();
            }
            if(key!="" && value!="")
                parameter[key]=value;
        }
        child = child->NextSiblingElement();
    }

    if(name==""){
        Kernel::getInstance()->log("ERROR: missing attribute 'name' for Component\n");
    }
    if(libName==""){
        Kernel::getInstance()->log("ERROR: missing attribute 'lib' for Component\n");
    }
}

END_NAMESPACE_STB

//========================================================================
// End of file
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
