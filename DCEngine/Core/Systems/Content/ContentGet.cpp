/*****************************************************************************/
/*!
@file   ContentGet.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Content.h" 
#include "../Filesystem/FileSystem.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Grabs a Bank resource.
    @return Returns a pointer to the bank object.
    */
    /**************************************************************************/
    BankPtr Content::getBank(const std::string bankName)
    {
      return MapBank.at(bankName);
    }

    /**************************************************************************/
    /*!
    @brief Grabs a shader resource.
    @return Returns a pointer to the shader object.
    */
    /**************************************************************************/
    ShaderPtr Content::getShader(const std::string shaderName)
    {
      return ShaderMap.at(shaderName);
    }

    /**************************************************************************/
    /*!
    @brief Grabs a font resource.
    @return Returns a pointer to the font object.
    */
    /**************************************************************************/
    FontPtr Content::getFont(const std::string & fontName)
    {
      // Check if the resource is present in the map
      if (!FontMap.count(fontName)) {
        // Return a default font
        return FontMap.at(DefaultFont);
      }

      return FontMap.at(fontName);
    }

    /**************************************************************************/
    /*!
    @brief Grabs a SpriteSource resource.
    @return Returns a pointer to the spritesource object.
    */
    /**************************************************************************/
    SpriteSourcePtr Content::getSpriteSrc(const std::string & spriteName)
    {
      // Check if the resource is present in the map
      if (!SpriteSourceMap.count(spriteName)) {
        //DCTrace << "Content::getSpriteSrc - " << spriteName << " was not found! Using default: \n";
        // Return a default '404 image not found.
        return SpriteSourceMap.at(DefaultImage);
      }

      //SpriteSourcePtr spriteSource(SpriteSourceMap.at(spriteName));
      //if (spriteSource == nullptr)
      //  throw DCException("Content::GetSpriteSource -" + spriteName + " was not found!");
      return SpriteSourceMap.at(spriteName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a SoundCue resource.
    @return Returns a pointer to the SoundCue object.
    */
    /**************************************************************************/
    SoundCuePtr Content::getSoundCue(const std::string & soundCueName)
    {
      // Check if the resource is present in the map
      if (!SoundCueMap.count(soundCueName)) {
        //DCTrace << "Content::getSpriteSrc - " << spriteName << " was not found! Using default: \n";
        // Return a default '404 image not found.
        //return SoundCueMap.at(DefaultSound);
        return nullptr;
      }
      return SoundCueMap.at(soundCueName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs an Archetype resource.
    @return Returns a pointer to the Archetype object.
    */
    /**************************************************************************/
    ArchetypePtr Content::getArchetype(const std::string & archetypeName)
    {
      return ArchetypeMap.at(archetypeName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a Level resource.
    @return Returns a pointer to the Level object.
    */
    /**************************************************************************/
    LevelPtr Content::getLevel(const std::string & levelName)
    {
      // Check if the resource is present in the map
      if (!LevelMap.count(levelName)) {
        DCTrace << "Content::getLevel - " << levelName << " was not found!\n";
        return nullptr;
      }
      // If it does, first load it
      LevelMap.at(levelName)->Load();
      // Then return it
      return LevelMap.at(levelName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a CollisionGroup resource.
    @return Returns a pointer to the CollisionGroup object.
    */
    /**************************************************************************/
    CollisionGroupPtr Content::getCollisionGroup(const std::string & groupName)
    {
      // Check if the resource is present in the map
      if (!MapCollisionGroup.count(groupName)) {
        // Return a default resource
        return MapCollisionGroup.at(DefaultCollisionGroup);
      }
      return MapCollisionGroup.at(groupName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a CollisionTable resource.
    @return Returns a pointer to the CollisionTable object.
    */
    /**************************************************************************/
    CollisionTablePtr Content::getCollisionTable(const std::string & table)
    {
      // Check if the resource is present in the map
      if (!MapCollisionTable.count(table)) {
        // Return a default resource
        return MapCollisionTable.at(DefaultCollisionTable);
      }
      return MapCollisionTable.at(table);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a PhysicsMaterial resource.
    @return Returns a pointer to the PhysicsMaterial object.
    */
    /**************************************************************************/
    PhysicsMaterialPtr Content::getPhysicsMaterial(const std::string & materialName)
    {
      return getResource<PhysicsMaterialMap, PhysicsMaterialPtr>(materialName, MapPhysicsMaterial, DefaultPhysicsMaterial);
    }

    SpriteLayerPtr Content::getSpriteLayer(const std::string & name)
    {
      return getResource<SpriteLayerMap, SpriteLayerPtr>(name, MapSpriteLayer, DefaultSpriteLayer);
    }

    TexturePtr Content::getTexture(const std::string & name)
    {
      return getResource<TextureMap, TexturePtr>(name, MapTextures, DefaultTexture);
    }

    SpriteLayerOrderPtr Content::getSpriteLayerOrder(const std::string & name)
    {
      return getResource<SpriteLayerOrderMap, SpriteLayerOrderPtr>(name, MapSpriteLayerOrder, DefaultSpriteLayerOrder);
    }

    /**************************************************************************/
    /*!
    @brief Returns pointers to the content maps.
    @return Returns a pointer to the SoundCue object.
    */
    /**************************************************************************/
    TextureMap * Content::AllTextures()
    {
      return &MapTextures;
    }

    SpriteSourceMap * Content::AllSpriteSources()
    {
      return &SpriteSourceMap;
    }

    SoundCueMap * Content::AllSoundCues()
    {
      return &SoundCueMap;
    }

    BankMap * Content::AllBanks()
    {
      return &MapBank;
    }

    ShaderMap * Content::AllShaders()
    {
      return &ShaderMap;
    }

    FontMap * Content::AllFonts()
    {
      return &FontMap;
    }

    ArchetypeMap * Content::AllArchetypes()
    {
      return &ArchetypeMap;
    }

    LevelMap * Content::AllLevels()
    {
      return &LevelMap;
    }

    ZilchScriptMap * Content::AllZilchScripts()
    {
      return &MapZilchScript;
    }

    CollisionGroupMap * Content::AllCollisionGroups()
    {
      return &MapCollisionGroup;
    }

    CollisionTableMap * Content::AllCollisionTables()
    {
      return &MapCollisionTable;
    }

    PhysicsMaterialMap * Content::AllPhysicsMaterials()
    {
      return &MapPhysicsMaterial;
    }

    SpriteLayerMap * Content::AllSpriteLayers()
    {
      return &MapSpriteLayer;
    }

    SpriteLayerOrderMap * Content::AllSpriteLayerOrders()
    {
      return &MapSpriteLayerOrder;
    }

  }
}