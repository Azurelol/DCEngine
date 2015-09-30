#include "Content.h" 

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Constructor for the Content system.
    */
    /**************************************************************************/
    Content::Content() : System(std::string("ContentSystem"), EnumeratedSystem::Content) {
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

      // Load the default resources of the engine's
      LoadDefaultResources();
    }

    /**************************************************************************/
    /*!
    @brief Loads the default resources from the engine.
    @note  Currently only generating the first 128 characters of the ASCII
    character set.
    */
    /**************************************************************************/
    void Content::LoadDefaultResources()
    {
      trace << "\n[Content::LoadDefaultResources] - Loading default resources \n";
      // Deserialize the engine's internal resource list

      // Load default shaders
      AddShader(std::string("SpriteShader"), ShaderPtr(new Shader(std::string("SpriteShader"), 
                                                                  "SpriteShader.vs", "SpriteShader.frag")));
      AddShader(std::string("SpriteTextShader"), ShaderPtr(new Shader(std::string("SpriteTextShader"),
                                                                  "SpriteTextShader.vs", "SpriteTextShader.frag")));

      // Load default sprites

      // Load default fonts      
      AddFont(std::string("Verdana"), FontPtr(new Font("Verdana.ttf")));

      // Characters.insert(std::pair<GLchar, Character>(c, character));

      // Load default soundcues
      trace << "[Content::LoadDefaultResources] - Finished loading default resources \n\n";
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
      fontPtr->Load();
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

  }
}
