/*****************************************************************************/
/*!
\file   Resource.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  Resources are the source data of a game. They are assets such as textures,
        sounds, levels, scripts. They are the resources found in a library window.

        Each resource is assigned an unique id when created, allowing objects
        and archetypes to refer to resources and not break their connection
        even if the name of the resource is changed.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.


*/
/******************************************************************************/
#pragma once
#include "Object.h"
#include "../Systems/Filesystem/FileSystem.h"

namespace DCEngine {

  class Resource : public Object {
  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(Resource, Object);
    #endif

    // Properties
    DCE_DEFINE_PROPERTY(std::string, ResourcePath);
    // Methods
    Resource(std::string type, std::string name, std::string resourcePath);
    void Serialize(Zilch::JsonBuilder& builder);
    void Deserialize(Zilch::JsonValue* properties);
    std::string Build();
    virtual void Save() {}
    virtual void Save(std::string& serializedData);
    virtual bool Load();
    bool Destroy();

  protected:    
    std::string ResourceType;
    std::string ResourcePath;    
    std::string SerializedData;

    //const std::string Extension;
    //virtual Resource* Find(std::string) = 0;
  };

  using ResourcePtr = Resource*;
  using ResourceStrongPtr = std::shared_ptr<Resource>;

}