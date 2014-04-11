/* Copyright (c) 2007 John W Wilkinson

   This source code can be used for any purpose as long as
   this comment is retained. */

#include "json_spirit_writer_test.h"
#include "json_spirit_writer.h"
#include "json_spirit_value.h" 
#include "Utils_test.h"

#include <sstream>

using namespace json_spirit;
using namespace std;

namespace
{
    void test_to_ascii( const Value& value, const string& expected_result )
    {
        ostringstream os;

        write( value, os );

        cout << os.str() << endl;
        cout << expected_result << endl;

        assert_eq( os.str(), expected_result );
    }

    void test_to_pretty_ascii( const Value& value, const string& expected_result )
    {
        ostringstream os;

        write_formatted( value, os );

        cout << os.str() << endl;
        cout << expected_result << endl;

        assert_eq( os.str(), expected_result );
    }

    void test_empty_obj()
    {
        test_to_ascii( Object(), "{}" );
        test_to_pretty_ascii( Object(), "{\n"
                                        "}" );
    }

    void test_obj_with_one_member()
    {
        Object obj;

        obj.push_back( Pair( "name", "value" ) );

        test_to_ascii       ( obj, "{\"name\":\"value\"}" );
        test_to_pretty_ascii( obj, "{\n"
                                   "    \"name\" : \"value\"\n"
                                   "}" );
    }

    void test_obj_with_two_members()
    {
        Object obj;

        obj.push_back( Pair( "name_1", "value_1" ) );
        obj.push_back( Pair( "name_2", "value_2" ) );

        test_to_ascii( obj, "{\"name_1\":\"value_1\",\"name_2\":\"value_2\"}" );

        test_to_pretty_ascii( obj, "{\n"
                                   "    \"name_1\" : \"value_1\",\n"
                                   "    \"name_2\" : \"value_2\"\n"
                                   "}" );
    }

    void test_obj_with_three_members()
    {
        Object obj;

        obj.push_back( Pair( "name_1", "value_1" ) );
        obj.push_back( Pair( "name_2", "value_2" ) );
        obj.push_back( Pair( "name_3", "value_3" ) );

        test_to_ascii( obj, "{\"name_1\":\"value_1\",\"name_2\":\"value_2\",\"name_3\":\"value_3\"}" );

        test_to_pretty_ascii( obj, "{\n"
                                   "    \"name_1\" : \"value_1\",\n"
                                   "    \"name_2\" : \"value_2\",\n"
                                   "    \"name_3\" : \"value_3\"\n"
                                   "}" );
    }

    void test_obj_with_one_empty_child_obj()
    {
        Object child;

        Object root;

        root.push_back( Pair( "child", child ) );

        test_to_ascii( root, "{\"child\":{}}" );

        test_to_pretty_ascii( root, "{\n"
                                    "    \"child\" : {\n"
                                    "    }\n"
                                    "}" );
    }

    void test_obj_with_one_child_obj()
    {
        Object child;

        child.push_back( Pair( "name_2", "value_2" ) );

        Object root;

        root.push_back( Pair( "name_1", "value_1" ) );
        root.push_back( Pair( "child", child ) );

        test_to_ascii( root, "{\"name_1\":\"value_1\",\"child\":{\"name_2\":\"value_2\"}}" );

        test_to_pretty_ascii( root, "{\n"
                                    "    \"name_1\" : \"value_1\",\n"
                                    "    \"child\" : {\n"
                                    "        \"name_2\" : \"value_2\"\n"
                                    "    }\n"
                                    "}" );
    }

