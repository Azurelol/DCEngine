/******************************************************************************/
/*!
@file   EditorProjects.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "EditorModule.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    EditorModule::EditorModule(bool add) : Module("EditorModule"), WillAddWindow(add), WindowEnabled(false)
    {
      Daisy->Connect<Events::EditorUpdate>(&EditorModule::OnEditorUpdateEvent, this);
      Daisy->Connect<Events::EditorInitialize>(&EditorModule::OnEditorInitializeEvent, this);
    }

    EditorModule::~EditorModule()
    {
    }

    void EditorModule::AddWindow()
    {
      if (WillAddWindow)
        Access().Add(this);
    }

    void EditorModule::OnEditorInitializeEvent(Events::EditorInitialize * event)
    {
      EditorRef = event->Reference;
      AddWindow();
    }

    void EditorModule::OnEditorUpdateEvent(Events::EditorUpdate * event)
    {
      Update();
    }
    void EditorModule::Toggle(bool toggle)
    {
      WindowEnabled = toggle;
    }

    void EditorModule::Toggle()
    {
      WindowEnabled = !WindowEnabled;
    }

    Editor & EditorModule::Access()
    {
      return *EditorRef;
    }


  }
}