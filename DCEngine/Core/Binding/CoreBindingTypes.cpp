#include "CoreBindingTypes.h"

/*******************************************************************************
  std::string <-> Zilch::String
*******************************************************************************/
inline Zilch::String stdstringToZilchString(const std::string& string)
{
  return Zilch::String(string.c_str());
}
inline std::string ZilchStringTostdstring(const Zilch::String& string)
{
  return std::string(string.c_str());
}
ZilchDefineRedirectType(std::string, stdstringToZilchString, ZilchStringTostdstring);

/*******************************************************************************
  glm::vec2 <-> Zilch::Real2
*******************************************************************************/
inline Zilch::Real2 Vec2ToReal2(const glm::vec2& vec)
{
  return Zilch::Real2(vec.x, vec.y);
}
inline glm::vec2 Real2ToVec2(const Zilch::Real2& vec)
{
  return glm::vec2(vec.x, vec.y);
}
ZilchDefineRedirectType(glm::vec2, Vec2ToReal2, Real2ToVec2);

/*******************************************************************************
glm::vec3 <-> Zilch::Real3
*******************************************************************************/
inline Zilch::Real3 Vec3ToReal3(const glm::vec3& vec)
{
  return Zilch::Real3(vec.x, vec.y, vec.z);
}
inline glm::vec3 Real3ToVec3(const Zilch::Real3& vec)
{
  return glm::vec3(vec.x, vec.y, vec.z);
}
ZilchDefineRedirectType(glm::vec3, Vec3ToReal3, Real3ToVec3);

/*******************************************************************************
glm::vec4 <-> Zilch::Real4
*******************************************************************************/
inline Zilch::Real4 Vec4ToReal4(const glm::vec4& vec)
{
  return Zilch::Real4(vec.x, vec.y, vec.z, vec.w);
}
inline glm::vec4 Real4ToVec4(const Zilch::Real4& vec)
{
  return glm::vec4(vec.x, vec.y, vec.z, vec.w);
}
ZilchDefineRedirectType(glm::vec4, Vec4ToReal4, Real4ToVec4);