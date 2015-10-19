/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : MetaTypeReg.cpp
Purpose       : Forces linking of a lot of registration
                objects.
-----------------------------------------------------*/

#pragma once

#pragma warning( disable : 4083 )

#ifndef CC_REG_EXPLICIT

DECLARE_META( int );
DECLARE_META( float );
DECLARE_META_EX( float *, floatPtr_meta );
DECLARE_META( double );
DECLARE_META( bool );
DECLARE_META( char );
DECLARE_META( unsigned );
DECLARE_META_EX( std::string, string_meta );
DECLARE_META_EX( char *, charPtr_meta );
DECLARE_META_EX( std::wstring, wstring_meta );
DECLARE_META_EX( int *, intPtr_meta );
DECLARE_META_EX( bool *, boolPtr_meta );
DECLARE_META_EX( void *, voidPtr_meta );
DECLARE_META_EX( std::vector<int>, intVector_meta );
DECLARE_META_EX( std::vector<float>, floatVector_meta );
DECLARE_META_EX( std::vector<double>, doubleVector_meta );
DECLARE_META_EX( std::vector<bool>, boolVector_meta );
DECLARE_META_EX( std::vector<char>, charVector_meta );
DECLARE_META_EX( std::vector<unsigned>, unsignedVector_meta );
DECLARE_META_EX( std::vector<std::string>, stringVector_meta );

#else

void RegisterDefaultMetaData( );

#endif // CC_REG_EXPLICIT
