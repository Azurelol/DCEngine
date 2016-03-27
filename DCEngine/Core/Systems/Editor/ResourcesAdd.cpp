/******************************************************************************/
/*!
@file   ResourcesAdd.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/28/2015
@brief  Methods that add various resources to the currently active project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief  Adds a resource to the project by creating it. This is normally
            done for engine-specific resources.
    @param  name The name of the resource.
    @param  type The type of the resource.
    */
    /**************************************************************************/
    void Editor::ResourceCreate(std::string& name, ResourceType type) {
      DCTrace << "Editor::ResourceCreate - Creating " << name << "\n";

      switch (type) {

      case ResourceType::Level:
        CreateLevel(name);
        break;
        
      case ResourceType::CollisionGroup:
        CreateCollisionGroup(name);
        break;

      case ResourceType::CollisionTable:
        CreateCollisionTable(name);
        break;
        
      case ResourceType::PhysicsMaterial:
        CreatePhysicsMaterial(name);
        break;

      case ResourceType::ZilchScript:
        CreateZilchScript(name);
        break;

      case ResourceType::SpriteLayer:
        CreateSpriteLayer(name);
        break;

      case ResourceType::SpriteLayerOrder:
        CreateSpriteLayerOrder(name);
        break;

      default:
        break;

      }

      // Close the "Add Resource" window if the resource was successfully created.
      Windows.AddResourceEnabled = false;

    }

    /**************************************************************************/
    /*!
    @brief  Adds a resource to the project from a file. This is normally
            done for resources that use external assets.
    @param  name The name of the resource.
    @param  type The type of the resource.
    */
    /**************************************************************************/
    void Editor::ResourceAddFromFile(std::string & name, ResourceType type)
    {
      DCTrace << "Editor::ResourceAddFromFile - Attempting to create: " << name << "\n";

      // Open the file dialog by default on the project's resource path
      auto executablePath = boost::filesystem::initial_path().string();      
      auto resourcesPath = executablePath + "\\" + Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->AssetPath;
      FileSystem::CorrectPath(resourcesPath);      
      //auto a = boost::filesystem::
      //auto resourcesPath = executablePath + Settings.ProjectsPath + Settings.ProjectProperties->ResourcePath; // "\\Projects";
      // If the projects directory doesn't exist, start from the executable
      if (!FileSystem::Exists(resourcesPath)) {
        resourcesPath = executablePath;
      }
      // Grab the absolute path of the asset file selected
      std::string assetPath;      
      // Create the resource from file
      switch (type) {
        
      case ResourceType::Bank:
        assetPath = FileSystem::FileOpenDialog(resourcesPath, std::string("bank"));
        if (assetPath.empty())
          return;
        CreateBank(FileSystem::FileNoExtension(assetPath), assetPath);
        break;

      case ResourceType::SpriteSource:
        assetPath = FileSystem::FileOpenDialog(resourcesPath, std::string("png,jpg"));
        if (assetPath.empty())
          return;
        CreateSpriteSource(FileSystem::FileNoExtension(assetPath), assetPath);
        break;

      case ResourceType::SoundCue:
        assetPath = FileSystem::FileOpenDialog(resourcesPath, std::string("wav,mp3,ogg"));
        if (assetPath.empty())
          return;
        CreateSoundCue(FileSystem::FileNoExtension(assetPath), assetPath);
        break;

      case ResourceType::Font:
        assetPath = FileSystem::FileOpenDialog(resourcesPath, std::string("ttf"));
        if (assetPath.empty())
          return;
        CreateFont(FileSystem::FileNoExtension(assetPath), assetPath);
        break;

      }

      // Close the "Add Resource" window if the resource was successfully created.
      Windows.AddResourceEnabled = false;

    }

    /**************************************************************************/
    /*!
    @brief  Creates a level on the currently selected project
    @param  The name of the level to create.
    */
    /**************************************************************************/
    bool Editor::CreateLevel(std::string & name)
    {
      DCTrace << "Editor::CreateLevel - Creating " << name << "\n";
      // Save the current level, if there's one loaded
      if (CurrentSpace->CurrentLevel)
        SaveLevel(CurrentSpace->CurrentLevel->Name());
      // Destroy the objects in the space
      CurrentSpace->DestroyAll();
      // Add a default camera from the 'Camera' archetype
      CurrentSpace->CreateObject("Camera");      
      // Save the level
      SaveLevel(name);
      // Add the editor camera right away
      SetEditorCamera(true);
      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Bank on the currently selected project.
    @param  The name of the bank to create.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateBank(std::string & name, std::string& assetPath)
    {
      DCTrace << "Editor::CreateBank - Creating " << name << "\n";
      auto path = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath
                                                    + name + Bank::Extension();
      auto resource = BankPtr(new Bank(path));
      // Save its asset path
      resource->setAssetPath(assetPath);
      Daisy->getSystem<Content>()->AddBank(name, resource);
      // Serialize it and save it to file
      resource->Build();
      // Load its textures immediately
      resource->Add();
      return resource.get();

    }

    /**************************************************************************/
    /*!
    @brief  Creates a CollisionGroup on the currently selected project
    @param  The name of the group to create.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateCollisionGroup(std::string & name)
    {
      DCTrace << "Editor::CreateCollisionGroup - Creating " << name << "\n";
      auto collisionGroupPath = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath 
                                + name + CollisionGroup::Extension();
      auto collisionGroup = CollisionGroupPtr(new CollisionGroup(collisionGroupPath));
      Daisy->getSystem<Content>()->AddCollisionGroup(name, collisionGroup);
      // Serialize it and save it to file
      collisionGroup->Build();


      return collisionGroup.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a CollisionTable on the currently selected project
    @param  The name of the group to create.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateCollisionTable(std::string & name)
    {
      DCTrace << "Editor::CreateCollisionTable - Creating " << name << "\n";
      auto collisionTablePath = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath
                                + name + CollisionTable::Extension();
      auto collisionTable = CollisionTablePtr(new CollisionTable(collisionTablePath));
      Daisy->getSystem<Content>()->AddCollisionTable(name, collisionTable);      
      // Serialize it and save it to file
      collisionTable->Build();

      return collisionTable.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a PhysicsMaterial on the currently selected project
    @param  name The name of the resource to create.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreatePhysicsMaterial(std::string & name)
    {
      DCTrace << "Editor::CreatePhysicsMaterial - Creating " << name << "\n";
      auto path = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath
                  + name + PhysicsMaterial::Extension();
      auto resource = PhysicsMaterialPtr(new PhysicsMaterial(path));
      Daisy->getSystem<Content>()->AddPhysicsMaterial(name, resource);
      // Serialize it and save it to file
      resource->Build();
      return resource.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SpriteSource resource from a given name and the
            path to where the image file lies.
    @param  name The name of the SpriteSource.
    @param  assetPath The path to the SpriteSource.
    @return A pointer to the recently-created resource object.
    @note   The SpriteSource will be created with the name of the file, not
            the name selected.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateSpriteSource(std::string & name, std::string & assetPath)
    {
      DCTrace << "Editor::CreateSpriteSource - Created '" << name << "' with asset: '" << assetPath << "' \n";  
      // Create a SpriteSource object
      auto spriteSourcePath = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath + name + SpriteSource::Extension();
      auto spriteSource = SpriteSourcePtr(new SpriteSource(spriteSourcePath));
      // Store the path of its asset
      spriteSource->setAssetPath(assetPath);
      // Serialize it and save it to file
      auto data = spriteSource->Build();
      // Add it to the content system
      
      Daisy->getSystem<Content>()->AddSpriteSource(name, spriteSource);
      // Load its textures immediately
      spriteSource->LoadTexture();

      return spriteSource.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SoundCue resource from a given name and the
            path to where the sound file lies.
    @param  name The name of the SoundCue.
    @param  assetPath The path to the SoundCue.
    @return A pointer to the recently-created resource object.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateSoundCue(std::string & name, std::string & assetPath)
    {
      DCTrace << "Editor::CreateSoundCue - Created '" << name << "' with asset: '" << assetPath << "' \n";
      // Create a SoundCue object
      auto soundCuePath = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath + name + SoundCue::Extension();
      auto soundCue = SoundCuePtr(new SoundCue(soundCuePath, SoundCue::SoundCueType::File));
      // Store the path of its asset
      soundCue->setAssetPath(assetPath);
      // Serialize it and save it to file
      auto data = soundCue->Build();
      // Add it to the content system
      Daisy->getSystem<Content>()->AddSoundCue(name, soundCue);
      // Generate the SoundCue immediately
      soundCue->Generate();

      return soundCue.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a ZilchScript on the currently selected project
    @param  The name of the script to create.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateZilchScript(std::string & name)
    {
      DCTrace << "Editor::CreateZilchScript - Creating " << name << "\n";
      auto path = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath
                  + name + ZilchScript::Extension();
      FileSystem::CorrectPath(path);
      auto script = ZilchScriptPtr(new ZilchScript(path));
      
      // Build the script from a template
      if (script->BuildDefaultFromTemplate()) {
        // If successful, open the default text editor and read it!
        TextEditor.Load(script);
        //std::string command = "notepad++ " + path;
        //system(command.c_str());
      }     
      
      // Add the script to the content system
      Daisy->getSystem<Content>()->AddZilchScript(name, script);      

      return script.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Font resource from a given name and the
            path to where the font file lies.
    @param  name The name of the Font.
    @param  assetPath The path to the Font.
    @return A pointer to the recently-created resource object.
    @note   The Font will be created with the name of the file, not
            the name selected.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateFont(std::string & name, std::string & assetPath)
    {
      DCTrace << "Editor::CreateFont - Created '" << name << "' with asset: '" << assetPath << "' \n";
      // Create a SpriteSource object
      auto path = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath + name + Font::Extension();
      auto font = FontPtr(new Font(path));
      // Store the path of its asset
      font->setAssetPath(assetPath);
      // Serialize it and save it to file
      auto data = font->Build();
      // Add it to the content system
      Daisy->getSystem<Content>()->AddFont(name, font);
      // Load its textures immediately
      font->Load();

      return font.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SpriteLayer resource from a given name.
    @param  name The name of the SpriteLayer.
    @return A pointer to the recently-created resource object.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateSpriteLayer(std::string & name)
    {
      DCTrace << "Editor::CreateSpriteLayer - Creating " << name << "\n";
      auto path = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath
                                            + name + SpriteLayer::Extension();
      auto resource = SpriteLayerPtr(new SpriteLayer(path));
      Daisy->getSystem<Content>()->AddSpriteLayer(name, resource);
      // Serialize it and save it to file
      resource->Build();
      return resource.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SpriteLayerOrder resource from a given name.
    @param  name The name of the SpriteLayerOrder.
    @return A pointer to the recently-created resource object.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateSpriteLayerOrder(std::string & name)
    {
      DCTrace << "Editor::CreateSpriteLayerOrder - Creating " << name << "\n";
      auto path = Settings.ProjectProperties->ProjectPath + Settings.ProjectProperties->ResourcePath
                  + name + SpriteLayerOrder::Extension();
      auto resource = SpriteLayerOrderPtr(new SpriteLayerOrder(path));
      Daisy->getSystem<Content>()->AddSpriteLayerOrder(name, resource);
      // Serialize it and save it to file
      resource->Build();
      return resource.get();
    }



  }

}
