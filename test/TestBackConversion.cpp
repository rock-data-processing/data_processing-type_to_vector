/** \file TestBackConversion.cpp
 *
 * Tests for converting vectorized data back into a type.
 */
#include <boost/test/auto_unit_test.hpp>

#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#include "TestSuite.hpp"

#include "BackConverter.hpp"
#include "VectorTocMaker.hpp"

#include "TestTypes.h"


using namespace Typelib;
using namespace type_to_vector;

BOOST_AUTO_TEST_CASE(test_backconvert_scalar)
{
    Registry registry;
    import_types(registry);

    {
        VectorToc toc = VectorTocMaker().apply(*registry.get("/double"));

        std::vector<double> dbl_vec;
        dbl_vec.push_back(1.4);

        double f = 0.0;
        FlatBackConverter fbctv(toc);
        fbctv.apply(dbl_vec, &f);
        BOOST_TEST_CHECKPOINT("FlatBackConverter for double");
        BOOST_CHECK( f == 1.4 );

        f = 0.0;
        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &f);
        BOOST_TEST_CHECKPOINT("FlatBackConverter for double");
        BOOST_CHECK( f == 1.4 );
    }

    {
        VectorToc toc = VectorTocMaker().apply(*registry.get("/int"));

        std::vector<double> dbl_vec;
        dbl_vec.push_back(3);

        int i = 0;
        FlatBackConverter fbctv(toc);
        fbctv.apply(dbl_vec, &i);
        BOOST_TEST_CHECKPOINT("FlatBackConverter for int");
        BOOST_CHECK( i == 3 );

        i = 0;
        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &i);
        BOOST_TEST_CHECKPOINT("BackConverter for int");
        BOOST_CHECK( i == 3 );
    }

    {
        VectorToc toc = VectorTocMaker().apply(*registry.get("/char"));

        std::vector<double> dbl_vec;
        dbl_vec.push_back(5);

        char c = 0;
        FlatBackConverter fbctv(toc);
        fbctv.apply(dbl_vec, &c);
        BOOST_TEST_CHECKPOINT("FlatBackConverter for char");
        BOOST_CHECK( c == 5 );

        c = 0;
        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &c);
        BOOST_TEST_CHECKPOINT("BackConverter for char");
        BOOST_CHECK( c == 5 );
    }

}

BOOST_AUTO_TEST_CASE( test_backconvert_array )
{
    Registry registry;
    import_types(registry);

    registry.build("/double[3]");
    VectorToc toc = VectorTocMaker().apply(*registry.get("/double[3]"));

    double d[] = { 1.4, -123.2, 54.8 };
    std::vector<double> dbl_vec;
    for (int i=0; i<3; i++) dbl_vec.push_back(d[i]);

    for (int i=0; i<3; i++) d[i] = 0.0;
    FlatBackConverter fbctv(toc);
    fbctv.apply(dbl_vec, d);
    BOOST_TEST_CHECKPOINT("FlatBackConverter for double[3]");
    for (int i=0; i<3; i++) BOOST_CHECK( dbl_vec[i] == d[i] );

    for (int i=0; i<3; i++) d[i] = 0.0;
    BackConverter bctv(toc, registry);
    bctv.apply(dbl_vec, d);
    BOOST_TEST_CHECKPOINT("BackConverter for double[3]");
    for (int i=0; i<3; i++) BOOST_CHECK( dbl_vec[i] == d[i] );
}

BOOST_AUTO_TEST_CASE( test_backconvert_struct )
{
    Registry registry;
    import_types(registry);

    VectorToc toc = VectorTocMaker().apply(*registry.get("/A"));

    struct A a = { 100, -23, 'c' , 12 };
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(a.a));
    dbl_vec.push_back(double(a.b));
    dbl_vec.push_back(double(a.c));
    dbl_vec.push_back(double(a.d));

    struct A get_back_flat;
    FlatBackConverter fbctv(toc);
    fbctv.apply(dbl_vec, &get_back_flat);
    BOOST_TEST_CHECKPOINT("FlatBackConverter for struct");
    BOOST_CHECK( get_back_flat.equals(a) );

    struct A get_back;
    BackConverter bctv(toc, registry);
    bctv.apply(dbl_vec, &get_back);
    BOOST_TEST_CHECKPOINT("BackConverter for struct");
    BOOST_CHECK( get_back.equals(a) );
}

