#include "Classes.h"
#include <iostream>
int main() {



  // End of demo
  std::cout << "End of reflection demo\n";
  std::cin.get();
  return 0;
=======
#include <iostream>

#include "Reflection.h"
#include "Object.h"

int main() {

  // Construct the reflection system
  Reflection();


  std::cout << "Reflection demo is finished." << std::endl;
  std::cin.get();
  return 0;

}