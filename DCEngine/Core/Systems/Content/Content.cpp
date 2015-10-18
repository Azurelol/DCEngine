#include "Content.h" 
#include "FileSystem.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Constructor for the Content system.
    */
    /**************************************************************************/
    Content::Content() : System(std::string("ContentSystem"), EnumeratedSystem::Content) {
      ProjectInfo.reset(new ProjectData());
      EngineInfo.reset(new ProjectData());
    }

    /**************************************************************************/
    /*!
    @brief Initializes the Content system.
    @note  
    */
    /**************************************************************************/
    void Content::Initialize() {
      if (TRACE_INITIALIZE)
        trace << "Content::Initialize \n";

      // Load the engine's default data
      LoadEngineData(std::string("Core/Daisy.dcp"));

      // Load the default resources of the engine's
      LoadCoreAssets();
    }

    /**************************************************************************/
    /*!
    @brief Loads the resources in every map.
    @note  Some resources need to be loaded after the systems they are used for
           have already been initialized.
    */
    /**************************************************************************/
    void Content::LoadAllResources()
    {
      // Load every SoundCue's sound
      for (auto soundCue : SoundCueMap) {
        soundCue.second->Load();
      }

      // Load every SpriteSource's texture
      for (auto texture : SpriteSourceMap) {
        texture.second->LoadTexture();
      }

      // Load every Font
      for (auto font : FontMap) {
        font.second->Load();
      }

    }

    /**************************************************************************/
    /*!
    @brief Deserializes an EngineData file for engine data settings.
    */
    /**************************************************************************/
    void Content::LoadEngineData(std::string& engineData)
    {
      

      // NOT REALLY LOADING, IS IT?
      EngineInfo->SpritePath = "Core/Assets/Sprites/";
      EngineInfo->ShaderPath = "Core/Assets/Shaders/";
      EngineInfo->FontPath = "Core/Assets/Fonts/";
      EngineInfo->SoundPath = "Core/Assets/Sounds/";
      
      trace << "Content::LoadEngineData - Finished loading all engine data. \n";

      
    }

    /**************************************************************************/
    /*!
    @brief Deserializes a ProjectData file for project data settings.
    */
    /**************************************************************************/
    void Content::LoadProjectData(std::string& projectData)
    {

      //auto test = DirectoryExtractFilePaths(std::string("Core/Assets"));

      // Load the loaded project's assets
      LoadProjectAssets();

      trace << "Content::LoadProjectData - Finished loading all project data. \n";
    }

    void Content::DeserializeProjectData(Json::Value& root)
    {
      // Deserialize all of the project file's data into the
      // project data struct.
      ProjectInfo->ProjectName = root.get("ProjectName", "").asString();

      ProjectInfo->AssetPath= root.get("AssetFolder", "").asString();
      ProjectInfo->ArchetypePath = root.get("ArchetypeFolder", "").asString();
      ProjectInfo->LevelPath = root.get("LevelFolder", "").asString();
    }

    /**************************************************************************/
    /*!
    @brief Loads the default resources from the engine.
    @note  Currently only generating the first 128 characters of the ASCII
    character set.
    */
    /**************************************************************************/
    void Content::LoadCoreAssets()
    {
      trace << "\n[Content::LoadDefaultResources] - Loading default resources \n";
      // Deserialize the engine's core assets file

      // Scan the specified folder and recursively add each resource to its appropiate
      // map.

      //auto a = FileSystem::FileFound(EngineInfo->SpritePath + "square2.png");
      //auto b = FileSystem::FileFound(EngineInfo->SpritePath + "square3.png");
      //std::vector<std::string> files;
      //auto c = FileSystem::DirectoryExtractFilePaths(EngineInfo->ShaderPath, files);
      //std::string contentRead;
      //auto d = FileSystem::FileReadToString(EngineInfo->ShaderPath + "SpriteShader.vs", contentRead);
      //auto f = FileSystem::FileWriteString("hey2.txt", contentRead);


      //std::string modifyData;
      //auto e = FileSystem::FileLastModified("hey2.txt", modifyData);

      //auto g = FileSystem::FileRename("hey2.txt", "lol3.txt");

      //std::string filePathy;
      //auto h = FileSystem::DirectoryFindFile("Debug.cpp", "Core", filePathy);

      //FileSystem::Remove("lol3.txt");

      // Load default shaders
      AddShader(std::string("SpriteShader"), ShaderPtr(new Shader(std::string("SpriteShader"), 
        EngineInfo->ShaderPath + "SpriteShader.vs", 
        EngineInfo->ShaderPath + "SpriteShader.frag")));
      AddShader(std::string("SpriteTextShader"), ShaderPtr(new Shader(std::string("SpriteTextShader"),
        EngineInfo->ShaderPath + "SpriteTextShader.vs",
        EngineInfo->ShaderPath + "SpriteTextShader.frag")));

      // Load default sprites
      AddSpriteSource(std::string("Square"), SpriteSourcePtr(new SpriteSource(EngineInfo->SpritePath + "square2.png")));
      AddSpriteSource(std::string("Angryeyes"), SpriteSourcePtr(new SpriteSource(EngineInfo->SpritePath + "angryeyes.png")));
      AddSpriteSource(std::string("Awesomeface"), SpriteSourcePtr(new SpriteSource(EngineInfo->SpritePath + "awesomeface.png")));
      AddSpriteSource(std::string("KnightAnim"), SpriteSourcePtr(new SpriteSource(EngineInfo->SpritePath + "KnightAnimation.png")));
      // Load default fonts      
      AddFont(std::string("Verdana"), FontPtr(new Font(EngineInfo->FontPath + "Verdana.ttf")));
      // Load default soundcues
      AddSoundCue(std::string("SpaceJam"), SoundCuePtr(new SoundCue(EngineInfo->SoundPath + "spacejam.mp3")));


      trace << "[Content::LoadDefaultResources] - Finished loading default resources \n\n";
    }

    void Content::LoadProjectAssets()
    {

    }

    /**************************************************************************/
    /*!
    @brief Grabs a shader resource.
    @return Returns a pointer to the shader object.
    */
    /**************************************************************************/
    ShaderPtr Content::getShader(std::string & shaderName)
    {
      return ShaderMap.at(shaderName); 
    }

    /**************************************************************************/
    /*!
    @brief Grabs a font resource.
    @return Returns a pointer to the font object.
    */
    /**************************************************************************/
    FontPtr Content::getFont(std::string & fontName)
    {
      return FontMap.at(fontName);
    }

    /**************************************************************************/
    /*!
    @brief Grabs a SpriteSource resource.
    @return Returns a pointer to the spritesource object.
    */
    /**************************************************************************/
    SpriteSourcePtr Content::getSpriteSrc(std::string & spriteName)
    {
      return SpriteSourceMap.at(spriteName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a SoundCue resource.
    @return Returns a pointer to the SoundCue object.
    */
    /**************************************************************************/
    SoundCuePtr Content::getSoundCue(std::string & soundCueName)
    {
      return SoundCueMap.at(soundCueName);
    }


    /**************************************************************************/
    /*!
    @brief Updates the Content system.
    @note 
    */
    /**************************************************************************/
    void Content::Update(float dt) {
      // On every update, scan for resources being changed outside the application. (Hot loading?)
    }

    /**************************************************************************/
    /*!
    @brief Terminates the Content system.
    @note
    */
    /**************************************************************************/
    void Content::Terminate() {
      trace << "Content::Terminate\n";
    }

    void Content::Serialize(Json::Value & root)
    {
    }

    void Content::Deserialize(Json::Value & root)
    {
    }

    /**************************************************************************/
    /*!
    @brief Adds a Font to the font resource map.
    @param The name of the Font.
    @param The pointer to the Font resource.
    @note  We load the font immediately so its ready for use by SpriteText
           component.
    */
    /**************************************************************************/
    void Content::AddFont(std::string & fontName, FontPtr fontPtr)
    {
      FontMap.insert(std::pair<std::string, FontPtr>(fontName, fontPtr));
      trace << "Content::AddFont - " << fontName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a shader to the shader resource map.
    @param The name of the shader.
    @param The pointer to the shader resource.
    */
    /**************************************************************************/
    void Content::AddShader(std::string & shaderName, ShaderPtr shaderPtr)
    {
      ShaderMap.insert(std::pair<std::string, ShaderPtr>(shaderName, shaderPtr));
      trace << "Content::AddShader - " << shaderName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a spritesource to the spritesource resource map.
    @param The name of the spritesource.
    @param The pointer to the spritesource resource.
    */
    /**************************************************************************/
    void Content::AddSpriteSource(std::string & spriteSourceName, SpriteSourcePtr spriteSourcePtr)
    {
      SpriteSourceMap.insert(std::pair<std::string, SpriteSourcePtr>(spriteSourceName, spriteSourcePtr));
      trace << "Content::AddSpriteSource - " << spriteSourceName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a soundcue resource to the soundcue resource map.
    @param The name of the soundcue.
    @param The pointer to the soundcue resource.
    */
    /**************************************************************************/
    void Content::AddSoundCue(std::string & soundCueName, SoundCuePtr soundcuePtr)
    {
      SoundCueMap.insert(std::pair<std::string, SoundCuePtr>(soundCueName, soundcuePtr));
      trace << "Content::AddSoundCue - " << soundCueName << " was added.\n";
    }

  }
}
