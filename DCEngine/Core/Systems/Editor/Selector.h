/******************************************************************************/
/*!
@file   MultiSelector.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/5/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// Engine
#include "../../Objects/Object.h"
#include "../../Objects/ObjectsInclude.h"
#include "../../Objects/Entities/EntitiesInclude.h"

namespace DCEngine {
  namespace Systems {
    
    class Editor;
    class Selector {
    public:

      Selector(Editor& editor);
      GameObjectPtr IsSelectableGameObject(ObjectPtr);
      void SelectMultiple(Vec2 & mousePosition, ObjectContainer selectedObjects, SpacePtr currentSpace);
      void CalculateSelectionBounding(ObjectPtr selectedObject, ObjectContainer selectedObjects);      
      void DrawMultiSelect(SpacePtr currentSpace);

    private:

      Editor& Editor;
      // Selection
      Vec3  SelectedBoundingCenter;
      float SelectedBoundingWidth;
      float SelectedBoundingHeight;
      // Multi-selection
      bool MultiSelectDragging = false;
      Vec4 MultiSelectColor = Vec4(0.3, 0.7, 0.3f, 0.5f);
      Vec3 MultiSelectMidpoint;
      Vec3 MultiSelectStartPos;
      Vec3 MultiSelectArea;


    };


  }
}