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
#include "../../Engine/Data.h"

namespace DCEngine {

  class Engine;
  class Factory;

  namespace Systems {

    //using SoundCueMap = std::map<std::string, SoundCuePtr>;
    using ResourceMap = std::map<std::string, std::string>;    
    using SpriteSourceMap = std::map<std::string, SpriteSourcePtr>;
    using SoundCueMap = std::map<std::string, SoundCuePtr>;
    using ShaderMap = std::map<std::string, ShaderPtr>;
    
    class Content : public System {
      friend class Engine;
      friend class Factory;
    public:    
      void LoadProjectResources(); //!< Load resources from a project.
      
      /* Getters */
      ShaderPtr getShader(std::string shaderName);
      FontPtr getFont(std::string& fontName);
      SpriteSourcePtr getSpriteSrc(std::string& spriteName);
      SoundCuePtr getSoundCue(std::string& soundCueName);

      SpriteSourceMap* AllSpriteSources();
      SoundCueMap* AllSoundCues();
      ShaderMap* AllShaders();

      //template <typename ResourceClass> getResource(); //!< Returns
    private:
      
      /* Data */
      std::string CoreAssetsPath;
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
      Content(std::string& coreAssetsPath);
      void Initialize();
      void Update(float dt);
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

      /* Loading functions */
      void LoadCoreAssets(); //!< Load default content files for the engine.        
      void LoadProjectAssets(); //!< Load the assets used by the loaded project.      
      void LoadAllResources();

      void LoadProjectData(std::string&); //!<             
    };
  }

}

