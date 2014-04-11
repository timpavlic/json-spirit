/* Copyright (c) 2007-2008 John W Wilkinson

   This source code can be used for any purpose as long as
   this comment is retained. */

// json spirit version 2.05

#include "json_spirit_value_test.h"
#include "json_spirit_value.h"
#include "utils_test.h"

using namespace json_spirit;
using namespace std;

namespace
{
    void test_obj_value()
    {
        const Pair p1( "name1", "value1" );
        const Pair p3( "name3", "value3" );

        Object obj_1; obj_1.push_back( p1 );
        Object obj_2; obj_2.push_back( p1 );
        Object obj_3; obj_3.push_back( p3 );

        Value v1( obj_1 );
        Value v2( obj_2 );
        Value v3( obj_3 );

        assert_eq( v1.type(), obj_type );
        assert_eq ( v1, v2 );
        assert_neq( v1, v3 );

        assert_eq( v1.get_obj(), obj_1 );
        assert_eq( v3.get_obj(), obj_3 );
    }

    void test_array_value()
    {
        Array array_1; array_1.push_back( 1 ); array_1.push_back( "2" );
        Array array_2; array_2.push_back( 1 ); array_2.push_back( "2" );
        Array array_3; array_3.push_back( 1 ); array_3.push_back( "X" );

        Value v1( array_1 );
        Value v2( array_2 );
        Value v3( array_3 );

        assert_eq( v1.type(), array_type );
        assert_eq ( v1, v2 );
        assert_neq( v1, v3 );

        assert_eq( v1.get_array(), array_1 );
        assert_eq( v3.get_array(), array_3 );
    }

    void test_bool_value()
    {
        Value v1( true );
        Value v2( true );
        Value v3( false );

        assert_eq( v1.type(), bool_type );
        assert_eq ( v1, v2 );
        assert_neq( v1, v3 );

        assert( v1.get_bool() );
        assert( !v3.get_bool() );
    }

    void test_int_value()
    {
        Value v1( 1 );
        Value v2( 1 );
        Value v3( INT_MAX );

        assert_eq( v1.type(), int_type );
        assert_eq ( v1, v2 );
        assert_neq( v1, v3 );

        assert_eq( v1.get_int(), 1 );
        assert_eq( v3.get_int(), INT_MAX );
        assert_eq( v1.get_int64(), 1 );
        assert_eq( v3.get_int64(), INT_MAX );

        Value v4( LLONG_MAX );

        assert_eq( v4.get_int64(), LLONG_MAX );
    }

    void test_real_value()
    {
        Value v1( 1.0 );
        Value v2( 1.0 );
        Value v3( 2.0 );

        assert_eq( v1.type(), real_type );
        assert_eq ( v1, v2 );
        assert_neq( v1, v3 );

        assert_eq( v1.get_real(), 1.0 );
        assert_eq( v3.get_real(), 2.0 );
    }

    void test_null_value()
    {
        Value v1;
        Value v2;

        assert_eq( v1.type(), null_type );
        assert_eq ( v1, v2 );
    }

    template< typename T >
    void test_get_value( const T& t )
    {
        assert_eq( Value( t ).get_value< T >(), t );
    }

    void test_get_value()
    {
        test_get_value( 123 );
        test_get_value( LLONG_MAX );
        test_get_value( 1.23 );
        test_get_value( true );
        test_get_value( false );
        test_get_value( string( "test" ) );

        Array a; a.push_back( 1 ); a.push_back( "2" );

        test_get_value( a );

        Object obj; obj.push_back( Pair( "name1", "value1" ) );

        test_get_value( obj );
    }
}

void json_spirit::test_value()
{
    Object obj;
    Value value_str ( "value" );
    Value value_obj ( obj );
    Value value_bool( true );

    Value value_str_2 ( string( "value" ) );
    Value value_obj_2 ( obj );
    Value value_bool_2( false );

    const char* str( "value" );
    Value value_str_2b ( str );

    assert_eq( value_str, value_str );
    assert_eq( value_str, value_str_2 );
    assert_eq( value_str, value_str_2b );
    assert_eq( value_obj, value_obj );
    assert_eq( value_obj, value_obj_2 );
    assert_neq( value_str, value_obj );
    assert_neq( value_str, value_bool );

    Object obj_2;
    obj_2.push_back( Pair( "name", value_str ) );
    Value value_str_3( "xxxxx" );
    Value value_obj_3( obj_2 );

    assert_neq( value_str, value_str_3 );
    assert_neq( value_obj, value_obj_3 );

    test_obj_value();
    test_array_value();
    test_bool_value();
    test_int_value();
    test_real_value();
    test_null_value();
    test_get_value();
}

