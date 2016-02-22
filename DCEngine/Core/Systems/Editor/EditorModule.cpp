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

    EditorModule::EditorModule(Editor & editor) : Module("EditorModule"), EditorRef(editor)
    {
      Daisy->Connect<Events::EditorUpdate>(&EditorModule::OnEditorUpdateEvent, this);
    }

    EditorModule::~EditorModule()
    {
    }

    void EditorModule::OnEditorUpdateEvent(Events::EditorUpdate * event)
    {
      Update();
    }

  }
}