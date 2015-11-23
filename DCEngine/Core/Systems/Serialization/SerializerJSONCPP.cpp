/*****************************************************************************/
/*!
@file   SerializerJSONCPP.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SerializerJSONCPP.h"
#include "../../Objects/ObjectsInclude.h"
#include "../System.h"

namespace DCEngine {

  ///**************************************************************************/
  ///*!
  //\brief  Serializes an entity object into a file.
  //\param  A pointer to the entity.
  //\param  The name of the output string.
  //\return The success of the operation.
  //\note   
  //*/
  ///**************************************************************************/
  //bool SerializerJSONCPP::Serialize(Entity * entityObj, std::string & output) {
  //  if (entityObj == NULL)
  //    return false;
  //  
  //  // Create an instance of Json::Value to act as our root
  //  Json::Value serializeRoot;
  //  // Pass it to the Serialize method which will fill the 'Value' object
  //  // with all the serialized data.
  //  entityObj->Serialize(serializeRoot);
  //  // Create an instance of 'StyledWriter' to write the Json data to the 
  //  // string that was passed in
  //  Json::StyledWriter writer;
  //  output = writer.write(serializeRoot);
  //  return true;
  //}

  ///**************************************************************************/
  ///*!
  //\brief  Deserializes a file into an entity object.
  //\param  A pointer to the entity.
  //\param  The name of the input string.
  //\return The success of the operation.
  //*/
  ///**************************************************************************/
  //bool SerializerJSONCPP::Deserialize(Entity * entityObj, std::string & input) {
  //  if (entityObj == NULL)
  //    return false;

  //  // Create an instance of 'Json::Value' to act as our root
  //  Json::Value deserializeRoot;
  //  Json::Reader reader;

  //  // Parse the input string and fill the 'Json::Value' object
  //  if (!reader.parse(input, deserializeRoot))
  //    return false;
  //  // Use the 'Value' object and pass it to the entity object's 
  //  // serialize method, which will set the data members based on the
  //  // 'Json::Value' object's values
  //  entityObj->Deserialize(deserializeRoot);
  //  return true;
  //}

  ///**************************************************************************/
  ///*!
  //\brief  Serializes an component object into a file.
  //\param  A pointer to the component.
  //\param  The name of the output string.
  //\return The success of the operation.
  //\note
  //*/
  ///**************************************************************************/
  //bool SerializerJSONCPP::Serialize(Component * object, std::string & output) {
  //  if (object == NULL)
  //    return false;

  //  // Create an instance of Json::Value to act as our root
  //  Json::Value serializeRoot;
  //  // Pass it to the Serialize method which will fill the 'Value' object
  //  // with all the serialized data.
  //  object->Serialize(serializeRoot);
  //  // Create an instance of 'StyledWriter' to write the Json data to the 
  //  // string that was passed in
  //  Json::StyledWriter writer;
  //  output = writer.write(serializeRoot);
  //  return true;
  //}

  //bool SerializerJSONCPP::Deserialize(Component * object, std::string & input) {
  //  if (object == NULL)
  //    return false;

  //  // Create an instance of 'Json::Value' to act as our root
  //  Json::Value deserializeRoot;
  //  Json::Reader reader;

  //  if (!reader.parse(input, deserializeRoot))
  //    return false;

  //  object->Deserialize(deserializeRoot);
  //  return true;
  //}

  //bool SerializerJSONCPP::Serialize(System * object, std::string & output)
  //{
  //  Json::Value serializeRoot;
  //  object->Serialize(serializeRoot);

  //  Json::StyledWriter writer;
  //  output = writer.write(serializeRoot);
  //  return true;
  //}

  //bool SerializerJSONCPP::Deserialize(System * object, std::string & input)
  //{
  //  // Create an instance of 'Json::Value' to act as our root
  //  Json::Value deserializeRoot;
  //  Json::Reader reader;

  //  if (!reader.parse(input, deserializeRoot))
  //    return false;

  //  object->Deserialize(deserializeRoot);
  //  return true;
  //}

}