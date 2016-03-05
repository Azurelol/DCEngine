/*****************************************************************************/
/*!
\file   SystemConfiguration.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   3/3/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../Engine/Timer.h"
#include "../Engine/Command.h"

namespace DCEngine {
  namespace Systems {

       
    /**************************************************************************/
    /*!
    \struct Configuration Serialized configuration class.
    */
    /**************************************************************************/
    struct Configuration {
      //class BaseSerializedProperty {
      //};

      //template <typename T>
      //class SerializedProperty {
      //  T Property;
      //  std::string Name;
      //  SerializedProperty(T propertyName, std::string name) 
      //                    : Property(propertyName), Name(name) {}
      //};
      //std::vector<SerializedProperty> Properties;

      //template <typename PropertyType>
      //void Add(PropertyType property) {
      //  Properties.push_back(property);
      //}

      /**************************************************************************/
      /*!
      @brief Deserializes a property.
      @param root A reference to the JSON root.
      @param property A reference to the property in question.
      @param name The name of the property.
      */
      /**************************************************************************/
      template <typename PropertyType>
      void DeserializeByType(Json::Value& root, PropertyType property, std::string name) {

        if (typeid(property) == typeid(std::string)) {
          property = root.get(name, "").asString();
        }
        else if (typeid(property) == typeid(bool)) {
          property = root.get(name, "").asBool();
        }
        else if (typeid(property) == typeid(int)) {
          property = root.get(name, "").asInt();
        }
        else if (typeid(property) == typeid(float)) {
          property = root.get(name, "").asInt();
        }
      }

      /**************************************************************************/
      /*!
      @brief Saves the configuration to the specified file.
      @param filePath The path which to save the configuration at.
      */
      /**************************************************************************/
      void Save(std::string filePath) {
        std::string configData;
        Serialization::Serialize(*this, configData);
        FileSystem::FileWriteString(filePath, configData);
      }

      virtual void Serialize(Json::Value&) = 0;

    };


    #define DCE_JSON_SERIALIZE(PropertyName)   \
    root[#PropertyName] = PropertyName
    #define DCE_JSON_DESERIALIZE(PropertyName)   \
    Deserialize(root, PropertyName, "##PropertyName")
    #define DCE_JSON_DESERIALIZE_INTRINSIC(PropertyName) \
    PropertyName = root.get(#PropertyName, "")

    /**************************************************************************/
    /*!
    @struct Editor Configuration for the Editor system.
    */
    /**************************************************************************/
    struct EditorConfig : public Configuration {
      static std::string FileName() { return "ConfigurationEditor.cfg"; }

      // Time
      int AutoSaveTime;      
      // Grid
      bool GridActive = true;
      Real GridLength = 1.0f;
      Vec4 GridColor;
      // Snapping
      bool Snapping = true;
      float SnapDistance = 1.0f;
      float SnapAngle = 15.0f;
      // Projects
      std::string RecentProject;
      std::string ProjectsPath;



      Time::Timer AutoSaveTimer;
      bool EditorEnabled = false;
      ProjectProperties* ProjectProperties;
      // Window
      Vec2 ViewportResize;
      // Tools
      bool TransformTool_IsComponent = false;
      //Panning
      bool Panning = false;
      Vec3 CamStartPos;
      Vec3 CameraLastPos;
      Vec3 MouseStartPos;
      Vec3 MouseLastPos;
      // Commands
      CommandManager Commands;
      // Console
      size_t ConsoleWindowStringLength = 0;
      size_t PreviousConsoleWindowStringLength = 0;
      EditorConfig() : AutoSaveTime(60), GridActive(true), GridLength(1.0f), GridColor(0.3f, 0.3f, 0.3f, 0.2f),
                       Snapping(true), SnapDistance(1.0f), SnapAngle(15.0f),
                       AutoSaveTimer(AutoSaveTime, Time::Timer::Mode::Countdown, true),
                       CameraLastPos(0.0f, 0.0f, 40.0f) {}


      void Serialize(Json::Value& root) {
        DCE_JSON_SERIALIZE(AutoSaveTime);
        DCE_JSON_SERIALIZE(GridActive);
        DCE_JSON_SERIALIZE(GridLength);
        DCE_JSON_SERIALIZE(Snapping);
        DCE_JSON_SERIALIZE(SnapDistance);
        DCE_JSON_SERIALIZE(SnapAngle);

      }
      void Deserialize(Json::Value& root) {
        //DeserializeByType(root, AutoSaveTime, "AutoSaveTime");
        //DeserializeByType(root, GridActive, "GridActive");
        //DeserializeByType(root, GridLength, "GridLength");
        //DeserializeByType(root, Snapping, "Snapping");
        //DeserializeByType(root, SnapDistance, "SnapDistance");
        //DeserializeByType(root, SnapAngle, "SnapAngle");

        DCE_JSON_DESERIALIZE_INTRINSIC(AutoSaveTime).asInt();
        DCE_JSON_DESERIALIZE_INTRINSIC(GridActive).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(GridLength).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(Snapping).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(SnapDistance).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(SnapAngle).asFloat();

        //DCE_JSON_DESERIALIZE_INT(AutoSaveTime);
        //DCE_JSON_DESERIALIZE_BOOL(GridActive);
        //DCE_JSON_DESERIALIZE_FLOAT(GridLength);
        //DCE_JSON_DESERIALIZE_BOOL(Snapping);
        //DCE_JSON_DESERIALIZE_FLOAT(SnapDistance);
        //DCE_JSON_DESERIALIZE_FLOAT(SnapAngle);
      }
    };

    /**************************************************************************/
    /*!
    @struct GraphicsConfig Configuration for Graphics system.
    */
    /**************************************************************************/
    struct GraphicsConfig : public Configuration {
      static std::string FileName() { return "ConfigurationGraphics.cfg"; }
      int MaxDrawLayers;
      bool LightningEnabled;
      const int ScreenWidth = 1024;
      const int ScreenHeight = 768;
      Vec4 ClearColor = Vec4(0.0f, 0.5f, 1.0f, 1.0f);
      Vec2 ViewportScale = Vec2(1, 1);
      Vec2 ViewportRatio;
      GraphicsConfig() : MaxDrawLayers(5), ScreenWidth(1024), ScreenHeight(768), 
                         ClearColor(0.0f, 0.5f, 1.0f, 1.0f), ViewportScale(1.0f, 1.0f), ViewportRatio(1.0f, 1.0f) {}

      void Serialize(Json::Value& root) {
        root["MaxDrawLayers"] = MaxDrawLayers;
        root["ClearColorW"] = ClearColor.w;
        root["ClearColorZ"] = ClearColor.z;
        root["ClearColorY"] = ClearColor.y;
        root["ClearColorX"] = ClearColor.x;
      }

      void Deserialize(Json::Value& root) {
        //DCE_JSON_DESERIALIZE_INT(MaxDrawLayers);
        MaxDrawLayers = root.get("MaxDrawLayers", "").asInt();
        ClearColor.x = root.get("ClearColorX", "").asFloat();
        ClearColor.y = root.get("ClearColorY", "").asFloat();
        ClearColor.z = root.get("ClearColorZ", "").asFloat();
        ClearColor.w = root.get("ClearColorW", "").asFloat();
      }
    };

    struct ConfigurationFiles {
      EditorConfig Editor;
      GraphicsConfig Graphics;
    };


  }
}