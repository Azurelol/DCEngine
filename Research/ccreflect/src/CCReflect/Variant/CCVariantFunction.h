#pragma once

#define CALL_PTR( PTR, FN ) \
  ((PTR)->*(FN))

#include <string>
#include "CCMetaData.h"
#include "CCRefVariant.h"
#include "CtorLister.h"

struct GenericBase1 {};
struct GenericBase2 {};
struct GenericMultiVirtualClass : public virtual GenericBase1, public GenericBase2{};
struct GenericMultiClass : public GenericBase1, public GenericBase2 {};
struct GenericClass {};
struct GenericUnknownClass;
 
typedef void (GenericUnknownClass::*UnknownClassFn)  ( void );
typedef void (GenericMultiClass::*MultiClassFn)      ( void );
typedef void (GenericMultiVirtualClass::*MultiVirtFn)( void );
typedef void (GenericClass::*GenericClassFn)         ( void );
typedef void (*StaticFn)                             ( void );

// Pointer can be of various sizes on different compilers. Just throw everything in a 
// union and let the compiler figure out what size we're dealing with.
union MultiFnPtr
{
  UnknownClassFn UnknownClassFn;
  MultiClassFn   MultiClassFn;
  MultiVirtFn    MultiVirtFn;
  GenericClassFn GenericClassFn;
  StaticFn       StaticFn;
  f64 padding[4];
};

/*------------------------------------------------------------------------------------------------

                                      FunctionSignature

------------------------------------------------------------------------------------------------*/
class FunctionSignature
{
public:
  friend class VariantFunction;

  FunctionSignature( );
  FunctionSignature( const FunctionSignature& rhs );
  const FunctionSignature& operator=( const FunctionSignature& rhs );

    /*----------------
      Global Fn Void
    ----------------*/
  FunctionSignature( void (*fn)( void ) );

