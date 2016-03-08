/******************************************************************************/
/*!
\file   ImGuiSFML.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   10/23/2015
\brief  An SFML backend for ImGui.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "ImGuiSFML.h"

// Access to the Window System
#include "../../Engine/Engine.h"
#include "../../Debug/DebugGraphics.h"

#define COMPILE_SHADER_MANUALLY 0
#define GENERATE_FONT_TEXTURE_MANUALLY 1

namespace DCEngine {
  namespace Systems {

    /* Statics used by ImGui */
    static GLuint g_FontTexture = 0;
    static int ShaderHandle = 0, VertexHandle = 0, FragHandle = 0;
    static int AttribLocationTex = 0, AttribLocationProjMtx = 0;
    static int AttribLocationPosition = 0, AttribLocationUV = 0, AttribLocationColor = 0;
    static unsigned int VBOHandle = 0, VAOHandle = 0, ElementsHandle = 0;
    
    /**************************************************************************/
    /*!
    \brief  Constructor.
    */
    /**************************************************************************/
    ImGuiSFML::ImGuiSFML()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Initializes the ImGui handler.
    */
    /**************************************************************************/
    void ImGuiSFML::Initialize()
    {      
      if (TRACE_INITIALIZE)
        DCTrace << "GUI::Initialize \n";
      
      // Save a pointer to the engine's window context
      WindowContext = Daisy->getSystem<Systems::Window>()->WindowHandler->GetWindow();

      // Setup ImGui bindng
      ImGuiSFMLInitialize(WindowContext, true);
    }
    
    /**************************************************************************/
    /*!
    @brief  Updates ImGui.
    @todo   If there's issues, move after the graphics update?
    */
    /**************************************************************************/
    void ImGuiSFML::StartFrame()
    {      
      //PollEvents();
      // 3. Have ImGui start a new frame.
      ImGui::NewFrame();
    }

    /**************************************************************************/
    /*!
    @brief  Poll for input events in SFML for ImGui.
    @todo   If there's issues, move after the graphics update?
    */
    /**************************************************************************/
    void ImGuiSFML::PollEvents()
    {
      //sf::Event& event = Daisy->getSystem<Input>()->InputHandler->_event;
      //while (WindowContext->pollEvent(event))
      //  ImGuiSFMLProcessEvent(event);
      // 2. Update ImGui after having polled for events
      ImGuiSFMLEventsUpdate();
    }

    /**************************************************************************/
    /*!
    @brief  Update.
    @todo   If there's issues, move after the graphics update?
    */
    /**************************************************************************/
    void ImGuiSFML::Render()
    {
      ImGui::Render();
    }

    /**************************************************************************/
    /*!
    \brief  Terminates ImGui.
    */
    /**************************************************************************/
    void ImGuiSFML::Terminate()
    {
      ImGuiSFMLTerminate();
    }


    /**************************************************************************/
    /*!                    ImGui Implementation Functions
    */
    /**************************************************************************/
    /**************************************************************************/
    /*!
    @brief  Initializes ImGui.
    @param  windowContext The Window Context used by SFML.
    @param  Whether to initialize callbacks?
    */
    /**************************************************************************/
    IMGUI_API bool ImGuiSFML::ImGuiSFMLInitialize(sf::Window * windowContext, bool installCallbacks)
    {
      // Genereate the objects used by ImGui
      ImGuiSFMLCreateDeviceObjects();

      // Bind ImGui to SFML input events
      ImGuiSFMLBindEvents();
      
      // Initialize the rendering functions
      ImGuiSFMLInitializeRendering();
      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Compiles stuff for ImGui.
    @note   Currently done through sf::Texture.
    */
    /**************************************************************************/
    IMGUI_API bool ImGuiSFML::ImGuiSFMLCreateDeviceObjects()
    {
      // 1. Backup the current OpenGL state
      GLint lastTexture, lastArrayBuffer, lastVertexArray;
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);
      
      // 2. Store a handle to the shader program used by ImGui
            
      if (COMPILE_SHADER_MANUALLY) {
        // 2. Compile a shader
        const GLchar *vertex_shader =
          "#version 330\n"
          "uniform mat4 ProjMtx;\n"
          "in vec2 Position;\n"
          "in vec2 UV;\n"
          "in vec4 Color;\n"
          "out vec2 Frag_UV;\n"
          "out vec4 Frag_Color;\n"
          "void main()\n"
          "{\n"
          "	Frag_UV = UV;\n"
          "	Frag_Color = Color;\n"
          "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
          "}\n";

        const GLchar* fragment_shader =
          "#version 330\n"
          "uniform sampler2D Texture;\n"
          "in vec2 Frag_UV;\n"
          "in vec4 Frag_Color;\n"
          "out vec4 Out_Color;\n"
          "void main()\n"
          "{\n"
          "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
          "}\n";

        ShaderHandle = glCreateProgram();
        VertexHandle = glCreateShader(GL_VERTEX_SHADER);
        FragHandle = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(VertexHandle, 1, &vertex_shader, 0);
        glShaderSource(FragHandle, 1, &fragment_shader, 0);

        glCompileShader(VertexHandle);
        glCompileShader(FragHandle);

        glAttachShader(ShaderHandle, VertexHandle);
        glAttachShader(ShaderHandle, FragHandle);
        glLinkProgram(ShaderHandle);
      }
      else {
        GUIShader = Daisy->getSystem<Content>()->getShader("GUIShader");
        GUIShader->Compile();
        ShaderHandle = GUIShader->Get();
      }

      // 3.  Save handles to the uniforms
      AttribLocationTex = glGetUniformLocation(ShaderHandle, "Texture");
      AttribLocationProjMtx = glGetUniformLocation(ShaderHandle, "ProjMtx");      
      AttribLocationPosition = glGetAttribLocation(ShaderHandle, "Position");
      AttribLocationUV = glGetAttribLocation(ShaderHandle, "UV");
      AttribLocationColor = glGetAttribLocation(ShaderHandle, "Color");

      glGenBuffers(1, &VBOHandle);
      glGenBuffers(1, &ElementsHandle);

      glGenVertexArrays(1, &VAOHandle);
      glBindVertexArray(VAOHandle);
      glBindBuffer(GL_ARRAY_BUFFER, VBOHandle);
      glEnableVertexAttribArray(AttribLocationPosition);
      glEnableVertexAttribArray(AttribLocationUV);
      glEnableVertexAttribArray(AttribLocationColor);

      #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
      glVertexAttribPointer(AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
      glVertexAttribPointer(AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
      glVertexAttribPointer(AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
      #undef OFFSETOF
      
      // 4. Generate font textures
      ImGuiSFMLGenerateFontTexture();

      // 5. Restores the modified OpenGL state
      glBindTexture(GL_TEXTURE_2D, lastTexture);
      glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
      glBindVertexArray(lastVertexArray);

      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Generates a font Texture and binds it to ImGui.
    @note   Currently done through sf::Texture.
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLGenerateFontTexture()
    {
      ImGuiIO& io = ImGui::GetIO();

      // Generate an OpenGL texture
      unsigned char* pixels;
      int width, height;      

      // Roll our own
      if (GENERATE_FONT_TEXTURE_MANUALLY) {
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        //io.Fonts->AddFontFromFileTTF("DroidSand.ttf", FileSystem::FileSize("DroidSand.ttf"));
        glGenTextures(1, &g_FontTexture);
        glBindTexture(GL_TEXTURE_2D, g_FontTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      
        io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;
      }
      // sf::Texture
      else {
        io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
        FontTexture.create(width, height);
        FontTexture.update(pixels);
        io.Fonts->TexID = (void*)&FontTexture;
      }

      // Load TTF FILE?
      //io.Fonts->AddFontFromFileTTF("DroidSand.ttf");
      
      io.Fonts->ClearInputData();
      io.Fonts->ClearTexData();


    }



    /**************************************************************************/
    /*!
    @brief  Initializes ImGui's rendering pipeline.
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLInitializeRendering()
    {
      // Grab a reference to the input output
      ImGuiIO& io = ImGui::GetIO();
      // Sets the initial display size
      io.DisplaySize = ImVec2(static_cast<float>(WindowContext->getSize().x), 
                              static_cast<float>(WindowContext->getSize().y));
      //io.DisplayFramebufferScale = ImVec2(static_cast<float>(WindowContext->getSize().x),
      //                               static_cast<float>(WindowContext->getSize().y));
      //io.DisplayFramebufferScale = ImVec2(static_cast<float>())
      
      // Bind our implemented 'RenderDrawLists' function
      io.RenderDrawListsFn = ImGuiSFMLRenderDrawLists;
    }

    /**************************************************************************/
    /*!
    \brief  Binds SFML input to ImGui.
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLBindEvents()
    {
      ImGuiIO& io = ImGui::GetIO();

      io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
      io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
      io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
      io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
      io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
      io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
      io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
      io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
      io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
      io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
      io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
      io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
      io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
      io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
      io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
      io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
      io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;
      TimeElapsed.restart();
    }

    /**************************************************************************/
    /*!
    \brief  Updates ImGui.
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLEventsUpdate()
    {
      ImGuiIO& io = ImGui::GetIO();

      // Update display size
      //io.DisplaySize = ImVec2(WindowContext->getSize().x, WindowContext->getSize().y);
      //io.DisplayFramebufferScale = ImVec2(WindowContext->getSize().x, WindowContext->getSize().y);

      // Setup display size (every frame to accomodate for window resizing)
      //int width, height;
      //int displayWidth, displayHeight;
      io.DisplaySize = ImVec2(static_cast<float>(WindowContext->getSize().x),
                              static_cast<float>(WindowContext->getSize().y));

      


      // Update time step
      static double time = 0.0f;
      const double currentTime = TimeElapsed.getElapsedTime().asSeconds();
      io.DeltaTime = static_cast<float>(currentTime - time);
      time = currentTime;

      // Update inputs
      sf::Vector2i mousePos = sf::Mouse::getPosition(*WindowContext);
      io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
      io.MouseDown[0] = MousePressed[0] || sf::Mouse::isButtonPressed(sf::Mouse::Left);
      io.MouseDown[1] = MousePressed[1] || sf::Mouse::isButtonPressed(sf::Mouse::Right);

    }

    /**************************************************************************/
    /*!
    @brief  Process input events into ImGui.
    @param  A reference to the event object.
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLProcessEvent(sf::Event & event)
    {
      switch (event.type) {
      case sf::Event::MouseButtonPressed:
      {
        MousePressed[event.mouseButton.button] = true;
        break;
      }
      case sf::Event::MouseButtonReleased:
      {
        MousePressed[event.mouseButton.button] = false;
        break;
      }
      case sf::Event::MouseWheelMoved:
      {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += (float)event.mouseWheel.delta;
        break;
      }
      case sf::Event::KeyPressed:
      {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.key.code] = true;
        io.KeyCtrl = event.key.control;
        io.KeyShift = event.key.shift;
        break;
      }
      case sf::Event::KeyReleased:
      {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.key.code] = false;
        io.KeyCtrl = event.key.control;
        io.KeyShift = event.key.shift;
        break;
      }
      case sf::Event::TextEntered:
      {
        if (event.text.unicode > 0 && event.text.unicode < 0x10000)
          ImGui::GetIO().AddInputCharacter(event.text.unicode);
        break;
      }
      default: 
        break;
      }
    }
    
    /**************************************************************************/
    /*!
    @brief  Backs up the current OpenGL state.
    @note   This is currently being done through the OpenGL fixed pipeline
            to make the code simpler to read!
    @todo   Update to use core profile.
    */
    /**************************************************************************/
    IMGUI_API OpenGLStateData ImGuiSFML::ImGuiSFMLBackupGLState()
    {
      OpenGLStateData currentState;      
      glGetIntegerv(GL_CURRENT_PROGRAM, &currentState.lastProgram);
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentState.lastTexture);
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentState.lastArrayBuffer);
      glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentState.lastElementArrayBuffer);
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentState.lastVertexArray);
      glGetIntegerv(GL_BLEND_SRC, &currentState.lastBlendSrc);
      glGetIntegerv(GL_BLEND_DST, &currentState.lastBlendDst);
      glGetIntegerv(GL_BLEND_EQUATION_RGB, &currentState.lastBlendEquationRGB);
      glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &currentState.lastBlendEquationAlpha);
      currentState.lastEnableBlend = glIsEnabled(GL_BLEND);
      currentState.lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
      currentState.lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
      currentState.lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
      return currentState;
    }

    /**************************************************************************/
    /*!
    @brief  Restores the modified OpenGL state.
    @param  lastTexture A handle to the last texture used before ImGui
            started rendering.
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLRestoreState(OpenGLStateData& currentState)
    {
      glUseProgram(currentState.lastProgram);
      glBindTexture(GL_TEXTURE_2D, currentState.lastTexture);
      glBindBuffer(GL_ARRAY_BUFFER, currentState.lastArrayBuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentState.lastElementArrayBuffer);
      glBindVertexArray(currentState.lastVertexArray);
      glBlendEquationSeparate(currentState.lastBlendEquationRGB, currentState.lastBlendEquationAlpha);
      glBlendFunc(currentState.lastBlendSrc, currentState.lastBlendDst);
      if (currentState.lastEnableBlend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
      if (currentState.lastEnableCullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
      if (currentState.lastEnableDepthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
      if (currentState.lastEnableScissorTest) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    }

    /**************************************************************************/
    /*!
    @brief  The main rendering function that has to be implemented and
            provided to ImGui (by binding it)
    @param  draw_data A struct containing all the render data
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLRenderDrawLists(ImDrawData * draw_data)
    {
      //ImGuiIO& io = ImGui::GetIO();
      
      //io.DisplaySize = ImVec2(WindowContext->getSize().x, WindowContext->getSize().y);

      // 1. Backup the current OpenGL state
      OpenGLStateData currentState = ImGuiSFMLBackupGLState();

      // 2. Setup the next render state: 
      glEnable(GL_BLEND);
      glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_SCISSOR_TEST);
      glActiveTexture(GL_TEXTURE0);

      // 2.1 Handle cases of screen coordinates != from framebuffer coordinates
      // (e.g retina displays)
      ImGuiIO& io = ImGui::GetIO();
      //io.DisplaySize = ImVec2(1440, 900);
      float fbHeight = io.DisplaySize.y * io.DisplayFramebufferScale.y;
      draw_data->ScaleClipRects(io.DisplayFramebufferScale);

      // 3. Setup the Orthographic projection matrix
      const glm::mat4x4 orthoProjection = {
        { 2.0f / io.DisplaySize.x, 0.0f,                        0.0f, 00.f },
        { 0.0f,                    2.0f / -io.DisplaySize.y,    0.0f, 0.0f },
        { 0.0f,                    0.0f,                       -1.0f, 0.0f },
        { -1.0f,                   1.0f,                        0.0f, 1.0f },
      };




      // 4. Active the shader program
      glUseProgram(ShaderHandle);
      glUniform1i(AttribLocationTex, 0);
      glUniformMatrix4fv(AttribLocationProjMtx, 1, GL_FALSE, &orthoProjection[0][0]);
      glBindVertexArray(VAOHandle);

      // 5. Update command lists

      for (int n = 0; n < draw_data->CmdListsCount; n++)
      {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, VBOHandle);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementsHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

        for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
        {
          if (pcmd->UserCallback)
          {
            pcmd->UserCallback(cmd_list, pcmd);
          }
          else
          {
            glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
            glScissor((int)pcmd->ClipRect.x, (int)(fbHeight - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
            glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
          }
          idx_buffer_offset += pcmd->ElemCount;
        }
      }

      // 6. Restores the modified state
      ImGuiSFMLRestoreState(currentState);
    }
    
    /**************************************************************************/
    /*!
    @brief  Terminates ImGui.
    */
    /**************************************************************************/
    IMGUI_API void ImGuiSFML::ImGuiSFMLTerminate()
    {
      if (VAOHandle) glDeleteVertexArrays(1, &VAOHandle);
      if (VBOHandle) glDeleteBuffers(1, &VBOHandle);
      if (ElementsHandle) glDeleteBuffers(1, &ElementsHandle);
      VAOHandle = VBOHandle = ElementsHandle = 0;

      // If we compiled the shader manually..
      if (COMPILE_SHADER_MANUALLY) {
        glDetachShader(ShaderHandle, VertexHandle);
        glDeleteShader(VertexHandle);
        VertexHandle = 0;

        glDetachShader(ShaderHandle, FragHandle);
        glDeleteShader(FragHandle);
        FragHandle = 0;

        glDeleteProgram(ShaderHandle);
        ShaderHandle = 0;
      }

      if (g_FontTexture)
      {
        glDeleteTextures(1, &g_FontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        g_FontTexture = 0;
      }

      //ImGuiSFMLInvalidateDeviceObjects();
      ImGui::Shutdown();
    }



  }
}