#include "MetaData.h"

namespace DCEngine {

  /**************************************************************************/
  /*!                           Member                                      */
  /**************************************************************************/

  Member::Member(std::string string, unsigned val, MetaData * meta) : MemberName(string),
                                                                      MemberOffset(val),
                                                                      MemberData(meta)
  {
  }

  Member::~Member()
  {
  }

  const std::string & Member::Name() const
  {
    return MemberName;
  }

  unsigned Member::Offset() const
  {
    return MemberOffset;
  }

  const MetaData * Member::Meta() const
  {
    return MemberData;
  }

  /**************************************************************************/
  /*!                           MetaData                                    */
  /**************************************************************************/

  MetaData::MetaData(std::string string, unsigned val) : DataName(string), DataSize(val) {
  }

  MetaData::~MetaData()
  {
  }

  void MetaData::Initialize(std::string string, unsigned val)
  {
    DataName = string;
    DataSize = val;
  }

  const std::string & MetaData::Name(void) const
  {
    return DataName;
  }

  unsigned MetaData::Size(void) const
  {
    return DataSize;
  }

  void MetaData::AddMember(const Member * member)
  {
    MemberContainer.push_back(member);
  }


}