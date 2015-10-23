#include "ZilchInterface.h"

int main()
{
  auto interface = new ZilchInterface();

  // Tests
  interface->Initialize();
  interface->AddCodeFromFile("Player.zilch");
  interface->Build();
  
  auto playerType = interface->getBoundType("Player");
  auto hello = interface->getFunction("SayHello", playerType);



  return 0;
}