BOOST_AUTO_TEST_CASE( test_backconvert_multi_struct )
{
    Registry registry;
    import_types(registry);

    VectorToc toc = VectorTocMaker().apply(*registry.get("/B"));

    struct B b = { '0', { 100, -23, 'c', 12 } };
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(b.a));
    dbl_vec.push_back(double(b.b.a));
    dbl_vec.push_back(double(b.b.b));
    dbl_vec.push_back(double(b.b.c));
    dbl_vec.push_back(double(b.b.d));

    struct B get_back_flat;
    FlatBackConverter fbctv(toc);
    fbctv.apply(dbl_vec, &get_back_flat);
    BOOST_TEST_CHECKPOINT("FlatBackConverter for multi-struct");
    BOOST_CHECK( get_back_flat.equals(b) );

    struct B get_back;
    BackConverter bctv(toc, registry);
    bctv.apply(dbl_vec, &get_back);
    BOOST_TEST_CHECKPOINT("BackConverter for multi-struct");
    BOOST_CHECK( get_back.equals(b) );
}

BOOST_AUTO_TEST_CASE( test_backconvert_container )
{
    Registry registry;
    import_types(registry);

    VectorToc toc = VectorTocMaker().apply(*registry.get("/std/vector</int>"));

    std::vector<int> int_vec;
    int_vec.push_back(-10);
    int_vec.push_back(22);
    int_vec.push_back(3);

    std::vector<double> dbl_vec(int_vec.begin(), int_vec.end());

    {
        std::vector<int> i2_vec;
        i2_vec.push_back(20);
        i2_vec.push_back(-12);

        FlatBackConverter fbctv(toc);
        fbctv.apply(dbl_vec, &i2_vec);
        BOOST_TEST_CHECKPOINT("FlatBack does not touch a container");
        BOOST_CHECK( i2_vec.size() == 2);
        BOOST_CHECK( i2_vec[0] == 20);
        BOOST_CHECK( i2_vec[1] == -12);
    }
    {
        std::vector<int> i2_vec;

        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &i2_vec);
        BOOST_TEST_CHECKPOINT("BackConvert a container, resize");
        BOOST_CHECK( i2_vec.empty() );
    }
    {
        std::vector<int> i2_vec;
        i2_vec.push_back(20);
        i2_vec.push_back(-12);

        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &i2_vec);
        BOOST_TEST_CHECKPOINT("BackConvert a container and keep smaller size");
        BOOST_CHECK( i2_vec.size() == 2);
        BOOST_CHECK( i2_vec[0] == -10);
        BOOST_CHECK( i2_vec[1] == 22);
    }
    {
        std::vector<int> i2_vec;
        i2_vec.push_back(20);
        i2_vec.push_back(-12);
        i2_vec.push_back(133);
        i2_vec.push_back(-2041);

        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &i2_vec);
        BOOST_TEST_CHECKPOINT("BackConvert a container and keep bigger size");
        BOOST_CHECK( i2_vec.size() == 4);
        BOOST_CHECK( i2_vec[0] == -10);
        BOOST_CHECK( i2_vec[1] == 22);
        BOOST_CHECK( i2_vec[2] == 3);
        BOOST_CHECK( i2_vec[3] == -2041);
    }
}

BOOST_AUTO_TEST_CASE( test_backconvert_string )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/std/string"));

    std::string str = "Hello world!";
    std::vector<double> dbl_vec(str.begin(), str.end());

    BackConverter bctv(toc, registry);

    for (int i = 0; i <= 15; i += 3) {
        std::string str2;
        if (i > 0) str2.resize(i,'x');

        bctv.apply(dbl_vec, &str2);
        BOOST_TEST_CHECKPOINT("BackConvert a string size 12 into a string size " << i);
        if (i == 0)
            BOOST_CHECK( str2.empty() );
        else if (i > 0 && i <= str.size())
            BOOST_CHECK( str2 == str.substr(0,i));
        else
            BOOST_CHECK( str2.substr(0,12) == str);
    }
}

