/******************************************************************************/
/*!
@file   EditorSelect.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Editor.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief  Selects the Space object for property inspection.
    */
    /**************************************************************************/
    void Editor::SelectSpace()
    {
      SelectedObject = CurrentSpace;
      WidgetPropertiesEnabled = true;
    }

  }
}