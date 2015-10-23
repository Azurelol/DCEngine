#pragma once
#include "Zilch.hpp"
#include <string>

class ZilchInterface
{

public:
  ZilchInterface();
  ~ZilchInterface();

  void Initialize();
  bool AddCodeFromFile(std::string fileName);
  void AddCodeFromString(std::string code, std::string origin);
  Zilch::ExecutableState* Compile(Zilch::Project& projectRef);
  void Build();

  Zilch::BoundType* getBoundType(std::string name);
  Zilch::Function* getFunction(std::string name, Zilch::BoundType* type);
  Zilch::Field* getInstanceField(std::string name, Zilch::BoundType* type);

  void Update();
  void Terminate();

private:
  void Clear();

  Zilch::ZilchSetup Setup;
  Zilch::Project Project;
  Zilch::LibraryRef Library;
  Zilch::ExecutableState* State;
  


};

