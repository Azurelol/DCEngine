#include "SerializerJSONCPP.h"
#include "../../Objects/ObjectsInclude.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  Serializes an entity object into a file.
  \param  A pointer to the entity.
  \param  The name of the output string.
  \return The success of the operation.
  \note   
  */
  /**************************************************************************/
  bool SerializerJSONCPP::Serialize(Entity * entityObj, std::string & output) {
    if (entityObj == NULL)
      return false;
    
    // Create an instance of Json::Value to act as our root
    Json::Value serializeRoot;
    // Pass it to the Serialize method which will fill the 'Value' object
    // with all the serialized data.
    entityObj->Serialize(serializeRoot);
    // Create an instance of 'StyledWriter' to write the Json data to the 
    // string that was passed in
    Json::StyledWriter writer;
    output = writer.write(serializeRoot);
    return true;
  }

  /**************************************************************************/
  /*!
  \brief  Deserializes a file into an entity object.
  \param  A pointer to the entity.
  \param  The name of the input string.
  \return The success of the operation.
  */
  /**************************************************************************/
  bool SerializerJSONCPP::Deserialize(Entity * entityObj, std::string & input) {
    if (entityObj == NULL)
      return false;

    // Create an instance of 'Json::Value' to act as our root
    Json::Value deserializeRoot;
    Json::Reader reader;

    // Parse the input string and fill the 'Json::Value' object
    if (!reader.parse(input, deserializeRoot))
      return false;
    // Use the 'Value' object and pass it to the entity object's 
    // serialize method, which will set the data members based on the
    // 'Json::Value' object's values
    entityObj->Deserialize(deserializeRoot);
    return true;
  }

  /**************************************************************************/
  /*!
  \brief  Serializes an component object into a file.
  \param  A pointer to the component.
  \param  The name of the output string.
  \return The success of the operation.
  \note
  */
  /**************************************************************************/
  bool SerializerJSONCPP::Serialize(Component * componentObj, std::string & output) {
    if (componentObj == NULL)
      return false;

    // Create an instance of Json::Value to act as our root
    Json::Value serializeRoot;
    // Pass it to the Serialize method which will fill the 'Value' object
    // with all the serialized data.
    componentObj->Serialize(serializeRoot);
    // Create an instance of 'StyledWriter' to write the Json data to the 
    // string that was passed in
    Json::StyledWriter writer;
    output = writer.write(serializeRoot);
    return true;
  }

  bool SerializerJSONCPP::Deserialize(Component * componentObj, std::string & input) {
    if (componentObj == NULL)
      return false;

    // Create an instance of 'Json::Value' to act as our root
    Json::Value deserializeRoot;
    Json::Reader reader;

    // Parse the input string and fill the 'Json::Value' object
    if (!reader.parse(input, deserializeRoot))
      return false;
    // Use the 'Value' object and pass it to the component object's 
    // serialize method, which will set the data members based on the
    // 'Json::Value' object's values
    componentObj->Deserialize(deserializeRoot);
    return true;
  }

}