  template <typename Arg0>
  FunctionSignature( void (*)( Arg0 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 1 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1>
  FunctionSignature( void (*)( Arg0, Arg1 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 2 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1, typename Arg2>
  FunctionSignature( void (*)( Arg0, Arg1, Arg2 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 3 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  FunctionSignature( void (*)( Arg0, Arg1, Arg2, Arg3 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 4 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  FunctionSignature( void (*)( Arg0, Arg1, Arg2, Arg3, Arg4 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 5 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  FunctionSignature( void (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 6 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  FunctionSignature( void (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 7 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  FunctionSignature( void (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 8 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 )
    };
    argArray = stat_args;
  }

  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  FunctionSignature( void (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( NULL ),
    argCount( 9 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 ),
      META_TYPE( Arg8 )
    };
    argArray = stat_args;
  }
    
    /*----------------
    Global Fn Non-Void
    ----------------*/
  template <typename R>
  FunctionSignature( R (*fn)( void ) ) :
    ret( META_TYPE( R ) ),
    argArray( NULL ),
    context( NULL ),
    argCount( 0 )
  {
  }

  template <typename R, typename Arg0>
  FunctionSignature( R (*)( Arg0 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 1 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1>
  FunctionSignature( R (*)( Arg0, Arg1 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 2 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1, typename Arg2>
  FunctionSignature( R (*)( Arg0, Arg1, Arg2 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 3 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  FunctionSignature( R (*)( Arg0, Arg1, Arg2, Arg3 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 4 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  FunctionSignature( R (*)( Arg0, Arg1, Arg2, Arg3, Arg4 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 5 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  FunctionSignature( R (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 6 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  FunctionSignature( R (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 7 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  FunctionSignature( R (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 8 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 )
    };
    argArray = stat_args;
  }

  template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  FunctionSignature( R (*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) ) :
    ret( META_TYPE( R ) ),
    context( NULL ),
    argCount( 9 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 ),
      META_TYPE( Arg8 )
    };
    argArray = stat_args;
  }

    /*----------------
        Member Fn

        Note:
          The C refers to context. The context is the object in which the function
          refers to, as a member function. The context is passed to a call as the first
          argument.

    ----------------*/
  template <typename C, typename R>
  FunctionSignature( R (C::*)( void ) ) :
    ret( META_TYPE( R ) ),
    argArray( NULL ),
    context( META_TYPE( C ) ),
    argCount( 0 )
  {
  }

  template <typename C, typename R, typename Arg0>
  FunctionSignature( R (C::*)( Arg0 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 1 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1>
  FunctionSignature( R (C::*)( Arg0, Arg1 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 2 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 3 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 4 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 5 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 6 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 7 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 8 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 9 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 ),
      META_TYPE( Arg8 )
    };
    argArray = stat_args;
  }

    /*----------------
      Member Fn Void

        Note:
          The C refers to context. The context is the object in which the function
          refers to, as a member function. The context is passed to a call as the first
          argument.

    ----------------*/
  template <typename C>
  FunctionSignature( void (C::*)( void ) ) :
    ret( META_TYPE( VoidType ) ),
    argArray( NULL ),
    context( META_TYPE( C ) ),
    argCount( 0 )
  {
  }

  template <typename C, typename Arg0>
  FunctionSignature( void (C::*)( Arg0 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 1 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1>
  FunctionSignature( void (C::*)( Arg0, Arg1 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 2 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 3 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 4 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 5 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 6 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 7 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 8 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 9 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 ),
      META_TYPE( Arg8 )
    };
    argArray = stat_args;
  }

    /*----------------
      Member Fn Const

        Note:
          The C refers to context. The context is the object in which the function
          refers to, as a member function. The context is passed to a call as the first
          argument.

    ----------------*/
  template <typename C, typename R>
  FunctionSignature( R (C::*)( void ) const ) :
    ret( META_TYPE( R ) ),
    argArray( NULL ),
    context( META_TYPE( C ) ),
    argCount( 0 )
  {
  }

  template <typename C, typename R, typename Arg0>
  FunctionSignature( R (C::*)( Arg0 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 1 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1>
  FunctionSignature( R (C::*)( Arg0, Arg1 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 2 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 3 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 4 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 5 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 6 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 7 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 8 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 )
    };
    argArray = stat_args;
  }

  template <typename C, typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  FunctionSignature( R (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) const ) :
    ret( META_TYPE( R ) ),
    context( META_TYPE( C ) ),
    argCount( 9 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 ),
      META_TYPE( Arg8 )
    };
    argArray = stat_args;
  }

    /*----------------
      Member Fn Const
          Void

        Note:
          The C refers to context. The context is the object in which the function
          refers to, as a member function. The context is passed to a call as the first
          argument.

    ----------------*/
  template <typename C>
  FunctionSignature( void (C::*)( void ) const ) :
    ret( META_TYPE( VoidType ) ),
    argArray( NULL ),
    context( META_TYPE( C ) ),
    argCount( 0 )
  {
  }

  template <typename C, typename Arg0>
  FunctionSignature( void (C::*)( Arg0 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 1 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1>
  FunctionSignature( void (C::*)( Arg0, Arg1 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 2 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 3 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 4 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 5 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 6 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 7 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 8 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 )
    };
    argArray = stat_args;
  }

  template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  FunctionSignature( void (C::*)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) const ) :
    ret( META_TYPE( VoidType ) ),
    context( META_TYPE( C ) ),
    argCount( 9 )
  {
    static const MetaData *stat_args[] = {
      META_TYPE( Arg0 ),
      META_TYPE( Arg1 ),
      META_TYPE( Arg2 ),
      META_TYPE( Arg3 ),
      META_TYPE( Arg4 ),
      META_TYPE( Arg5 ),
      META_TYPE( Arg6 ),
      META_TYPE( Arg7 ),
      META_TYPE( Arg8 )
    };
    argArray = stat_args;
  }

    /*----------------
        Data Access
    ----------------*/
  unsigned ArgCount( void ) const;
  const MetaData *GetRet( void ) const;
  const MetaData *GetArg( unsigned i ) const;
  bool IsMemberFn( void ) const;
  const MetaData *Context( void ) const;

private:
  const MetaData *ret;
  const MetaData **argArray;
  const MetaData *context;
  const unsigned argCount;
};

/*------------------------------------------------------------------------------------------------

                              Static Function Calls with Return Value

------------------------------------------------------------------------------------------------*/
template <typename R>
void Call( R (*fn)( void ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 0 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( );
}

template <typename R, typename Arg0>
void Call( R (*fn)( Arg0 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 1 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ) );
}

template <typename R, typename Arg0, typename Arg1>
void Call( R (*fn)( Arg0, Arg1 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 2 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( )
    );
}

template <typename R, typename Arg0, typename Arg1, typename Arg2>
void Call( R (*fn)( Arg0, Arg1, Arg2 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 3 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( )
    );
}

template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Call( R (*fn)( Arg0, Arg1, Arg2, Arg3 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 4 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( )
    );
}

template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Call( R (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 5 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( )
    );
}

template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Call( R (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 6 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( )
    );
}

template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Call( R (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 7 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( )
    );
}

template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Call( R (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 8 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( )
    );
}

template <typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void Call( R (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 8 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( ),
    args[8].GetValue<RemoveConstRef<Arg8>::type>( )
    );
}

/*------------------------------------------------------------------------------------------------

                            Static Function Calls without Return Value

------------------------------------------------------------------------------------------------*/
void Call( void (*fn)( void ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount );

template <typename Arg0>
void Call( void (*fn)( Arg0 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 1 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ) );
}

template <typename Arg0, typename Arg1>
void Call( void (*fn)( Arg0, Arg1 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 2 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( )
    );
}

template <typename Arg0, typename Arg1, typename Arg2>
void Call( void (*fn)( Arg0, Arg1, Arg2 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 3 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( )
    );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Call( void (*fn)( Arg0, Arg1, Arg2, Arg3 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 4 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( )
    );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Call( void (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 5 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( )
    );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Call( void (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 6 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( )
    );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Call( void (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 7 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( )
    );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Call( void (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 8 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( )
    );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void Call( void (*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 9 );

  fn( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( ),
    args[8].GetValue<RemoveConstRef<Arg8>::type>( )
    );
}

/*------------------------------------------------------------------------------------------------

                              Member Function Calls with Return Value

------------------------------------------------------------------------------------------------*/
template <typename R, typename C>
void Call( R (C::*fn)( void ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 0 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( );
}

template <typename R, typename C, typename Arg0>
void Call( R (C::*fn)( Arg0 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 1 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ) );
}

template <typename R, typename C, typename Arg0, typename Arg1>
void Call( R (C::*fn)( Arg0, Arg1 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 2 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2>
void Call( R (C::*fn)( Arg0, Arg1, Arg2 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 3 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 4 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 5 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 6 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 7 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 8 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 9 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( ),
    args[8].GetValue<RemoveConstRef<Arg8>::type>( )
    );
}

/*------------------------------------------------------------------------------------------------

                            Member Function Calls with Return Value Const

------------------------------------------------------------------------------------------------*/
template <typename R, typename C>
void Call( R (C::*fn)( void ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 0 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( );
}

template <typename R, typename C, typename Arg0>
void Call( R (C::*fn)( Arg0 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 1 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ) );
}

template <typename R, typename C, typename Arg0, typename Arg1>
void Call( R (C::*fn)( Arg0, Arg1 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 2 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2>
void Call( R (C::*fn)( Arg0, Arg1, Arg2 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 3 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 4 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 5 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 6 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 7 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 8 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( )
    );
}

template <typename R, typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void Call( R (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 9 );

  ret.GetValue<RemoveConstRef<R>::type>( ) = CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( ),
    args[8].GetValue<RemoveConstRef<Arg8>::type>( )
    );
}

/*------------------------------------------------------------------------------------------------

                          Member Function Calls without Return Value

------------------------------------------------------------------------------------------------*/
template <typename C>
void Call( void (C::*fn)( void ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 0 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( );
}

template <typename C, typename Arg0>
void Call( void (C::*fn)( Arg0 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 1 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ) );
}

template <typename C, typename Arg0, typename Arg1>
void Call( void (C::*fn)( Arg0, Arg1 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 2 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2>
void Call( void (C::*fn)( Arg0, Arg1, Arg2 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 3 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 4 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 5 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 6 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 7 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 8 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 9 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( ),
    args[8].GetValue<RemoveConstRef<Arg8>::type>( )
    );
}

/*------------------------------------------------------------------------------------------------

                      Member Function Calls without Return Value Const

------------------------------------------------------------------------------------------------*/
template <typename C>
void Call( void (C::*fn)( void ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 0 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( );
}

template <typename C, typename Arg0>
void Call( void (C::*fn)( Arg0 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 1 );

  // If you have a mismatch for context here being C * and C, then you probably loaded a script
  // with *this, as opposed to just passing this.
  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ) );
}

template <typename C, typename Arg0, typename Arg1>
void Call( void (C::*fn)( Arg0, Arg1 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 2 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2>
void Call( void (C::*fn)( Arg0, Arg1, Arg2 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 3 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 4 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 5 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 6 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 7 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 8 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( )
    );
}

template <typename C, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void Call( void (C::*fn)( Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8 ) const, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 9 );

  CALL_PTR( binding.GetValue<C *>( ), fn )( args[0].GetValue<RemoveConstRef<Arg0>::type>( ),
    args[1].GetValue<RemoveConstRef<Arg1>::type>( ),
    args[2].GetValue<RemoveConstRef<Arg2>::type>( ),
    args[3].GetValue<RemoveConstRef<Arg3>::type>( ),
    args[4].GetValue<RemoveConstRef<Arg4>::type>( ),
    args[5].GetValue<RemoveConstRef<Arg5>::type>( ),
    args[6].GetValue<RemoveConstRef<Arg6>::type>( ),
    args[7].GetValue<RemoveConstRef<Arg7>::type>( ),
    args[8].GetValue<RemoveConstRef<Arg8>::type>( )
    );
}

/*------------------------------------------------------------------------------------------------

                                            CallHelper

------------------------------------------------------------------------------------------------*/
// Stores a function pointer for proper typecasting of the MultiFnPtr
template <typename Signature>
void CallHelper( const MultiFnPtr& fn, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  Call( *reinterpret_cast<Signature *>(&const_cast<MultiFnPtr&>(fn)), binding, ret, args, argCount );
}

// Lua function call binding
template <>
void CallHelper<const char *>( const MultiFnPtr& fn, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount );

/*------------------------------------------------------------------------------------------------

                                        VariantFunction

------------------------------------------------------------------------------------------------*/
class VariantFunction : public CtorLister<VariantFunction>
{
public:
    /*----------------
        Ctors/Dtors
    ----------------*/
  template <typename Signature> // Used only in REGISTER_FUNCTION for static construction + registration
  VariantFunction( const char *name_, Signature signature, uint32 static_construction );
  template <typename Signature> // Used during run-time, no registration going on under hood
  VariantFunction( const char *name_, Signature signature );
  VariantFunction( const char *name_, const char *signature, uint32 arg_count ); // Lua only?
  VariantFunction( );

    /*----------------
        Modifiers
    ----------------*/
  template <typename T>
  void Bind( T& self );
  template <typename T>
  void Bind( T *self );
  void BindFromLua( lua_State *L, unsigned index );
  const RefVariant& GetBinding( void ) const;
  RefVariant& GetBinding( void );
  void ClearBinding( void );
  
    /*----------------
      Functor Calls
    ----------------*/
  void operator( )( RefVariant& ret, RefVariant *args, unsigned argCount ) const;
  void operator( )( RefVariant& ret ) const;
  void operator( )( void ) const;

  // has return value
  template <typename Arg0>
  void operator( )( RefVariant& ret, Arg0& arg0 ) const;
  template <typename Arg0, typename Arg1>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1 ) const;
  template <typename Arg0, typename Arg1, typename Arg2>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  void operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7, Arg8& arg8 ) const;

  // void return
  template <typename Arg0>
  void operator( )( Arg0& arg0 ) const;
  template <typename Arg0, typename Arg1>
  void operator( )( Arg0& arg0, Arg1& arg1 ) const;
  template <typename Arg0, typename Arg1, typename Arg2>
  void operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  void operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  void operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  void operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  void operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  void operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7 ) const;
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  void operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7, Arg8& arg8 ) const;

    /*----------------
        Data Access
    ----------------*/
  const char *Name( void ) const;
  const FunctionSignature& GetSignature( void ) const;
  bool BindingIsValid( void ) const;

private:
  const char *name;
  FunctionSignature sig;
  MultiFnPtr fn;
  RefVariant binding;

  // Pointer to templated CallHelper
  void (*call)( const MultiFnPtr&, RefVariant&, RefVariant&, RefVariant *, unsigned );

  friend class MetaManager;
};

    /*----------------
        Ctors/Dtors
    ----------------*/
template <typename Signature> // Compile time constructor -- registers with MetaManager
VariantFunction::VariantFunction( const char *name_, Signature signature, uint32 static_construction ) :
  name( name_ ),
  sig( signature ),
  fn( *reinterpret_cast<MultiFnPtr *>(&signature) ),
  binding( sig.Context( ), NULL ),
  call( CallHelper<Signature> )
{
#ifdef CC_REG_EXPLICIT
  META_MANAGER->Insert( this );
#else
  MetaManager::RegisterFn( this );
#endif
}

template <typename Signature> // Non-compile time constructor -- no MetaManager registration
VariantFunction::VariantFunction( const char *name_, Signature signature ) :
  name( name_ ),
  sig( signature ),
  fn( *reinterpret_cast<MultiFnPtr *>(&signature) ),
  binding( sig.Context( ), NULL ),
  call( CallHelper<Signature> )
{
}

    /*----------------
        Modifiers
    ----------------*/
template <typename T>
void VariantFunction::Bind( T& self )
{
  binding = &self;
}

template <typename T>
void VariantFunction::Bind( T *self )
{
  binding = self;
}

    /*----------------
      Functor Calls
    ----------------*/
// has return value
template <typename Arg0>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0 ) const
{
  assert( sig.ArgCount( ) == 1 );

  RefVariant stackArgs[1];
  
  new (stackArgs) RefVariant( arg0 );

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1 ) const
{
  assert( sig.ArgCount( ) == 2 );
    
  RefVariant stackArgs[2];
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2 ) const
{
  assert( sig.ArgCount( ) == 3 );
    
  RefVariant stackArgs[3];
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3 ) const
{
  assert( sig.ArgCount( ) == 4 );
    
  RefVariant stackArgs[4];
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4 ) const
{
  assert( sig.ArgCount( ) == 5 );
    
  RefVariant stackArgs[5];
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5 ) const
{
  assert( sig.ArgCount( ) == 6 );
    
  RefVariant stackArgs[6];
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6 ) const
{
  assert( sig.ArgCount( ) == 7 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  RefVariant stackArgs[7];

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7 ) const
{
  assert( sig.ArgCount( ) == 8 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  RefVariant stackArgs[8];

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void VariantFunction::operator( )( RefVariant& ret, Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7, Arg8& arg8 ) const
{
  assert( sig.ArgCount( ) == 9 );
    
  RefVariant stackArgs[9];
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );
  new (stackArgs + 6) RefVariant( arg6 );
  new (stackArgs + 7) RefVariant( arg7 );
  new (stackArgs + 8) RefVariant( arg8 );

  call( fn, const_cast<RefVariant &>(binding), ret, stackArgs, sig.ArgCount( ) );
}

// no return value
template <typename Arg0>
void VariantFunction::operator( )( Arg0& arg0 ) const
{
  assert( sig.ArgCount( ) == 1 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1 ) const
{
  assert( sig.ArgCount( ) == 2 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2 ) const
{
  assert( sig.ArgCount( ) == 3 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3 ) const
{
  assert( sig.ArgCount( ) == 4 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4 ) const
{
  assert( sig.ArgCount( ) == 5 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5 ) const
{
  assert( sig.ArgCount( ) == 6 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6 ) const
{
  assert( sig.ArgCount( ) == 7 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );
  new (stackArgs + 6) RefVariant( arg6 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7 ) const
{
  assert( sig.ArgCount( ) == 8 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );
  new (stackArgs + 6) RefVariant( arg6 );
  new (stackArgs + 7) RefVariant( arg7 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void VariantFunction::operator( )( Arg0& arg0, Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6, Arg7& arg7, Arg8& arg8 ) const
{
  assert( sig.ArgCount( ) == 9 );

  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * sig.ArgCount( ) ));
  
  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );
  new (stackArgs + 6) RefVariant( arg6 );
  new (stackArgs + 7) RefVariant( arg7 );
  new (stackArgs + 8) RefVariant( arg8 );

  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), stackArgs, sig.ArgCount( ) );
}
