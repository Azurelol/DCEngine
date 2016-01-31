/******************************************************************************/
/*!
@file   EditorToolsDraw.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/30/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Editor.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  The select tool allows the user to select an object on screen.
    */
    /**************************************************************************/
    void Editor::DrawSelection()
    {
      if (!SelectedObject())
        return;

      if (IsSelectableGameObject(SelectedObject())) {

        // If drawing only 1 object, draw the bounding box larger so it can be seen!
        auto& width = Selection.SelectedBoundingWidth;
        auto& height = Selection.SelectedBoundingHeight;

        if (SelectedObjects.size() == 1) {
          width *= 2;
          height *= 2;
        }

        // Draw a selected 'box' encompassing all selected objects
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(Selection.SelectedBoundingCenter,
          width, height, Vec4(1, 0, 0, 1));
      }
    }

    /**************************************************************************/
    /*!
    @brief  The translate tool allows the user to move an object on screen.
    */
    /**************************************************************************/
    void Editor::DrawTranslateTool()
    {
      if (!SelectedObject())
        return;

      if (IsSelectableGameObject(SelectedObject())) {

        Vec3& pos = Selection.SelectedBoundingCenter;
        Real tip = 0.5;
        Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
        Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
                                         // X-axis

        // Bounding rectangle
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos, Selection.SelectedBoundingWidth / 8, Selection.SelectedBoundingWidth / 8, Vec4(1, 1, 1, 1));
        // X-axis
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, TransformData.XAxisBoundaryEnd, xColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(TransformData.XAxisBoundaryEnd, TransformData.XAxisMidpoint - Vec3(-tip, -tip, 0), xColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(TransformData.XAxisBoundaryEnd, TransformData.XAxisMidpoint - Vec3(-tip, tip, 0), xColor);

        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, TransformData.YAxisBoundaryEnd, yColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(TransformData.YAxisBoundaryEnd, TransformData.YAxisMidpoint - Vec3(-tip, -tip, 0), yColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(TransformData.YAxisBoundaryEnd, TransformData.YAxisMidpoint - Vec3(tip, -tip, 0), yColor);
      }

      // Create thin box-colliders on every line

    }

    /**************************************************************************/
    /*!
    @brief  The rotate tool allows the user to rotate an object.
    */
    /**************************************************************************/
    void Editor::DrawRotateTool()
    {
      if (!SelectedObject())
        return;

      if (auto gameObject = IsSelectableGameObject(SelectedObject())) {

        Vec3& pos = Selection.SelectedBoundingCenter;
        Real radius = Selection.SelectedBoundingWidth;

        if (SelectedObjects.size() == 1) {
          radius *= 2;
        }

        Vec4 color(0.0f, 0.0f, 1.0f, 1.0);

        if (Settings.Rotating == true)
        {
          auto normal = Vec3(Selection.SelectedBoundingCenter.y - Settings.OriginMousePos.y, -(Selection.SelectedBoundingCenter.x - Settings.OriginMousePos.x), 0);
          normal *= 10;
          auto negNormal = -normal;
          Vec4 colorR(1.0, 0.0, 0.0, 1.0); // Red
          Vec4 colorG(0.0, 1.0, 0.0, 1.0); // Red
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(-(Selection.SelectedBoundingCenter.y - Settings.OriginMousePos.y),
            Selection.SelectedBoundingCenter.x - Settings.OriginMousePos.x, 0) + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0),
            Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), colorG);
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(negNormal + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0),
            normal + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), colorR);
        }

        // Draw a selected 'box' around the object
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawCircle(pos, radius, color);
      }

    }

    /**************************************************************************/
    /*!
    @brief  The scale tool allows the user to scale an object.
    */
    /**************************************************************************/
    void Editor::DrawScaleTool()
    {
      if (!SelectedObject())
        return;

      if (IsSelectableGameObject(SelectedObject())) {

        Vec3& pos = Selection.SelectedBoundingCenter;
        Real radius = 5;
        Real tip = 0.5;
        Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
        Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
                                         // X-axis

        // Bounding rectangle
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos, Selection.SelectedBoundingWidth / 8, Selection.SelectedBoundingWidth / 8, Vec4(1, 1, 1, 1));

        // X-Axis Arrow
        auto xAxisArrowEnd = pos + Vec3(radius, 0, 0);
        auto xMidpoint = Vec3((pos.x + xAxisArrowEnd.x) / 2,
                              (pos.y + xAxisArrowEnd.y) / 2,
                              (pos.z + xAxisArrowEnd.z) / 2);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, xAxisArrowEnd, xColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(xAxisArrowEnd, 
                                                                               Selection.SelectedBoundingWidth / 8, 
                                                                               Selection.SelectedBoundingWidth / 8, 
                                                                               xColor);
        // Y-axis Arrow
        auto yAxisArrowEnd = pos + Vec3(0, radius, 0);
        auto yMidpoint = Vec3((pos.x + yAxisArrowEnd.x) / 2,
                              (pos.y + yAxisArrowEnd.y) / 2,
                              (pos.z + yAxisArrowEnd.z) / 2);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, yAxisArrowEnd, yColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(yAxisArrowEnd, 
                                                                               Selection.SelectedBoundingWidth / 8, 
                                                                               Selection.SelectedBoundingWidth / 8, 
                                                                               yColor);
      }

    }


  }
}