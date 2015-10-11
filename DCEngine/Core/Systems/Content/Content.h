/*****************************************************************************/
/*!
\file   Content.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  The Content system handles the loading of all content data required
        for a project. It stores references to the names of these resources
        as well as their file locations. The deserialization of these files
        is handled by the factory system.
*/
/******************************************************************************/
#pragma once
#include "../System.h"
#include "../../Resources/ResourcesInclude.h"
#include "../../Engine/ProjectData.h"

namespace DCEngine {

  class Engine;
  class Factory;

  namespace Systems {

    //using SoundCueMap = std::map<std::string, SoundCuePtr>;
    using ResourceMap = std::map<std::string, std::string>;    

    class Content : public System {
      friend class Engine;
      friend class Factory;
    public:    
      void LoadProjectResources(); //!< Load resources from a project.
      
      /* Getters */
      ShaderPtr getShader(std::string& shaderName);
      FontPtr getFont(std::string& fontName);
      SpriteSourcePtr getSpriteSrc(std::string& spriteName);
      SoundCuePtr getSoundCue(std::string& soundCueName);

      //template <typename ResourceClass> getResource(); //!< Returns
    private:
      
      /* Data */
      EngineDataPtr EngineInfo;
      ProjectDataPtr ProjectInfo;

      /* Resource maps */
      std::map<std::string, SoundCuePtr> SoundCueMap;
      std::map<std::string, ShaderPtr> ShaderMap;
      std::map<std::string, FontPtr> FontMap;
      std::map<std::string, SpriteSourcePtr> SpriteSourceMap;

      /* Map functions */
      void AddFont(std::string& fontName, FontPtr fontPtr);
      void AddShader(std::string& shaderName, ShaderPtr shaderPtr);
      void AddSpriteSource(std::string& spriteSourceName, SpriteSourcePtr spriteSourcePtr);
      void AddSoundCue(std::string& soundCueName, SoundCuePtr soundcuePtr);      

      /* Core functions */
      Content();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

      /* Loading functions */
      void LoadCoreAssets(); //!< Load default content files for the engine.        
      void LoadEngineData();
      void LoadProjectData(); //!< 

      void DeserializeProjectData(Json::Value& root); //!< Deserializes the loaded project's data.
      void LoadProjectAssets(); //!< Load the assets used by the loaded project.
      
      void LoadAllResources();

      void LoadArchetypes(); //!< Load archetypes from a project.
      void LoadSpriteSources(); //!< Load spritesources from a project

      void ScanFolder();
      void LoadFile();
    };
  }

}