BOOST_AUTO_TEST_CASE( test_backconvert_vector_in_struct )
{
    Registry registry;
    import_types(registry);

    VectorToc toc = VectorTocMaker().apply(*registry.get("/DoubleVector"));

    DoubleVector dv;
    dv.a = 23;
    dv.dbl_vector.push_back(-123456.789);
    dv.dbl_vector.push_back(1.4);

    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(dv.a));
    for (int i=0; i<dv.dbl_vector.size(); i++)
        dbl_vec.push_back(double(dv.dbl_vector[i]));

    {
        DoubleVector dv_test;
        dv_test.a = 0;
        dv_test.dbl_vector.resize(3, 1.0);

        FlatBackConverter fbctv(toc);
        fbctv.apply(dbl_vec, &dv_test);
        BOOST_TEST_CHECKPOINT("FlatBackConvert a container inside a struct");
        BOOST_CHECK( dv_test.a == 23);
        BOOST_CHECK( dv_test.dbl_vector.size() == 3);
        BOOST_CHECK( dv_test.dbl_vector[0] == 1.0);
        BOOST_CHECK( dv_test.dbl_vector[1] == 1.0);
        BOOST_CHECK( dv_test.dbl_vector[2] == 1.0);
    }

    {
        DoubleVector dv_test;
        dv_test.dbl_vector.resize(2);
        dv_test.a = 0;

        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &dv_test);
        BOOST_TEST_CHECKPOINT("BackConvert a container inside a struct");
        BOOST_CHECK( dv_test.a == 23);
        BOOST_REQUIRE( dv_test.dbl_vector.size() == 2);
        BOOST_CHECK( dv_test.dbl_vector[0] == dv.dbl_vector[0]);
        BOOST_CHECK( dv_test.dbl_vector[1] == dv.dbl_vector[1]);
    }
}

BOOST_AUTO_TEST_CASE( test_backconvert_array_of_containers )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/VectorArray"));

    VectorArray va;
    va.dbl_vector_array[0].push_back(12.0);
    va.dbl_vector_array[2].push_back(-123456.789);
    va.dbl_vector_array[2].push_back(1.4);

    std::vector<double> dbl_vec;
    for (int j=0; j<3; j++)
        for (int i=0; i<va.dbl_vector_array[j].size(); i++)
            dbl_vec.push_back(va.dbl_vector_array[j][i]);

    {
        VectorArray va_test;
        va_test.dbl_vector_array[0].resize(1);
        va_test.dbl_vector_array[2].resize(2);
        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &va_test);
        BOOST_TEST_CHECKPOINT("BackConvert array of containers, presized");
        BOOST_CHECK( va_test.equals(va) );
    }
    {
        VectorArray va_test;
        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &va_test);
        BOOST_TEST_CHECKPOINT("BackConvert array of containers, stay empty");
        BOOST_CHECK( va_test.dbl_vector_array[0].empty());
        BOOST_CHECK( va_test.dbl_vector_array[1].empty());
        BOOST_CHECK( va_test.dbl_vector_array[2].empty());
    }
    {
        VectorArray va_test;
        va_test.dbl_vector_array[0].resize(1);
        va_test.dbl_vector_array[1].resize(1);
        va_test.dbl_vector_array[2].resize(1);
        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &va_test);
        BOOST_TEST_CHECKPOINT("BackConvert array of containers, different sizes");
        BOOST_CHECK( va_test.dbl_vector_array[0][0] == va.dbl_vector_array[0][0]);
        BOOST_CHECK( va_test.dbl_vector_array[1][0] == va.dbl_vector_array[2][0]);
        BOOST_CHECK( va_test.dbl_vector_array[2][0] == va.dbl_vector_array[2][1]);
    }
}


