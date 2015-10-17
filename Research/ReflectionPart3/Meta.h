#pragma once

#define DEFINE_META( TYPE ) \
  MetaCreator<RemQual<TYPE>::type> NAME_GENERATOR( )( #TYPE, sizeof( TYPE ) ); \
  RemQual<TYPE>::type *TYPE::NullCast( void ) { return reinterpret_cast<RemQual<TYPE>::type *>(NULL); } \
  void TYPE::AddMember( std::string name, unsigned offset, MetaData *data ) { return MetaCreator<RemQual<TYPE>::type>::AddMember( name, offset, data ); } \
  void MetaCreator<RemQual<TYPE>::type>::RegisterMetaData( void ) { TYPE::RegisterMetaData( ); } \
  void TYPE::RegisterMetaData( void )

  // META_DATA
  // Purpose : This macro goes on the inside of a class within the public section. It declares
  //           a few member functions for use by the MetaData system to retrieve information about
  //           the class.
#define META_DATA( TYPE ) \
  static void AddMember( std::string name, unsigned offset, MetaData *data ); \
  static RemQual<TYPE>::type *NullCast( void ); \
  static void RegisterMetaData( void )

// Defines the RegisterMetaData for you
#define DEFINE_META_POD( TYPE ) \
  MetaCreator<RemQual<TYPE>::type> NAME_GENERATOR( )( #TYPE, sizeof( TYPE ) ); \
  void MetaCreator<RemQual<TYPE>::type>::RegisterMetaData( void ) \
  { \
  }

#define ADD_MEMBER( MEMBER ) \
  AddMember( #MEMBER, (unsigned)(&(NullCast( )->MEMBER)), META( NullCast( )->MEMBER ))

#define PASTE( _, __ )  _##__
#define GENERATE_LINE( _ ) PASTE( GENERATED_TOKEN_, _ )
#define GENERATE_FILE( _ ) PASTE( __FILE__, _ )
#define NAME_GENERATOR( ) GENERATE_FILE( __LINE__ )

  //
  // META_TYPE
  // Purpose: Retrieves the proper MetaData instance of an object by type.
  //
#define META_TYPE( TYPE ) (MetaCreator<RemQual<TYPE>::type>::Get( ))

  //
  // META
  // Purpose: Retrieves the proper MetaData instance of an object by an object's type.
  //
#define META( OBJECT ) (MetaCreator<RemQual<decltype( OBJECT )>::type>::Get( ))

  //
  // META_STR
  // Purpose : Finds a MetaData instance by string name
  //
#define META_STR(STRING) (MetaManager::Get(STRING))

class MetaData;

//
// Member
// Purpose: Stores information (name and offset of member) about a data member of a specific class. Multiple
//          Member objects can be stored in MetaData objects within a std::vector.
//
class Member
{
public:
  Member( std::string string, unsigned val, MetaData *meta );
  ~Member( );

  const std::string &Name( void ) const; // Gettor for name
  unsigned Offset( void ) const; // Gettor for offset
  const MetaData *Meta( void ) const; // Gettor for data

private:
  std::string name;
  unsigned offset;
  const MetaData *data;
};

//
// MetaData
// Purpose: Object for holding various info about any C++ type for the MetaData reflection system.
//
class MetaData
  {
  public:
    MetaData( std::string string = "", unsigned val = 0 );
    ~MetaData( );

    void Init( std::string string, unsigned val );

    const std::string& Name( void ) const;
    unsigned Size( void ) const;
    void AddMember( const Member *member );
    
    // Public so can print easily in main
    std::vector<const Member *> members;

  private:
    std::string name;
    unsigned size;
};

template <typename Metatype>
class MetaCreator
{
public:
  MetaCreator( std::string name, unsigned size )
  {
    Init( name, size );
  }

  static void Init( std::string name, unsigned size )
  {
    Get( )->Init( name, size );
    RegisterMetaData( );
  }

  static void AddMember( std::string memberName, unsigned memberOffset, MetaData *meta )
  {
    Get( )->AddMember( new Member( memberName, memberOffset, meta ) );
  }

  static Metatype *NullCast( void )
  {
    return reinterpret_cast<Metatype *>(NULL);
  }

  static void RegisterMetaData( void );

  // Ensure a single instance can exist for this class type
  static MetaData *Get( void )
  {
    static MetaData instance;
    return &instance;
  }
};