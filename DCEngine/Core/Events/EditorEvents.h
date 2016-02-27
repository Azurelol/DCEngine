/******************************************************************************/
/*!
\file   EditorEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/6/2015
\brief  Events for the editor system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  // Forward declarations
  //class Object;    
  
  namespace Events {

    class EditorEnabled : public Event {
    public:
    };

    class EditorInitialize : public Event {
    };

    class EditorSelectObject : public Event {
    public:
     // Object* SelectedObject;
    };

    class EditorDeselectObject : public Event {
    };
    
    class EditorUpdate : public Event {
    };

  }
}