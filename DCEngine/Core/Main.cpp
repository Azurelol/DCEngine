/******************************************************************************/
/*!
\file   Main.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The entry point for the application.

*/
/******************************************************************************/
#include "Engine\Engine.h"

using namespace DCEngine;

namespace DCEngine {
  // ???
  extern std::unique_ptr<Engine> Daisy;
}

int main(int argc, char* args[]) {

  new Engine();
    
  Daisy->Initialize();
  
  trace << "\n[Engine::Loop]\n";
  Daisy->Loop();

  Daisy->Terminate();  

}