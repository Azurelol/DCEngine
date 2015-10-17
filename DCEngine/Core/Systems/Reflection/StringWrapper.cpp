#include "StringWrapper.h"


namespace DCEngine {

  META_DEFINE_CLASS(StringWrapper)
  {
    META_ADD_CLASS_MEMBER(Data);
    META_ADD_CLASS_MEMBER(Length);
  }

  StringWrapper::StringWrapper(const char *src) : Data(src)
  {
  }

  StringWrapper::~StringWrapper()
  {
  }

  bool StringWrapper::IsEqual(const StringWrapper& rhs) const
  {
    if (strcmp(Data, rhs.Data) == 0)
      return true;
    return false;
  }

  // Untested!
  void StringWrapper::Concat(const StringWrapper& rhs)
  {
    char *temp = new char[rhs.Length + Length];
    strcpy(temp, Data);
    strcpy(temp + Length, rhs.Data);
  }

  std::ostream& operator<<(std::ostream &os, StringWrapper &rhs)
  {
    os << rhs.Data;
    return os;
  }

}