BOOST_AUTO_TEST_CASE( test_backconvert_container_container )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/ContainerContainer"));

    ContainerContainer cc;
    DoubleVector dv;
    dv.a = 10;
    dv.dbl_vector.push_back(12.2);
    cc.dbl_vv.push_back(dv);
    dv.a = -23;
    dv.dbl_vector.push_back(23.0);
    dv.dbl_vector.push_back(-142.2);
    cc.dbl_vv.push_back(dv);
    dv.a = 0;
    dv.dbl_vector.pop_back();
    cc.dbl_vv.push_back(dv);

    std::vector<double> dbl_vec;
    for (int i=0; i<cc.dbl_vv.size(); i++) {
        dbl_vec.push_back(cc.dbl_vv[i].a);
        for (int j=0; j<cc.dbl_vv[i].dbl_vector.size(); j++ )
            dbl_vec.push_back(cc.dbl_vv[i].dbl_vector[j]);
    }
    {
        ContainerContainer cc_test;
        cc_test.dbl_vv.resize(3);
        cc_test.dbl_vv[0].dbl_vector.resize(1);
        cc_test.dbl_vv[1].dbl_vector.resize(3);
        cc_test.dbl_vv[2].dbl_vector.resize(2);

        BackConverter bctv(toc, registry);
        bctv.apply(dbl_vec, &cc_test);
        BOOST_TEST_CHECKPOINT("BackConvert container of containers, presize");
        BOOST_CHECK( cc_test.equals(cc) );
    }
}

BOOST_AUTO_TEST_CASE( test_backconvert_two_array )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/TwoArrays"));

    TwoArrays a = {{1,2,3},{4,5,6,7,8}};
    std::vector<double> dbl_vec;
    for (int i=1; i<=8; i++) dbl_vec.push_back(i);

    TwoArrays b;
    FlatBackConverter fbctv(toc);
    fbctv.apply(dbl_vec, &b);
    BOOST_CHECK( a.equals(b) );

    TwoArrays c;
    BackConverter bctv(toc, registry);
    bctv.apply(dbl_vec, &c);
    BOOST_CHECK( a.equals(c) );
}

BOOST_AUTO_TEST_CASE( test_backconvert_array_with_slice ) {
        
    Registry registry;
    import_types(registry);

    {
        BOOST_TEST_CHECKPOINT("back convert double[3] with slice");
        registry.build("/double[3]");
        VectorToc toc = VectorTocMaker().apply(*registry.get("/double[3]")); 
        double d[] = {1.4, -123.2, 54.8};
     
        std::vector<double> dbl_vec;
        dbl_vec.push_back(-123.2);

        FlatBackConverter fbctv(toc);
        fbctv.setSlice("1");
        
        double d3[] = {1.4, 11.0, 54.8};
        fbctv.apply(dbl_vec, d3);
        for (int i = 0; i < 3; i++) BOOST_CHECK( d[i] == d3[i] );
    }
    
    {
        BOOST_TEST_CHECKPOINT("back convert double[20] with slice");
        registry.build("/double[20]");
        VectorToc toc = VectorTocMaker().apply(*registry.get("/double[20]")); 
        double d[20], val = -2.0;
        for (int i = 0; i < 20; i++, val += 0.6 )
            d[i] = val;
        
        std::vector<double> dbl_vec;
        dbl_vec.push_back(d[1]);
        dbl_vec.push_back(d[12]);
        dbl_vec.push_back(d[13]);
        dbl_vec.push_back(d[15]);
        dbl_vec.push_back(d[17]);
     
        FlatBackConverter fbctv(toc);
        fbctv.setSlice("[1,12,13-17:2]");
        
        double d20[20];
        for (int i = 0; i < 20; i++) d20[i] = d[i];
        d20[1] = -3.3;
        d20[12] = -3.3;
        d20[13] = -3.3;
        d20[15] = -3.3;
        d20[17] = -3.3;

        fbctv.apply(dbl_vec, d20);
        for (int i = 0; i < 20; i++) BOOST_CHECK( d[i] == d20[i] );
    }

}

BOOST_AUTO_TEST_CASE( test_backconvert_struct_with_slice )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/B"));
     
    struct B b = { '0', { 100, -23, 'c', 12 } };
    
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(b.a));
    dbl_vec.push_back(double(b.b.a));
    dbl_vec.push_back(double(b.b.c));

    struct B b2 = { '1', { -33, -23, '-', 12 }};
    FlatBackConverter fbctv(toc);
    fbctv.setSlice("a b.a b.c");
    fbctv.apply(dbl_vec, &b2);
    BOOST_CHECK(b.equals(b2));

    struct B b3 = { '0', { 100, 100, 'c', 99} };
    fbctv.setSlice("a b.b b.d");
    fbctv.apply(dbl_vec, &b2);
    BOOST_CHECK(b3.equals(b2));
}

