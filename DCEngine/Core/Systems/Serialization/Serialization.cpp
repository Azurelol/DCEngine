#include "Serialization.h"
#include "../Reflection/ReflectionMeta.h"

namespace DCEngine {

  // Takes a pointer and returns a pointer offset in bytes
  #define PTR_ADD(PTR, OFFSET) \
  ((void*)(((char*)PTR) + (OFFSET)))

  static unsigned level = 0;

  void Serialization::Padding(std::ostream & os)
  {
    for (unsigned i = 0; i < level; ++i)
      os << " ";
  }

  void Serialization::SerializeText(std::ostream & os, RefVariant var)
  {
    const MetaData* meta = var.Meta();
    void * data = var.Data();

    // If this type does not have a SerializePrimitive associated with it...
    assert(meta->HasMembers());

    os << meta->Name() << std::endl;
    Padding(os);
    os << "{" << std::endl;

    // Iterate through each member and serialize... 
    for (auto member : meta->MemberContainer) {
      ++level;
      void* offsetData = PTR_ADD(var.Data(), member->Offset());
      member->Meta()->Serialize(os, RefVariant(member->Meta(), offsetData));
      --level;
    }

    Padding(os);
    os << "}" << std::endl;

  }
}