#include "ReflectionZilch.h"

namespace DCEngine {
  namespace Systems {
    
    ReflectionZilch::ReflectionZilch()
    {

      ///* Any one time startup and static initialization that Zilch needs to do
      //   This also registers a custom assertion handler.
      //   (Zilch code has many user friendly asserts!)  
      // */
      //Zilch::ZilchSetup setup(Zilch::StartupFlags::None);

      ///* A project contains all of the code we combine together to make a single
      //  zilch library. The project also sends events for compilation errors that occur
      //  (includes friendly nessages / error codes)
      //*/       
      //Zilch::Project project;

      ///* Here, we register our own callback for when compilation errors occur,.
      //   The default callback prints the file, line/character number, and message to stderr */
      //Zilch::EventConnect(&project, Zilch::Events::CompilationError, Zilch::DefaultErrorCallback);

      ///* Add some test Zilch code that does NOT compile, just to try? 
      //   The secodn parameter provides a name for when compilation errors come up,
      //   in this case 'MyCode' */

      ///* Create a list of dependent libraries, in our case we're really not adding anything
      //   to this. Side note: The core library in Zilch is always added as a depenedency, because
      //   Core includes things like Integer, Boolean, Real, the basic vector types, String, etc.. */
      //Zilch::Module dependencies;
      //
      ///* Compuile all the code we added together into a singgle library named "Game".
      //   We already know this is going to fail to compile, so the error callback we
      //   provided above should get called. */
      //project.Compile("Game", dependencies, Zilch::EvaluationMode::Project);
    }

    void ReflectionZilch::Initialize()
    {
    }

    void ReflectionZilch::Update(float dt)
    {
    }

    void ReflectionZilch::Terminate()
    {
    }

  }
}