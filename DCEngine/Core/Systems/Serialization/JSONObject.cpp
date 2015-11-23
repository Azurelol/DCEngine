#include "JSONObject.h"

namespace DCEngine {

  JSONObject::JSONObject(Zilch::JsonValue * value) : Value(value), IsValue(true)
  {
  }

  JSONObject::JSONObject(Zilch::JsonMember * member) : Member(member), IsValue(false)
  {
  }

  Zilch::JsonValue * JSONObject::GetValue()
  {
    return this->Value;
  }

  Zilch::JsonMember * JSONObject::GetMember()
  {
    return this->Member;
  }

}