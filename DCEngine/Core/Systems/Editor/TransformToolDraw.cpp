#include "TransformTool.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


//    /**************************************************************************/
//    /*!
//    @brief  Activates the selected tool. Called on update.
//    */
//    /**************************************************************************/
//    void EditorToolTransform::Display()
//    {
//      switch (ActiveMode) {
//
//      case Mode::Translate:
//        DrawTranslateTool();
//        break;
//      case Mode::Rotate:
//        DrawRotateTool();
//        break;
//      case Mode::Scale:
//        DrawScaleTool();
//        break;
//      default:
//        break;
//
//      }
//    }
//
//    /**************************************************************************/
//    /*!
//    @brief  The translate tool allows the user to move an object on screen.
//    */
//    /**************************************************************************/
//    void EditorToolTransform::DrawTranslateTool()
//    {
//      if (IsSelectable(SelectedObject())) {
//
//        Vec3& pos = EditorRef.Selection.SelectedBoundingCenter;
//        Real tip = 0.5;
//        Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
//        Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
//                                         // X-axis
//
//                                         // Bounding rectangle
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos, EditorRef.Selection.SelectedBoundingWidth / 8, EditorRef.Selection.SelectedBoundingWidth / 8, Vec4(1, 1, 1, 1));
//        // X-axis
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, XAxisBoundaryEnd, xColor);
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(XAxisBoundaryEnd, XAxisMidpoint - Vec3(-tip, -tip, 0), xColor);
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(XAxisBoundaryEnd, XAxisMidpoint - Vec3(-tip, tip, 0), xColor);
//
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, YAxisBoundaryEnd, yColor);
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(YAxisBoundaryEnd, YAxisMidpoint - Vec3(-tip, -tip, 0), yColor);
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(YAxisBoundaryEnd, YAxisMidpoint - Vec3(tip, -tip, 0), yColor);
//      }
//
//      // Create thin box-colliders on every line
//
//    }
//
//    /**************************************************************************/
//    /*!
//    @brief  The rotate tool allows the user to rotate an object.
//    */
//    /**************************************************************************/
//    void EditorToolTransform::DrawRotateTool()
//    {
//      if (auto gameObject = IsSelectable(SelectedObject())) {
//
//        Vec3& pos = EditorRef.Selection.SelectedBoundingCenter;
//        Real radius = EditorRef.Selection.SelectedBoundingWidth;
//
//        if (EditorRef.AllSelectedObjects().size() == 1) {
//          radius *= 2;
//        }
//
//        Vec4 color(0.0f, 0.0f, 1.0f, 1.0);
//
//        if (Rotating == true)
//        {
//          auto normal = Vec3(EditorRef.Selection.SelectedBoundingCenter.y - OriginMousePos.y, -(EditorRef.Selection.SelectedBoundingCenter.x - OriginMousePos.x), 0);
//          normal *= 10;
//          auto negNormal = -normal;
//          Vec4 colorR(1.0, 0.0, 0.0, 1.0); // Red
//          Vec4 colorG(0.0, 1.0, 0.0, 1.0); // Red
//          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(-(EditorRef.Selection.SelectedBoundingCenter.y - OriginMousePos.y),
//            EditorRef.Selection.SelectedBoundingCenter.x - OriginMousePos.x, 0) + Vec3(OriginMousePos.x, OriginMousePos.y, 0),
//            Vec3(OriginMousePos.x, OriginMousePos.y, 0), colorG);
//          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(negNormal + Vec3(OriginMousePos.x, OriginMousePos.y, 0),
//            normal + Vec3(OriginMousePos.x, OriginMousePos.y, 0), colorR);
//        }
//
//        // Draw a selected 'box' around the object
//        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawCircle(pos, radius, color);
//      }
//
//    }
//
//    /**************************************************************************/
//    /*!
//    @brief  The scale tool allows the user to scale an object.
//    */
//    /**************************************************************************/
//    void EditorToolTransform::DrawScaleTool()
//    {
//
//      Vec3& pos = EditorRef.Selection.SelectedBoundingCenter;
//      Real radius = 5;
//      Real tip = 0.5;
//      Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
//      Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
//                                       // X-axis
//
//                                       // Bounding rectangle
//      CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos, 
//        EditorRef.Selection.SelectedBoundingWidth / 8, EditorRef.Selection.SelectedBoundingWidth / 8, Vec4(1, 1, 1, 1));
//
//      // X-Axis Arrow
//      auto xAxisArrowEnd = pos + Vec3(radius, 0, 0);
//      auto xMidpoint = Vec3((pos.x + xAxisArrowEnd.x) / 2,
//        (pos.y + xAxisArrowEnd.y) / 2,
//        (pos.z + xAxisArrowEnd.z) / 2);
//      CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, xAxisArrowEnd, xColor);
//      CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(xAxisArrowEnd,
//        EditorRef.Selection.SelectedBoundingWidth / 8,
//        EditorRef.Selection.SelectedBoundingWidth / 8,
//        xColor);
//      // Y-axis Arrow
//      auto yAxisArrowEnd = pos + Vec3(0, radius, 0);
//      auto yMidpoint = Vec3((pos.x + yAxisArrowEnd.x) / 2,
//        (pos.y + yAxisArrowEnd.y) / 2,
//        (pos.z + yAxisArrowEnd.z) / 2);
//      CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, yAxisArrowEnd, yColor);
//      CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(yAxisArrowEnd,
//        EditorRef.Selection.SelectedBoundingWidth / 8,
//        EditorRef.Selection.SelectedBoundingWidth / 8,
//        yColor);
//    }
//
  }
}