    void test_obj_with_grandchild_obj()
    {
        Object child_1; child_1.push_back( Pair( "name_1", "value_1" ) );
        Object child_2; child_2.push_back( Pair( "name_2", "value_2" ) );
        Object child_3; child_3.push_back( Pair( "name_3", "value_3" ) );

        child_2.push_back( Pair( "grandchild", child_3 ) );

        Object root;

        root.push_back( Pair( "name_a", "value_a" ) );
        root.push_back( Pair( "child_1", child_1  ) );
        root.push_back( Pair( "child_2", child_2  ) );
        root.push_back( Pair( "name_b", "value_b" ) );

        test_to_ascii( root, "{\"name_a\":\"value_a\","
                              "\"child_1\":{\"name_1\":\"value_1\"},"
                              "\"child_2\":{\"name_2\":\"value_2\",\"grandchild\":{\"name_3\":\"value_3\"}},"
                              "\"name_b\":\"value_b\"}" );

        test_to_pretty_ascii( root, "{\n"
                                    "    \"name_a\" : \"value_a\",\n"
                                    "    \"child_1\" : {\n"
                                    "        \"name_1\" : \"value_1\"\n"
                                    "    },\n"
                                    "    \"child_2\" : {\n"
                                    "        \"name_2\" : \"value_2\",\n"
                                    "        \"grandchild\" : {\n"
                                    "            \"name_3\" : \"value_3\"\n"
                                    "        }\n"
                                    "    },\n"
                                    "    \"name_b\" : \"value_b\"\n"
                                    "}" );
    }

    void test_objs_with_bool_pairs()
    {
        Object obj;

        obj.push_back( Pair( "name_1", true  ) );
        obj.push_back( Pair( "name_2", false ) );
        obj.push_back( Pair( "name_3", true  ) );
 
        test_to_ascii( obj, "{\"name_1\":true,\"name_2\":false,\"name_3\":true}" );
    }

    void test_objs_with_int_pairs()
    {
        Object obj;

        obj.push_back( Pair( "name_1", 11 ) );
        obj.push_back( Pair( "name_2", 22 ) );
        obj.push_back( Pair( "name_3", 33 ) );
 
        test_to_ascii( obj, "{\"name_1\":11,\"name_2\":22,\"name_3\":33}" );
    }

    void test_objs_with_real_pairs()
    {
        Object obj;

        obj.push_back( Pair( "name_1", 1.0 ) );
        obj.push_back( Pair( "name_2", 123.456e-78 ) );
        obj.push_back( Pair( "name_3", -1e-6 ) );
 
        test_to_ascii( obj, "{\"name_1\":1,\"name_2\":1.23456e-076,\"name_3\":-1e-006}" );
    }

    void test_objs_with_null_pairs()
    {
        Object obj;

        obj.push_back( Pair( "name_1", Value::null ) );
        obj.push_back( Pair( "name_2", Value::null ) );
        obj.push_back( Pair( "name_3", Value::null ) );
 
        test_to_ascii( obj, "{\"name_1\":null,\"name_2\":null,\"name_3\":null}" );
    }

    void test_empty_array()
    {
        test_to_ascii( Array(), "[]" );
        test_to_pretty_ascii( Array(), "[\n"
                                       "]" );
    }

    void test_array_with_one_member()
    {
        Array arr;

        arr.push_back( "value" );

        test_to_ascii       ( arr, "[\"value\"]" );
        test_to_pretty_ascii( arr, "[\n"
                                   "    \"value\"\n"
                                   "]" );
    }

    void test_array_with_two_members()
    {
        Array arr;

        arr.push_back( "value_1" );
        arr.push_back( 1 );

        test_to_ascii       ( arr, "[\"value_1\",1]" );
        test_to_pretty_ascii( arr, "[\n"
                                   "    \"value_1\",\n"
                                   "    1\n"
                                   "]" );
    }

    void test_array_with_n_members()
    {
        Array arr;

        arr.push_back( "value_1" );
        arr.push_back( 123 );
        arr.push_back( 123.456 );
        arr.push_back( true );
        arr.push_back( false );
        arr.push_back( Value() );

        test_to_ascii       ( arr, "[\"value_1\",123,123.456,true,false,null]" );
        test_to_pretty_ascii( arr, "[\n"
                                   "    \"value_1\",\n"
                                   "    123,\n"
                                   "    123.456,\n" 
                                   "    true,\n"
                                   "    false,\n"
                                   "    null\n"
                                   "]" );
    }

    void test_array_with_one_empty_child_array()
    {
        Array arr;

        arr.push_back( Array() );

        test_to_ascii       ( arr, "[[]]" );
        test_to_pretty_ascii( arr, "[\n"
                                   "    [\n"
                                   "    ]\n"
                                   "]" );
    }

