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

#include "../Systems/Serialization/Serialization.h"

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

    //#define DCE_JSON_SERIALIZE(PropertyName)   \
    //root[#PropertyName] = PropertyName

    //#define CONCAT_NX(A, B) A ## B
    //#define CONCAT(A, B), CONCAT_NX(A, B)
    //#define STRINGIZE(A) ((A),STRINGIZE_NX(A))

    //#define DCE_JSON_SERIALIZE_VEC4(PropertyName)                     \ 
    //root[#PropertyName "X"] = PropertyName.x;                         \
    //root[#PropertyName "Y"] = PropertyName.y;                         \
    //root[#PropertyName "Z"] = PropertyName.z;                         \
    //root[#PropertyName "W"] = PropertyName.w       

    //#define DCE_JSON_DESERIALIZE_VEC4(PropertyName)                   \ 
    //PropertyName.x = root.get(#PropertyName "X", "").asFloat();       \
    //PropertyName.y = root.get(#PropertyName "Y", "").asFloat();       \
    //PropertyName.z = root.get(#PropertyName "Z", "").asFloat();       \
    //PropertyName.w = root.get(#PropertyName "W", "").asFloat()


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

      void Serialize(Json::Value& root) {
        DCE_JSON_SERIALIZE(AutoSaveTime);
        DCE_JSON_SERIALIZE(GridActive);
        DCE_JSON_SERIALIZE(GridLength);
        DCE_JSON_SERIALIZE(Snapping);
        DCE_JSON_SERIALIZE(SnapDistance);
        DCE_JSON_SERIALIZE(SnapAngle);
        DCE_JSON_SERIALIZE(RecentProject);
        DCE_JSON_SERIALIZE(ProjectsPath);
      }
      void Deserialize(Json::Value& root) {
        DCE_JSON_DESERIALIZE_INTRINSIC(AutoSaveTime).asInt();
        DCE_JSON_DESERIALIZE_INTRINSIC(GridActive).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(GridLength).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(Snapping).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(SnapDistance).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(SnapAngle).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(RecentProject).asString();
        DCE_JSON_DESERIALIZE_INTRINSIC(ProjectsPath).asString();
      }

      EditorConfig() : AutoSaveTime(60), GridActive(true), GridLength(1.0f), GridColor(0.3f, 0.3f, 0.3f, 0.2f),
                       Snapping(true), SnapDistance(1.0f), SnapAngle(15.0f),
                       AutoSaveTimer(AutoSaveTime, Time::Timer::Mode::Countdown, true),
                       CameraLastPos(0.0f, 0.0f, 40.0f) {}
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
      int ScreenWidth;
      int ScreenHeight;
      int Framerate;
      bool Fullscreen;
      std::string Caption;
      Vec4 ClearColor = Vec4(0.0f, 0.5f, 1.0f, 1.0f);
      Vec2 ViewportScale = Vec2(1, 1);
      Vec2 ViewportRatio;

      void Serialize(Json::Value& root) {
        DCE_JSON_SERIALIZE(MaxDrawLayers);
        DCE_JSON_SERIALIZE(ScreenWidth);
        DCE_JSON_SERIALIZE(ScreenHeight);
        DCE_JSON_SERIALIZE(Framerate);
        DCE_JSON_SERIALIZE(Fullscreen);
        DCE_JSON_SERIALIZE(Caption);
        //root["MaxDrawLayers"] = MaxDrawLayers;
        root["ClearColorW"] = ClearColor.w;
        root["ClearColorZ"] = ClearColor.z;
        root["ClearColorY"] = ClearColor.y;
        root["ClearColorX"] = ClearColor.x;
      }

      void Deserialize(Json::Value& root) {
        DCE_JSON_DESERIALIZE_INTRINSIC(MaxDrawLayers).asInt();
        DCE_JSON_DESERIALIZE_INTRINSIC(ScreenWidth).asInt();
        DCE_JSON_DESERIALIZE_INTRINSIC(ScreenHeight).asInt();
        DCE_JSON_DESERIALIZE_INTRINSIC(Framerate).asInt();
        DCE_JSON_DESERIALIZE_INTRINSIC(Fullscreen).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(Caption).asString();
        ClearColor.x = root.get("ClearColorX", "").asFloat();
        ClearColor.y = root.get("ClearColorY", "").asFloat();
        ClearColor.z = root.get("ClearColorZ", "").asFloat();
        ClearColor.w = root.get("ClearColorW", "").asFloat();
      }

      GraphicsConfig() : MaxDrawLayers(5), ScreenWidth(1440), ScreenHeight(900), Framerate(60), Fullscreen(false),
                         Caption("Daisy Chain Engine"), ClearColor(0.0f, 0.5f, 1.0f, 1.0f), ViewportScale(1.0f, 1.0f), 
                         ViewportRatio(1.0f, 1.0f) {}
    };

    /**************************************************************************/
    /*!
    @struct GraphicsConfig Configuration for Audio system.
    */
    /**************************************************************************/
    struct AudioConfig : public Configuration {
      static std::string FileName() { return "ConfigurationAudio.cfg"; }
      bool Enabled;
      int MasterVolume;

      void Serialize(Json::Value& root) {
        DCE_JSON_SERIALIZE(Enabled);
        DCE_JSON_SERIALIZE(MasterVolume);
      }

      void Deserialize(Json::Value& root) {
        DCE_JSON_DESERIALIZE_INTRINSIC(Enabled).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(MasterVolume).asInt();
      }

      AudioConfig() : Enabled(true), MasterVolume(100) {}
    };

    /**************************************************************************/
    /*!
    @struct GraphicsConfig Configuration for GUI system.
    */
    /**************************************************************************/
    struct GUIConfig : public Configuration {
      static std::string FileName() { return "ConfigurationGUI.cfg"; }
      ImGuiStyle& Style; // A pointer to the ImGui Style

      void Serialize(Json::Value& root) {
        DCE_JSON_SERIALIZE(Style.Alpha);
        //DCE_JSON_SERIALIZE_VEC4(Col_Text);
      }

      void Deserialize(Json::Value& root) {
        DCE_JSON_DESERIALIZE_INTRINSIC(Style.Alpha).asFloat();
      }

      void Link() {
        //Style = &ImGui::GetStyle();
      }

      GUIConfig();

    };

    /**************************************************************************/
    /*!
    @struct ConfigurationFiles Configuration files for the engine.
    */
    /**************************************************************************/
    struct ConfigurationFiles {
      EditorConfig Editor;
      GraphicsConfig Graphics;
      AudioConfig Audio;
      GUIConfig GUI;
      static ConfigurationFiles& Access();
    };


  }
}