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
  
  namespace Systems {
    class Editor;
  }

  namespace Events {

    class EditorEnabled : public Event {
    public:
      EditorEnabled() : Event("EditorEnabled") {}
    };



    class EditorInitialize : public Event {
    public:
      EditorInitialize() : Event("EditorInitialize") {}
      Systems::Editor* Reference;
    };

    class EditorSelectObject : public Event {
    public:
      EditorSelectObject() : Event("EditorSelectObject") {}
     // Object* SelectedObject;
    };

    class EditorDeselectObject : public Event {
    public:
      EditorDeselectObject() : Event("EditorDeselectObject") {}
    };

    class EditorSave : public Event {
    public:
      EditorSave() : Event("EditorSave") {}
    };
    
    class EditorUpdate : public Event {
    public:
      EditorUpdate() : Event("EditorUpdate") {}
    };

    class EditorRebuildZilchComponents : public Event {
    public:
      EditorRebuildZilchComponents() : Event("EditorRebuildZilchComponents") {}
    };


  }
}