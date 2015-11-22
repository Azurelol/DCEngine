/******************************************************************************/
/*!
@file   EditorInput.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/17/2015
@brief  This file includes the implementation for the Editor's input binding.
*/
/******************************************************************************/
#include "Editor.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Receives a KeyDown event.
    @param  event A pointer to the event.
    */
    /**************************************************************************/
    void Editor::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

      case Keys::Tilde:
        WindowConsoleEnabled = !WindowConsoleEnabled;
        break;

      case Keys::F5:        
        PlayGame();
        break;

      case Keys::F8:
        StopGame();
        break;

      case Keys::Num1:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::SelectTool \n";
        ActiveTool = EditorTool::Select;
        break;

      case Keys::Num2:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::TranslateTool \n";
        ActiveTool = EditorTool::Translate;
        break;

      case Keys::Num3:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::RotateTool \n";
        ActiveTool = EditorTool::Rotate;
        break;

      case Keys::Num4:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::ScaleTool \n";
        ActiveTool = EditorTool::Scale;
        break;

      case Keys::Delete:
        DeleteObject();
        break;

      case Keys::W:
        MoveObject(Vec3(0, SnapDistance, 0));
        break;

      case Keys::S:
        MoveObject(Vec3(0, -SnapDistance, 0));
        break;

      case Keys::A:
        MoveObject(Vec3(-SnapDistance, 0,0));
        break;

      case Keys::D:
        MoveObject(Vec3(SnapDistance, 0, 0));
        break;

      default:
        ActiveTool = EditorTool::None;
        break;

      }

    }


    void Editor::Hotkeys(Events::KeyDown * event)
    {


    }

  }
}