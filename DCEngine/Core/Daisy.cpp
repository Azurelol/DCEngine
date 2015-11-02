/******************************************************************************/
/*!
@file   Daisy.cpp(git)
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   7/30/2015
@brief  The entry point for the Daisy Chain engine.

*/
/******************************************************************************/
#include "Engine\Engine.h"

using namespace DCEngine;

namespace DCEngine {
  extern std::unique_ptr<Engine> Daisy;
}

/**************************************************************************/
/*!
@brief  The entry point for the Daisy Chain Engine.
@param  argc The number of arguments passed to 'main'
@param  argv An array of strings.
@return If there's an error, 1, otherwise 0.
*/
/**************************************************************************/
int main(int argc, char* argv[]) {

  // Temporary:
  std::string configFile = "Daisy.cfg";

  // Construct the engine object
  new Engine(configFile);

  try {
    // Initialize the engine
    Daisy->Initialize();
    // Load the project into the engine
    Daisy->LoadProject(std::string(argv[0]));
    // Start the engine now that project has been loaded
    Daisy->StartProject();
    // Start the engine's main loop
    trace << "\n[Engine::Loop]\n";
    Daisy->Loop();
  }
  catch (Debug::Exception exception) 
  {
    trace << exception << "\n";
    // Poor Daisy cannot afford a nice GUI
    std::cout << "Press any key to leave her... :(\n";
    std::cin.get();
  }

  // Close the engine and clean up
  Daisy->Terminate();

}