    void test_array_with_one_child_array()
    {
        Array child;

        child.push_back( 2 );

        Array root;

        root.push_back( 1 );
        root.push_back( child );

        test_to_ascii       ( root, "[1,[2]]" );
        test_to_pretty_ascii( root, "[\n"
                                    "    1,\n"
                                    "    [\n"
                                    "        2\n"
                                    "    ]\n"
                                    "]" );
    }

    void test_array_with_grandchild_array()
    {
        Array child_1; child_1.push_back( 11 );
        Array child_2; child_2.push_back( 22 );
        Array child_3; child_3.push_back( 33 );

        child_2.push_back( child_3 );

        Array root;

        root.push_back( 1);
        root.push_back(  child_1 );
        root.push_back(  child_2 );
        root.push_back( 2 );

        test_to_ascii       ( root, "[1,[11],[22,[33]],2]" );
        test_to_pretty_ascii( root, "[\n"
                                    "    1,\n"
                                    "    [\n"
                                    "        11\n"
                                    "    ],\n"
                                    "    [\n"
                                    "        22,\n"
                                    "        [\n"
                                    "            33\n"
                                    "        ]\n"
                                    "    ],\n"
                                    "    2\n"
                                    "]" );
    }

    void test_array_and_objs()
    {
        Array a;

        a.push_back( 11 );

        Object obj;

        obj.push_back( Pair( "a", 1 ) );

        a.push_back( obj );

        test_to_ascii       ( a, "[11,{\"a\":1}]" );
        test_to_pretty_ascii( a, "[\n"
                                 "    11,\n"
                                 "    {\n"
                                 "        \"a\" : 1\n"
                                 "    }\n"
                                 "]" );

        obj.push_back( Pair( "b", 2 ) );

        a.push_back( 22 );
        a.push_back( 33 );
        a.push_back( obj );

        test_to_ascii       ( a, "[11,{\"a\":1},22,33,{\"a\":1,\"b\":2}]" );
        test_to_pretty_ascii( a, "[\n"
                                 "    11,\n"
                                 "    {\n"
                                 "        \"a\" : 1\n"
                                 "    },\n"
                                 "    22,\n"
                                 "    33,\n"
                                 "    {\n"
                                 "        \"a\" : 1,\n"
                                 "        \"b\" : 2\n"
                                 "    }\n"
                                 "]" );
    }

    void test_obj_and_arrays()
    {
        Object obj;

        obj.push_back( Pair( "a", 1 ) );

        Array a;

        a.push_back( 11 );

        obj.push_back( Pair( "b", a ) );

        test_to_ascii       ( obj, "{\"a\":1,\"b\":[11]}" );
        test_to_pretty_ascii( obj, "{\n"
                                   "    \"a\" : 1,\n"
                                   "    \"b\" : [\n"
                                   "        11\n"
                                   "    ]\n"
                                   "}" );

        a.push_back( obj );

        obj.push_back( Pair( "c", a ) );

        test_to_ascii       ( obj, "{\"a\":1,\"b\":[11],\"c\":[11,{\"a\":1,\"b\":[11]}]}" );
        test_to_pretty_ascii( obj, "{\n"
                                   "    \"a\" : 1,\n"
                                   "    \"b\" : [\n"
                                   "        11\n"
                                   "    ],\n"
                                   "    \"c\" : [\n"
                                   "        11,\n"
                                   "        {\n"
                                   "            \"a\" : 1,\n"
                                   "            \"b\" : [\n"
                                   "                11\n"
                                   "            ]\n"
                                   "        }\n"
                                   "    ]\n"
                                   "}" );
    }

    void test_to_ascii()
    {
        test_empty_obj();
        test_obj_with_one_member();
        test_obj_with_two_members();
        test_obj_with_three_members();
        test_obj_with_one_empty_child_obj();
        test_obj_with_one_child_obj();
        test_obj_with_grandchild_obj();
        test_objs_with_bool_pairs();
        test_objs_with_int_pairs();
        test_objs_with_real_pairs();
        test_objs_with_null_pairs();
        test_empty_array();
        test_array_with_one_member();
        test_array_with_two_members();
        test_array_with_n_members();
        test_array_with_one_empty_child_array();
        test_array_with_one_child_array();
        test_array_with_grandchild_array();
        test_array_and_objs();
        test_obj_and_arrays();
    }
}

void json_spirit::test_writer()
{
    test_to_ascii();
}
