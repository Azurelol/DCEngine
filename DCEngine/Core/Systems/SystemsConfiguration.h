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
      const std::string FileName;
      static std::string Folder() { return "Configuration"; }
      Configuration(std::string extension);
      virtual void Deserialize(Json::Value&) = 0;
      virtual void Serialize(Json::Value&) = 0;
      bool Save();
      bool Load();
      bool Load(std::string filePath);
      void Save(std::string filePath);

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
    };

    // MACROS!
    #define DCE_JSON_SERIALIZE(PropertyName)   \
    root[#PropertyName] = PropertyName

    #define DCE_JSON_SERIALIZE_VEC4(PropertyName) \
    root[#PropertyName "X"] = PropertyName.x; \
    root[#PropertyName "Y"] = PropertyName.y; \
    root[#PropertyName "Z"] = PropertyName.z; \
    root[#PropertyName "W"] = PropertyName.w

    #define DCE_JSON_DESERIALIZE_VEC4(PropertyName) \
    PropertyName.x = root.get(#PropertyName "X", 0.f).asFloat(); \
    PropertyName.y = root.get(#PropertyName "Y", 0.f).asFloat(); \
    PropertyName.z = root.get(#PropertyName "Z", 0.f).asFloat(); \
    PropertyName.w = root.get(#PropertyName "W", 0.f).asFloat()

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
      EditorConfig();
      static std::string FileName() { return "ConfigurationEditor.cfg"; }
      //----------------------------------------------------------------------/
      // Saving
      bool AutoSave;
      int AutoSaveTime;      
      // Grid
      bool GridActive = true;
      Real GridLength = 1.0f;
      Vec4 GridColor;
      // Snapping
      bool Snapping = true;
      float SnapDistance = 1.0f;
      float SnapAngle = 15.0f;
      Vec4 LockedColor;
      //Text Editor
      std::string ExternalTextEditor;
      bool TextEditorOpenExternally;
      bool CompileOnContextSwitch;
      bool CompilationPopUps;
      //----------------------------------------------------------------------/
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
        DCE_JSON_SERIALIZE(AutoSave);
        DCE_JSON_SERIALIZE(AutoSaveTime);
        DCE_JSON_SERIALIZE(GridActive);
        DCE_JSON_SERIALIZE(GridLength);
        DCE_JSON_SERIALIZE(Snapping);
        DCE_JSON_SERIALIZE(SnapDistance);
        DCE_JSON_SERIALIZE(SnapAngle);
        DCE_JSON_SERIALIZE(RecentProject);
        DCE_JSON_SERIALIZE(ProjectsPath);
        DCE_JSON_SERIALIZE(ExternalTextEditor);
        DCE_JSON_SERIALIZE(CompileOnContextSwitch);
        DCE_JSON_SERIALIZE(CompilationPopUps);
        DCE_JSON_SERIALIZE(TextEditorOpenExternally);
        DCE_JSON_SERIALIZE_VEC4(LockedColor);
      }

      void Deserialize(Json::Value& root) {
        DCE_JSON_DESERIALIZE_INTRINSIC(AutoSave).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(AutoSaveTime).asInt();
        DCE_JSON_DESERIALIZE_INTRINSIC(GridActive).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(GridLength).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(Snapping).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(SnapDistance).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(SnapAngle).asFloat();
        DCE_JSON_DESERIALIZE_INTRINSIC(RecentProject).asString();
        DCE_JSON_DESERIALIZE_INTRINSIC(ProjectsPath).asString();
        DCE_JSON_DESERIALIZE_INTRINSIC(ExternalTextEditor).asString();
        DCE_JSON_DESERIALIZE_INTRINSIC(CompileOnContextSwitch).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(CompilationPopUps).asBool();
        DCE_JSON_DESERIALIZE_INTRINSIC(TextEditorOpenExternally).asBool();
        DCE_JSON_DESERIALIZE_VEC4(LockedColor);
      }
      

    };

    /**************************************************************************/
    /*!
    @struct GraphicsConfig Configuration for Graphics system.
    */
    /**************************************************************************/
    struct GraphicsConfig : public Configuration {
      GraphicsConfig();
      static std::string FileName() { return "ConfigurationGraphics.cfg"; }
      int MaxDrawLayers;
      bool LightningEnabled;
      int ScreenWidth;
      int ScreenHeight;
      int Framerate;
      bool Fullscreen;
      int Samples;
      std::string Caption;
      Vec4 ClearColor = Vec4(0.0f, 0.5f, 1.0f, 1.0f);
      float Exposure;

      void Serialize(Json::Value& root) {
        DCE_JSON_SERIALIZE(MaxDrawLayers);
        DCE_JSON_SERIALIZE(ScreenWidth);
        DCE_JSON_SERIALIZE(ScreenHeight);
        DCE_JSON_SERIALIZE(Framerate);
        DCE_JSON_SERIALIZE(Fullscreen);
        DCE_JSON_SERIALIZE(Caption);
        DCE_JSON_SERIALIZE(Exposure);
				DCE_JSON_SERIALIZE(Samples);
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
        DCE_JSON_DESERIALIZE_INTRINSIC(Exposure).asFloat();
				DCE_JSON_DESERIALIZE_INTRINSIC(Samples).asInt();
        ClearColor.x = root.get("ClearColorX", "").asFloat();
        ClearColor.y = root.get("ClearColorY", "").asFloat();
        ClearColor.z = root.get("ClearColorZ", "").asFloat();
        ClearColor.w = root.get("ClearColorW", "").asFloat();
      }

    };

    /**************************************************************************/
    /*!
    @struct GraphicsConfig Configuration for Audio system.
    */
    /**************************************************************************/
    struct AudioConfig : public Configuration {
      AudioConfig();
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

    };

    /**************************************************************************/
    /*!
    @struct GUIConfig Configuration for GUI system.
    */
    /**************************************************************************/
    struct GUIConfig : public Configuration {
      GUIConfig();
      static std::string FileName() { return "ConfigurationGUI.cfg"; }
      void Serialize(Json::Value& root);
      void Deserialize(Json::Value& root);

      ImGuiStyle& Style; // A pointer to the ImGui Style
      std::string FontPath;
    };

    /**************************************************************************/
    /*!
    @struct DebugConfig Configuration for debugging system.
    */
    /**************************************************************************/
    struct DebugConfig : public Configuration {
      struct TraceInfo {
        bool Construction;
        bool Initialize;
        bool Update;
      };
      struct TraceTypes {
        TraceInfo GameObjects;
        TraceInfo Components;
        TraceInfo Events;
      };
      TraceTypes Tracing;
      DebugConfig();
      static std::string FileName() { return "ConfigurationDebug.cfg"; }
      void Serialize(Json::Value& root);
      void Deserialize(Json::Value& root);
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
      DebugConfig Debug;
      static ConfigurationFiles& Access();
    };


  }
}