BOOST_AUTO_TEST_CASE( test_backconvert_string_with_slice )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/std/string"));

    std::string str = "Hello world!";
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(str[0]));
    dbl_vec.push_back(double(str[6]));
    dbl_vec.push_back(double(str[7]));
    dbl_vec.push_back(double(str[8]));
    dbl_vec.push_back(double(str[9]));
    dbl_vec.push_back(double(str[10]));

    {
        std::string str2 = "_ello _____!";
        BackConverter bctv(toc, registry);
        bctv.setSlice("[0,6-10]");
        bctv.apply(dbl_vec, &str2);
        BOOST_TEST_CHECKPOINT( "preset string with slice");
        BOOST_CHECK(str == str2);
    }
    
    {
        std::string str2;
        str2.resize(12);
        BackConverter bctv(toc, registry);
        bctv.setSlice("[0,6-10]");
        bctv.apply(dbl_vec, &str2);
        BOOST_TEST_CHECKPOINT( "string with slice, size set");
        BOOST_CHECK(str[0] == str2[0]);
        BOOST_CHECK(str[6] == str2[6]);
        BOOST_CHECK(str[7] == str2[7]);
        BOOST_CHECK(str[8] == str2[8]);
        BOOST_CHECK(str[9] == str2[9]);
        BOOST_CHECK(str[10] == str2[10]);
    }
}

BOOST_AUTO_TEST_CASE( test_backconvert_array_in_struct_with_slice )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/StructArray"));
 
    StructArray sa;
    struct A a = { 10, -23, 51, 112 };
    struct A b = { -12452134, 12, 33, -1 };
    sa.A_vector.push_back(a);
    sa.A_vector.push_back(b);
    sa.A_vector.push_back(b);
    sa.A_vector.push_back(a);
    
    std::vector<double> dbl_vec;
    for (int i=0; i<sa.A_vector.size(); i++) {
        dbl_vec.push_back(sa.A_vector[i].a);
    }

    StructArray sb;
    sb.A_vector.resize(4);

    BackConverter bctv(toc, registry);
    bctv.setSlice("A_vector.*.a"); 
    bctv.apply(dbl_vec, &sb);
    for (int i = 0; i < 4; i++)
       BOOST_CHECK( sb.A_vector[i].a == sa.A_vector[i].a );
}

BOOST_AUTO_TEST_CASE( test_backconvert_container_container_with_slice )
{
    Registry registry;
    import_types(registry);
    VectorToc toc = VectorTocMaker().apply(*registry.get("/ContainerContainer"));
        
    ContainerContainer cc;
    DoubleVector dv;
    dv.a = 10;
    dv.dbl_vector.push_back(12.2);
    cc.dbl_vv.push_back(dv);
    dv.a = -23;
    dv.dbl_vector.push_back(23.0);
    dv.dbl_vector.push_back(-142.2);
    cc.dbl_vv.push_back(dv);
    dv.a = 0;
    dv.dbl_vector.pop_back();
    cc.dbl_vv.push_back(dv);
        
    std::vector<double> dbl_vec;
    dbl_vec.push_back(cc.dbl_vv[0].a);
    dbl_vec.push_back(cc.dbl_vv[1].a);
    dbl_vec.push_back(cc.dbl_vv[2].a);
    dbl_vec.push_back(cc.dbl_vv[2].dbl_vector[1]);
    
    ContainerContainer cc2;
    cc2.dbl_vv.resize(3);
    cc2.dbl_vv[0].dbl_vector.resize(1, -1.0);
    cc2.dbl_vv[2].dbl_vector.resize(2, -1.0);
    
    BackConverter bctv(toc, registry);    
    bctv.setSlice("dbl_vv.*.a dbl_vv.[0,2].dbl_vector.1");
    bctv.apply(dbl_vec, &cc2);
    
    BOOST_CHECK( cc2.dbl_vv[0].a == cc.dbl_vv[0].a );
    BOOST_CHECK( cc2.dbl_vv[1].a == cc.dbl_vv[1].a );
    BOOST_CHECK( cc2.dbl_vv[2].a == cc.dbl_vv[2].a );
    BOOST_CHECK( cc2.dbl_vv[2].dbl_vector[1] == cc.dbl_vv[2].dbl_vector[1] );
}
