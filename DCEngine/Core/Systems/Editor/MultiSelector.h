#pragma once

namespace DCEngine {
  namespace Systems {

    class MultiSelector {
    public:


    private:
      bool MultiSelectDragging = false;
      Vec4 MultiSelectColor = Vec4(0.3, 0.7, 0.3f, 0.5f);

      Vec3 MultiSelectMidpoint;
      Vec3 MultiSelectStartPos;
      Vec3 MultiSelectArea;

      Vec3  SelectedBoundingCenter;
      float SelectedBoundingWidth;
      float SelectedBoundingHeight;

    };

  }
}