#include "ZilchInterface.h"

/**************************************************************************/
/*!
@brief  Constructor.
@note   Any one time startup and static initialization Zilch needs to do
        This also registers a custom assertion handler (Zilch code has many 
        user friendly asserts!)
*/
/**************************************************************************/
ZilchInterface::ZilchInterface() : Setup(Zilch::StartupFlags::None)
{
  // Here, we can register our own callback for when compilation errors occur
  // The default callback prints the file, line/character number, and message to stderr
  Zilch::EventConnect(&Project, Zilch::Events::CompilationError, Zilch::DefaultErrorCallback);
  // Set up the console so that when we call 'Console.WriteLine', it outputs to stdio
  //Zilch::EventConnect(&Project, Zilch::Events::CompilationError, Zilch::DefaultErrorCallback);
}
ZilchInterface::~ZilchInterface()
{
}

void ZilchInterface::Initialize()
{  
}

bool ZilchInterface::AddCodeFromFile(std::string fileName)
{
  return Project.AddCodeFromFile(fileName.c_str());
}

void ZilchInterface::AddCodeFromString(std::string code, std::string origin)
{
  Project.AddCodeFromString(code.c_str(), origin.c_str());
}

void ZilchInterface::Build()
{
  State = Compile(Project);
}

Zilch::BoundType * ZilchInterface::getBoundType(std::string typeName)
{
  Zilch::BoundType* type = Library->BoundTypes.findValue(typeName.c_str(), nullptr);
  ErrorIf(type == nullptr, std::string("Failed to find a Zilch type named '" + typeName + "'").c_str());
  return type;
}

Zilch::Function * ZilchInterface::getFunction(std::string name, Zilch::BoundType* type)
{
  Zilch::Function* function = type->FindFunction(name.c_str(),
                                                Zilch::Array<Zilch::Type*>(),
                                                ZilchTypeId(void),
                                                Zilch::FindMemberOptions::None);

  auto typeName = type->Name.c_str();
  ErrorIf(function == nullptr, std::string("Failed to find a function named '" + name + "' on" + typeName).c_str() );  
  return function;
}

Zilch::Field * ZilchInterface::getInstanceField(std::string name, Zilch::BoundType* type)
{
  Zilch::Field* field = type->InstanceFields.findValue(name.c_str(), nullptr);
  auto typeName = type->Name.c_str();
  ErrorIf(field == nullptr, std::string("Failed to find '" + name + "' on " + typeName).c_str() );
  return field;
}

Zilch::ExecutableState* ZilchInterface::Compile(Zilch::Project& projectRef)
{
  // Links together all core libraries, along our own library.
  Zilch::Module dependencies;
  // The compile function returns a Zilch library
  Library = projectRef.Compile("Daisy Engine", dependencies, Zilch::EvaluationMode::Project);
  ErrorIf(Library == nullptr, "Failed to compile library");
  // Link together all libraries, core and custom
  dependencies.push_back(Library);
  // Link all the libraries together into one executable state
  Zilch::ExecutableState* state = dependencies.Link();
  ErrorIf(state == nullptr, "Failed to link libraries together");
  return state;
}



void ZilchInterface::Terminate()
{
  delete State;
}
