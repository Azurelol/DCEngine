#include "TypesMeta.h"

// Engine types
#include "../../Engine/Types.h"
#include "MetaData.h"

namespace DCEngine {

  // Integrals
  META_ADD_POD(int);
  META_ADD_POD(int*);
  META_ADD_POD(char);
  META_ADD_POD(char*);
  META_ADD_POD(double);
  META_ADD_POD(float);
  META_ADD_POD(bool);

  // Standard library
  META_ADD_POD(std::string);

  // Standard-template library

  
  // Engine types
  //META_ADD_POD(Real);
  //META_ADD_POD(Boolean);

}


