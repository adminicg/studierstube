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
/** The header file for the StbModule class.
*
* @author Denis Kalkofen
*
* $Id: StbMoudle.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef __STBMODULE_H__
#define __STBMODULE_H__



#include <string.h>
#include "StbEvent.h"
#include "StbSink.h"

class ot::NodeFactory;
class ot::Module;

class STBEVENT_API StbModule : public ot::Module, public ot::NodeFactory
{
public:
    /**
     * the constructor
     */
    StbModule(); 

    /** destructor deposes of any data structures. */
    virtual ~StbModule();

    virtual ot::Node * createNode( const std::string& name,  ot::StringTable& attributes);

	StbSink* getStbSink(std::string stbSinkName);
protected:

#ifdef WIN32
	template class STBEVENT_API std::allocator<StbSink*>;
	template class STBEVENT_API std::vector<StbSink*, std::allocator<StbSink*> >;
#endif
	std::vector<StbSink*> stbSinks;

	
};

#endif
