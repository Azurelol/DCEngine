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

    void Copy(GameObject*);
    void Paste(Space*);
        
    std::deque<CommandPtr> CommandsCurrent;
    std::deque<CommandPtr> CommandsUndo;

  private:
    unsigned int Maximum;
    ArchetypePtr ObjectCopyData;
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

    CommandObjectCreation(GameObject* gameObject, Space* space, Setting setting);    
    CommandObjectCreation(ArchetypePtr copyData, Space* space);
    void Undo();
    void Execute();

    
    void Create();
    void Destroy();
    void Copy();

    Setting CurrentSetting;
    GameObject* GameObjectRef;
    Space* SpaceRef;
    ArchetypePtr GameObjectData;
  };  

  class ObjectCopy {
  public:
    void Set(GameObject* gameObject, Space* space);
    void Duplicate();

  private:
    GameObject* GameObjectRef;
    Space* SpaceRef;
    ArchetypePtr GameObjectData;
  };

  /*===================*
  *     Transform     *
  *===================*/
  namespace Components {
    class Transform;
  }  
  class CommandObjectTransform : public Command {
  public:
    CommandObjectTransform(Components::Transform* transform);
    void Undo();
    void Execute();
    
    void SaveNew(Vec3 transform, Vec3 rotation, Vec3 scale);
    void SaveNew(Components::Transform * transform);
    void SavePrevious(Vec3 transform, Vec3 rotation, Vec3 scale);
    void SavePrevious(Components::Transform * transform);
    // Object that was transformed
    Components::Transform* TransformedObject;
    // New Transform
    Vec3 NewTranslation;
    Vec3 NewRotation;
    Vec3 NewScale;
    // Previous Transform
    Vec3 PreviousTranslation;
    Vec3 PreviousRotation;
    Vec3 PreviousScale;
  };







}