/* Copyright (c) 2007 John W Wilkinson

   This source code can be used for any purpose as long as
   this comment is retained. */

#include "json_spirit_reader_test.h"
#include "json_spirit_reader.h"
#include "json_spirit_value.h" 
#include "json_spirit_writer.h" 
#include "Utils_test.h"

#include <sstream>
#include <boost/assign/list_of.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>

using namespace json_spirit;
using namespace std;
using namespace boost;
using namespace boost::assign;

namespace
{
    void check_eq( const Object& obj_1, const Object& obj_2 )
    {
        const Object::size_type size( obj_1.size() );

        assert_eq( size, obj_2.size() );

        for( Object::size_type i = 0; i < size; ++i )
        {
            assert_eq( obj_1[i], obj_2[i] ); 
        }
    }

    void test_syntax( const string& s )
    {
        Value value;

        const bool success = read( s, value );

        assert_eq( success, true );
    }

    void test_syntax()
    {
        test_syntax( "{}" );
        test_syntax( "{ }" );
        test_syntax( "{\"\":\"\"}" );
        test_syntax( "{\"test\":\"123\"}" );
        test_syntax( "{\"test\" : \"123\"}" );
        test_syntax( "{\"testing testing testing\":\"123\"}" );
        test_syntax( "{\"\":\"abc\"}" );
        test_syntax( "{\"abc\":\"\"}" );
        test_syntax( "{\"\":\"\"}" );
        test_syntax( "{\"test\":true}" );
        test_syntax( "{\"test\":false}" );
        test_syntax( "{\"test\":null}" );
        test_syntax( "{\"test1\":\"123\",\"test2\":\"456\"}" );
        test_syntax( "{\"test1\":\"123\",\"test2\":\"456\",\"test3\":\"789\"}" );
        test_syntax( "{\"test1\":{\"test2\":\"123\",\"test3\":\"456\"}}" );
        test_syntax( "{\"test1\":{\"test2\":{\"test3\":\"456\"}}}" );
        test_syntax( "{\"test1\":[\"a\",\"bb\",\"cc\"]}" );
        test_syntax( "{\"test1\":[true,false,null]}" );
        test_syntax( "{\"test1\":[true,\"abc\",{\"a\":\"b\"},{\"d\":false},null]}" );
        test_syntax( "{\"test1\":[1,2,-3]}" );
        test_syntax( "{\"test1\":[1.1,2e4,-1.234e-34]}" );
        test_syntax( "{\n"
                      "\t\"test1\":\n"
                      "\t\t{\n"
                      "\t\t\t\"test2\":\"123\",\n"
                      "\t\t\t\"test3\":\"456\"\n"
                      "\t\t}\n"
                      "}\n" );
        test_syntax( "[]" );
        test_syntax( "[ ]" );
        test_syntax( "[1,2,3]" );
        test_syntax( "[ 1, -2, 3]" );
        test_syntax( "[ 1.2, -2e6, -3e-6 ]" );
        test_syntax( "[ 1.2, \"str\", -3e-6, { \"field\" : \"data\" } ]" );
    }

    const Pair p1( "name 1", "value 1" );
    const Pair p2( "name 2", "value 2" );
    const Pair p3( "name 3", "value 3" );

    void check_read( istream& is, Value& value )
    {
        const bool success = read( is, value );

        assert_eq( success, true );
    }

    void check_read( const string& s, Value& value )
    {
        const bool success = read( s, value );

        assert_eq( success, true );
    }

    void check_reading( const string& s )
    {
        static Value value;

        istringstream is( s );

        const bool success = read( is, value );

        assert_eq( success, true );

        cout << s << endl << write_formatted( value )<< endl ;

        assert_eq( s, write_formatted( value ) );
    }

