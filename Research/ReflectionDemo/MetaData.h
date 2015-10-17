#pragma once
#include <string>


  /**************************************************************************/
  /*!
  @brief Simple class that stores the size and name of a type of data.
  */
  /**************************************************************************/
  class MetaData {
  public:
    MetaData(std::string string, unsigned val) : DataName(string), DataSize(val) {}
    ~MetaData() {}

    const std::string& Name(void) const {
      return DataName;
    }

  private:
    std::string DataName;
    unsigned DataSize;
  };

  /**************************************************************************/
  /*!
  @brief Simple class that stores the size and name of a type of data.
  */
  /**************************************************************************/
  template <typename MetaType>
  class MetaCreator {
  public:
    MetaCreator(std::string name, unsigned size) {
      Initialize(name, size);
    }

    static void Initialize(std::string name, unsigned size) {
      Get()->Initialize(name, size);
    }

    // Ensures a single isntance can exist for this class type
    static MetaData* Get() {
      static MetaData instance;
      return &instance;
    }


  };
