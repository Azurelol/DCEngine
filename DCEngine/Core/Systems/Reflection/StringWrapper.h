#pragma once
#include <iostream>
#include "MetaData.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Wrapper class for std::string.
  */
  /**************************************************************************/
  class StringWrapper {

  public:
    StringWrapper(const char *src);
    ~StringWrapper();

    bool IsEqual(const StringWrapper& rhs) const;
    void Concat(const StringWrapper& rhs);
    friend std::ostream& operator<<(std::ostream& os, StringWrapper& rhs);

    META_ADD_CLASS(StringWrapper);

  private:
    const char* Data;
    unsigned Length;
  };

}