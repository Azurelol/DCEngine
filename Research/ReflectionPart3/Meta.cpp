#include "Precompiled.h"

namespace Engine
{
  Member::Member(std::string string, unsigned val, MetaData *meta) : name(string), offset(val), data(meta)
  {
  }

  Member::~Member()
  {
  }

  const std::string& Member::Name(void) const
  {
    return name;
  }

  unsigned Member::Offset(void) const
  {
    return offset;
  }

  const MetaData *Member::Meta(void) const
  {
    return data;
  }

  MetaData::MetaData(std::string string, unsigned val) : name(string), size(val)
  {
  }

  MetaData::~MetaData()
  {
  }

  void MetaData::Init(std::string string, unsigned val)
  {
    name = string;
    size = val;
  }

  const std::string& MetaData::Name(void) const
  {
    return name;
  }

  unsigned MetaData::Size(void) const
  {
    return size;
  }

  void MetaData::AddMember(const Member *member)
  {
    members.push_back(member);
  }
}