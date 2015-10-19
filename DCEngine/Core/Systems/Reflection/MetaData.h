#pragma once
#include <string>
#include <vector>

#include "RemoveQualifier.h"

namespace DCEngine {
  
  class RefVariant;
  typedef void(*SerializeFn)(std::ostream&, RefVariant);

  /* META_DEFINE_CLASS:  Defines a MetaCreator for a specific type of data */
  #define META_DEFINE_CLASS( TYPE ) \
    MetaCreator<RemoveQualifier<TYPE>::type> NAME_GENERATOR( )( #TYPE, sizeof( TYPE ) ); \
    RemoveQualifier<TYPE>::type *TYPE::NullCast( void ) { return reinterpret_cast<RemoveQualifier<TYPE>::type *>(NULL); } \
    void TYPE::AddMember( std::string name, unsigned offset, MetaData *data ) { return MetaCreator<RemoveQualifier<TYPE>::type>::AddMember( name, offset, data ); } \
    void MetaCreator<RemoveQualifier<TYPE>::type>::RegisterMetaData( void ) { TYPE::RegisterMetaData( ); } \
    void TYPE::RegisterMetaData( void )

  /* META_ADD_CLASS:  This macro goes on the inside of a class within the public
  section. It declares a few member functions for use by the MetaData
  system to retrieve information about the class. */
#define META_ADD_CLASS( TYPE ) \
    static void AddMember( std::string name, unsigned offset, MetaData *data ); \
    static RemoveQualifier<TYPE>::type *NullCast( void ); \
    static void RegisterMetaData( void )

  /* META_ADD_POD: Defines the RegisterMetaData for the client. (??) */
#define META_ADD_POD( TYPE ) \
  MetaCreator<RemoveQualifier<TYPE>::type> NAME_GENERATOR( )( #TYPE, sizeof( TYPE ) ); \
  void MetaCreator<RemoveQualifier<TYPE>::type>::RegisterMetaData( void ) \
  { \
  }
    //MetaCreator<RemoveQualifier<TYPE>::type>::SetSerializeFn( Serialization::SerializeTextPrimitive<RemoveQualifier<TYPE>::type> ); \
  }

  /* META_ADD_CLASS_MEMBER: This macro takes the number zero, turns it into a pointer to a
  type of object (class or struct). After it is typecast it uses the -> operator to access
  one of the members. Lastly, it uses the & operator to retrieve the address of the member's
  location (which is offset from zero by the -> operator) and typecasts this into an unsigned
  integer. */

#define META_ADD_CLASS_MEMBER(MEMBER) \
    AddMember( #MEMBER, (unsigned)(&(NullCast( )->MEMBER)), META( NullCast( )->MEMBER ))

#define PASTE_TOKENS_2( _, __ ) _##__
#define PASTE_TOKENS( _, __ ) PASTE_TOKENS_2( _, __ )
#define NAME_GENERATOR_INTERNAL( _ ) PASTE_TOKENS( GENERATED_TOKEN_, _ )
#define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )
  //#define PASTE( _, __ )  _##__
  //#define GENERATE_LINE( _ ) PASTE( GENERATED_TOKEN_, _ )
  //#define GENERATE_FILE( _ ) PASTE( __FILE__, _ )
  //#define NAME_GENERATOR( ) GENERATE_FILE( __LINE__ )

  /* META_TYPE: Retrieves the proper MetaData instance of an object by type. */
#define META_TYPE( TYPE ) (MetaCreator<RemoveQualifier<TYPE>::type>::Get( ))
  /* META: Retrives the proper MetaData instance of an object by an object's type */
#define META( OBJECT ) (MetaCreator<RemoveQualifier<decltype( OBJECT )>::type>::Get( ))
  /* META_STR: Finds a MetaData instance by string name */
#define META_STR( STRING ) (MetaManager::Get( STRING ))
  /* PRINT_MEMBERS: Prints the members of a class.*/
#define META_PRINT_MEMBERS( TYPE ) \
  PrintMembers<TYPE>( #TYPE )
#define META_PRINT_TYPE( TYPE ) \
  PrintType<TYPE>( #TYPE )

  /**************************************************************************/
  /*!                           Meta Class Definitions                      */
  /**************************************************************************/
  /**************************************************************************/
  /*!
  @brief The Member class is a container of the various bits of information
  we'd like to store about any member variable. It stores the name
  and offset about a data member of a specific class. Multiple
  Member objects can be stored in MetaData objects within a vector.
  */
  /**************************************************************************/
  class MetaData;
  class Member {
  public:
    Member(std::string string, unsigned val, MetaData* meta);
    ~Member();

    const std::string& Name() const;
    unsigned Offset() const;
    const MetaData* Meta() const;

    Member*& Next(void);
    Member* const& Next(void) const;

  private:
    std::string MemberName;
    unsigned MemberOffset;
    const MetaData* MemberData;
    Member* NextMember;
  };
  
  /**************************************************************************/
  /*!
  @brief A MetaData object is a non-templated class, allowing them to be
         stored in a data structure. There is one MetaData object for each
         type of data registered to the system, and the MetaData object of
         a corresponding type is a representation of that type.
         It stores information about whether or not it is a plain old data
         type (POD), the size of the type, its members, methods, and name.
         Inheritance information can be stored along with multiple 
         inheritance information (though the latter is not implemented).
  */
  /**************************************************************************/
  class MetaData {
  public:
    MetaData(std::string string = "", unsigned val = 0);
    ~MetaData();

    void Initialize(std::string string, unsigned val);

    const std::string& Name() const;
    unsigned Size() const;
    void AddMember(const Member* member);
    bool HasMembers() const;

    void Copy(void* data, const void* source) const;
    void Delete(void* data) const;
    void* NewCopy(const void* source) const;
    void* New(void) const;

    void SetSerialize(SerializeFn fn = NULL);
    void Serialize(std::ostream& os, RefVariant var) const;

    std::vector<const Member*> MemberContainer;
    void PrintMembers(std::ostream& os) const;

  private:
    SerializeFn DataSerializeFn;
    std::string DataName;
    unsigned DataSize;
  };

  /**************************************************************************/
  /*!
  @brief The MetaCreator is a templated class that manages the creation of a 
         single unique MetaData instance. It then adds its instance into the 
         MetaManager which contains it in some sort of map.
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
      RegisterMetaData();
    }

    static void AddMember(std::string memberName, unsigned memberOffset, MetaData* meta) {
      Get()->AddMember(new Member(memberName, memberOffset, meta));
    }

    static void SetSerializeFn(SerializeFn fn) {
      Get()->SetSerialize(fn);
    }

    static MetaType *NullCast() {
      return reinterpret_cast<MetaType*>(NULL);
    }

    static void RegisterMetaData(void);

    // Ensures a single instance can exist for this class type
    static MetaData* Get() {
      static MetaData instance;
      return &instance;
    }
  };







}