    void test_reading()
    {
        check_reading( "{\n}" );

        Value value;

        istringstream is1( "{\n"
                           "    \"name 1\" : \"value 1\"\n"
                           "}" );

        check_read( is1, value );

        check_eq( value.get_obj(), list_of( p1 ) );

        istringstream is2( "{\"name 1\":\"value 1\",\"name 2\":\"value 2\"}"  );

        check_read( is2, value );

        check_eq( value.get_obj(), list_of( p1 )( p2 ) );

        check_read( "{\n"
                    "    \"name 1\" : \"value 1\",\n"
                    "    \"name 2\" : \"value 2\",\n"
                    "    \"name 3\" : \"value 3\"\n"
                    "}", value );

        check_eq( value.get_obj(), list_of( p1 )( p2 )( p3 ) );

        check_read( "{\n"
                    "    \"\" : \"value\",\n"
                    "    \"name\" : \"\"\n"
                    "}", value );

        check_eq( value.get_obj(), list_of( Pair( "", "value" ) )( Pair( "name", "" ) ) );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : {\n"
                        "        \"name 3\" : \"value 3\",\n"
                        "        \"name_4\" : \"value_4\"\n"
                        "    }\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : {\n"
                        "        \"name 3\" : \"value 3\",\n"
                        "        \"name_4\" : \"value_4\",\n"
                        "        \"name_5\" : {\n"
                        "            \"name_6\" : \"value_6\",\n"
                        "            \"name_7\" : \"value_7\"\n"
                        "        }\n"
                        "    }\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : {\n"
                        "        \"name 3\" : \"value 3\",\n"
                        "        \"name_4\" : {\n"
                        "            \"name_5\" : \"value_5\",\n"
                        "            \"name_6\" : \"value_6\"\n"
                        "        },\n"
                        "        \"name_7\" : \"value_7\"\n"
                        "    }\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : {\n"
                        "        \"name 3\" : \"value 3\",\n"
                        "        \"name_4\" : {\n"
                        "            \"name_5\" : \"value_5\",\n"
                        "            \"name_6\" : \"value_6\"\n"
                        "        },\n"
                        "        \"name_7\" : \"value_7\"\n"
                        "    },\n"
                        "    \"name_8\" : \"value_8\",\n"
                        "    \"name_9\" : {\n"
                        "        \"name_10\" : \"value_10\"\n"
                        "    }\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : {\n"
                        "        \"name 2\" : {\n"
                        "            \"name 3\" : {\n"
                        "                \"name_4\" : {\n"
                        "                    \"name_5\" : \"value\"\n"
                        "                }\n"
                        "            }\n"
                        "        }\n"
                        "    }\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : true,\n"
                        "    \"name 3\" : false,\n"
                        "    \"name_4\" : \"value_4\",\n"
                        "    \"name_5\" : true\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : null,\n"
                        "    \"name 3\" : \"value 3\",\n"
                        "    \"name_4\" : null\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : 123,\n"
                        "    \"name 3\" : \"value 3\",\n"
                        "    \"name_4\" : -567\n"
                        "}" );

        check_reading( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : 1.2,\n"
                        "    \"name 3\" : \"value 3\",\n"
                        "    \"name_4\" : 1.234e+025,\n"
                        "    \"name_5\" : -1.234e-123,\n"
                        "    \"name_6\" : 1e-123\n"
                        "}" );

        check_reading( "[\n]" );

        check_reading( "[\n"
                       "    1\n"
                       "]" );

        check_reading( "[\n"
                       "    1,\n"
                       "    1.2,\n"
                       "    \"john\",\n"
                       "    true,\n"
                       "    false,\n"
                       "    null\n"
                       "]" );

        check_reading( "[\n"
                       "    1,\n"
                       "    [\n"
                       "        2,\n"
                       "        3\n"
                       "    ]\n"
                       "]" );

        check_reading( "[\n"
                       "    1,\n"
                       "    [\n"
                       "        2,\n"
                       "        3\n"
                       "    ],\n"
                       "    [\n"
                       "        4,\n"
                       "        [\n"
                       "            5,\n"
                       "            6,\n"
                       "            7\n"
                       "        ]\n"
                       "    ]\n"
                       "]" );

        check_reading( "[\n"
                       "    {\n"
                       "        \"name\" : \"value\"\n"
                       "    }\n"
                       "]" );

        check_reading( "{\n"
                       "    \"name\" : [\n"
                       "        1\n"
                       "    ]\n"
                       "}" );

        check_reading( "[\n"
                       "    {\n"
                       "        \"name 1\" : \"value\",\n"
                       "        \"name 2\" : [\n"
                       "            1,\n"
                       "            2,\n"
                       "            3\n"
                       "        ]\n"
                       "    }\n"
                       "]" );

        check_reading( "{\n"
                       "    \"name 1\" : [\n"
                       "        1,\n"
                       "        {\n"
                       "            \"name 2\" : \"value 2\"\n"
                       "        }\n"
                       "    ]\n"
                       "}" );

        check_reading( "[\n"
                       "    {\n"
                       "        \"name 1\" : \"value 1\",\n"
                       "        \"name 2\" : [\n"
                       "            1,\n"
                       "            2,\n"
                       "            {\n"
                       "                \"name 3\" : \"value 3\"\n"
                       "            }\n"
                       "        ]\n"
                       "    }\n"
                       "]" );

        check_reading( "{\n"
                       "    \"name 1\" : [\n"
                       "        1,\n"
                       "        {\n"
                       "            \"name 2\" : [\n"
                       "                1,\n"
                       "                2,\n"
                       "                3\n"
                       "            ]\n"
                       "        }\n"
                       "    ]\n"
                       "}" );
    }

    void test_escape_chars()
    {
        Value value;

        istringstream is( "{\n"
                          "    \"a\\tb\" : \"c\\nd\"\n"
                          "}" );

        check_read( is, value );

        assert_eq( value.get_obj()[0].name_, "a\tb" );
        assert_eq( value.get_obj()[0].value_, "c\nd" );
    }
}

void json_spirit::test_reader()
{
    test_syntax();
    test_reading();
    test_escape_chars();

    //Object obj;

    //for( int i = 0; i < 100000; ++i )
    //{
    //    obj.push_back( Pair( "test", lexical_cast< string >( i ) ) );
    //}

    //string s = write( obj );

    //Value value;

    //timer t;

    //read( s, value );

    //cout << t.elapsed() << endl;

    //cout << "obj size " << value.get_obj().size();
}
