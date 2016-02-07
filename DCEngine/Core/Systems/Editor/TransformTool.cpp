#include "TransformTool.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
        
    /**************************************************************************/
    /*!
    @brief  TransformTool constructor
    */
    /**************************************************************************/
    TransformTool::TransformTool(Editor& editor) : EditorTool(editor)
    {
    }
    
    /**************************************************************************/
    /*!
    @brief  Checks whether the currently selected object is within this editor's region.
    @param translation The translation of the object.
    @param scale The scale of the object.
    */
    /**************************************************************************/
    bool TransformTool::WithinRegion(const Vec3& translation, const Vec3& scale)
    {
      return false;
    }



    

    


  }
}