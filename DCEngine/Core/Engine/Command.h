/*****************************************************************************/
/*!
@file   Command.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/25/2015
@brief  The Command Class is used by the editor to store a list of all
        recently given commands to the Editor. Such as deleting, moving,
        creating objects, etc.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include <deque>

#include "../Resources/Archetype.h"

namespace DCEngine {
  
  class Command {
  public:    
    Command(std::string name) : CommandName(name) {}
    std::string CommandName;
        
    virtual void Undo() = 0;
    virtual void Execute() = 0;    
  };
  using CommandPtr = std::shared_ptr<Command>;

  // Forward declarations
  class GameObject;
  using GameObjectPtr = GameObject*;
  using GameObjectRawVec = std::vector<GameObjectPtr>;
  class Space;
  class Archetype;
  /*===================*
  *     Manager       *
  *===================*/
  class CommandManager {
  public:
    CommandManager();
    void Add(CommandPtr command);
    void Undo();
    void Redo();

    void Copy(GameObjectRawVec);
    void Paste(Space*);
        
    std::deque<CommandPtr> CommandsCurrent;
    std::deque<CommandPtr> CommandsUndo;

  private:
    unsigned int Maximum;
    ArchetypeContainer ObjectCopyData;
  };

  /*===================*
  *     Creation    *
  *===================*/
  class CommandObjectCreation : public Command {
  public:

    enum class Setting {
      Create,
      Destroy,
    };

    CommandObjectCreation(GameObjectPtr gameObject, Space* space, Setting setting);
    CommandObjectCreation(GameObjectRawVec gameObjects, Space* space, Setting setting);    
    CommandObjectCreation(ArchetypeContainer copyData, Space* space);
    void Undo();
    void Execute();

    
    void Create();
    void Destroy();
    void Copy();

    Setting CurrentSetting;
    Space* SpaceRef;
    GameObjectRawVec GameObjectReferences;
    ArchetypeContainer GameObjectData;
  };  

  class ObjectCopy {
  public:
    void Set(GameObject* gameObject, Space* space);
    void Duplicate();

  private:
    GameObjectRawVec GameObjectReferences;
    Space* SpaceRef;
    ArchetypePtr GameObjectData;
  };

  /*===================*
  *     Transform     *
  *===================*/
  //namespace Components {
  //  class Transform;
  //}  
  //#include "../Components/Transform.h"
  class CommandObjectTransform : public Command {
  public:

    CommandObjectTransform(TransformDataPairVec& previous, TransformDataPairVec& current);
    void Undo();
    void Execute();
    void ApplyTransform(TransformDataPairVec& transformData);   


    // Containers of pairs of Transform components and their transform data
    TransformDataPairVec PreviousTransforms;
    TransformDataPairVec CurrentTransforms;    
  };







}