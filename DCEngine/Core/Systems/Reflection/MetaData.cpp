#include "MetaData.h"
#include "RefVariant.h"
#include "../Serialization/Serialization.h"

namespace DCEngine {

  /**************************************************************************/
  /*!                           Member                                      */
  /**************************************************************************/

  Member::Member(std::string string, unsigned val, MetaData * meta) : MemberName(string),
                                                                      MemberOffset(val),
                                                                      MemberData(meta),
                                                                      NextMember(NULL)
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

  Member *& Member::Next(void)
  {
    return NextMember;
  }

  Member * const & Member::Next(void) const
  {
    return NextMember;
  }

  /**************************************************************************/
  /*!                           MetaData                                    */
  /**************************************************************************/

  MetaData::MetaData(std::string string, unsigned val) : DataName(string), 
                                                          DataSize(val) 
  {
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

  bool MetaData::HasMembers() const
  {
    if (MemberContainer.size() > 0)
      return true;
    else
      return false;    
  }

  void MetaData::Copy(void * dest, const void * source) const
  {
    memcpy(dest, source, DataSize);
  }

  void MetaData::Delete(void * data) const
  {
    delete[] reinterpret_cast<char*>(data);
    data = NULL;
  }

  void * MetaData::NewCopy(const void * source) const
  {
    void* data = new char[DataSize];
    memcpy(data, source, DataSize);
    return data;
  }

  void * MetaData::New(void) const
  {
    return new char[DataSize];
  }

  void MetaData::SetSerialize(SerializeFn fn)
  {
    DataSerializeFn = fn;
  }

  void MetaData::Serialize(std::ostream & os, RefVariant var) const
  {
    if (DataSerializeFn)
      DataSerializeFn(os, var);
    else
      Serialization::SerializeText(os, var);
    
  }

  void MetaData::PrintMembers(std::ostream & os) const
  {

  }

  /**************************************************************************/
  /*!                           Text Serializaiton                          */
  /**************************************************************************/

  // Takes a pointer and returns a pointer offset in bytes
  #define PTR_ADD(PTR, OFFSET) \
  ((void*)(((char*)PTR) + (OFFSET)))

  static unsigned level = 0;

  void Padding(std::ostream & os)
  {
    for (unsigned i = 0; i < level; ++i)
      os << " ";
  }

  void SerializeText(std::ostream & os, RefVariant var)
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