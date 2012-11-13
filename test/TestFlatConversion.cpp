// \file  TestConversion.cpp

#include <boost/test/auto_unit_test.hpp>

#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#include <utilmm/stringtools.hh>

#include "TestSuite.hpp"

#include "Converter.hpp"
#include "VectorTocMaker.hpp"
#include "Utilities.hpp"

#include "TestTypes.h"

using namespace Typelib;
using namespace general_processing;

BOOST_AUTO_TEST_CASE( test_flat_convert_scalar )
{
    Registry registry;
    import_types(registry);
    
    {
        double f = 1.4;
        Value v(&f, *registry.get("/double"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/double")); 

        FlatConverter fc(toc);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(f);

        BOOST_CHECK( dbl_vec == fc.apply(v,true) );
        
        BOOST_TEST_CHECKPOINT("Testing double places");

        std::string places = "";

        std::vector<std::string> places_res = fc.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places );
    }

    {
        int i = 3;
        Value v(&i, *registry.get("/int"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/int")); 

        FlatConverter fc(toc);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(i);

        BOOST_CHECK( dbl_vec == fc.apply(v) );
    }
    
    {
        char c = 3;
        Value v(&c, *registry.get("/char"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/char")); 

        FlatConverter fc(toc);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(c);

        BOOST_CHECK( dbl_vec == fc.apply(v) );
    }

}

BOOST_AUTO_TEST_CASE( test_flat_convert_array )
{
    Registry registry;
    import_types(registry);

    registry.build("/double[3]");
    const Type& t = *registry.get("/double[3]");

    double d[] = { 1.4, -123.2, 54.8 };
 
    Value v(&d, t);

    VectorToc toc = VectorTocMaker().apply(t); 

    FlatConverter fc(toc);

    std::vector<double> dbl_vec;
    for (int i=0; i<3; i++) dbl_vec.push_back(d[i]);

    BOOST_CHECK( dbl_vec == fc.apply(v, true) );
        
    BOOST_TEST_CHECKPOINT("Testing double[3] places");

    std::string places = "0 1 2";

    std::vector<std::string> places_res = fc.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );

}

BOOST_AUTO_TEST_CASE( test_flat_convert_struct )
{
    Registry registry;
    import_types(registry);

    struct A a = { 100, -23, 'c', 12 };
    
    const Type& t = *registry.get("/A");

    Value v(&a, t);

    VectorToc toc = VectorTocMaker().apply(t);
    
    FlatConverter fc(toc);
    
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(a.a));
    dbl_vec.push_back(double(a.b));
    dbl_vec.push_back(double(a.c));
    dbl_vec.push_back(double(a.d));

    std::vector<double> res = fc.apply(v, true);

    BOOST_REQUIRE ( res.size() == dbl_vec.size() );

    BOOST_CHECK( dbl_vec == res );
    
    BOOST_TEST_CHECKPOINT("Testing struct A places");

    std::string places = "a b c d";

    std::vector<std::string> places_res = fc.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );
}

BOOST_AUTO_TEST_CASE( test_flat_convert_multi_struct )
{
    Registry registry;
    import_types(registry);

    struct B b = { '0', { 100, -23, 'c', 12 } };
    
    const Type& t = *registry.get("/B");

    Value v(&b, t);

    VectorToc toc = VectorTocMaker().apply(t); 
    
    FlatConverter fc(toc);
    
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(b.a));
    dbl_vec.push_back(double(b.b.a));
    dbl_vec.push_back(double(b.b.b));
    dbl_vec.push_back(double(b.b.c));
    dbl_vec.push_back(double(b.b.d));

    std::vector<double> res = fc.apply(v, true);

    BOOST_REQUIRE ( res.size() == dbl_vec.size() );

    BOOST_CHECK( dbl_vec == res );
    
    BOOST_TEST_CHECKPOINT("Testing struct B places");

    std::string places = "a b.a b.b b.c b.d";

    std::vector<std::string> places_res = fc.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );

}

BOOST_AUTO_TEST_CASE( test_flat_convert_container )
{
    Registry registry;
    import_types(registry);

    const Type& t = *registry.get("/std/vector</int>");
    
    VectorToc toc = VectorTocMaker().apply(t);

    FlatConverter fc(toc);
    
    std::vector<int> int_vec;
    int_vec.push_back(-10);
    int_vec.push_back(22);
    int_vec.push_back(3);

    Value v(&int_vec, t); 

    std::vector<double> res = fc.apply(v);

    BOOST_CHECK( res.empty() );

}

BOOST_AUTO_TEST_CASE( test_flat_convert_string )
{
    Registry registry;
    import_types(registry);

    const Type& t = *registry.get("/std/string");

    std::string str = "Hello world!";

    Value v(&str, t);

    VectorToc toc = VectorTocMaker().apply(t);

    FlatConverter fc(toc);
    
    std::vector<double> res = fc.apply(v);

    BOOST_CHECK( res.empty() ); 
}

BOOST_AUTO_TEST_CASE( test_flat_convert_advanced )
{
    Registry registry;
    import_types(registry);
    
    {
        const Type& t = *registry.get("/DoubleVector");
        
        DoubleVector dv;
        dv.a = 23;
        dv.dbl_vector.push_back(-123456.789);
        dv.dbl_vector.push_back(1.4);

        Value v(&dv, t);
        
        VectorToc toc = VectorTocMaker().apply(t);

        FlatConverter fc(toc);
        
        std::vector<double> res = fc.apply(v);

        BOOST_CHECK( res.size() == 1 );
        BOOST_CHECK( res[0] == dv.a );
    }

    BOOST_TEST_CHECKPOINT("Going to VectorArray");

    {
        const Type& t = *registry.get("/VectorArray");
        
        VectorArray va;
        va.dbl_vector_array[0].push_back(12.0);
        va.dbl_vector_array[2].push_back(-123456.789);
        va.dbl_vector_array[2].push_back(1.4);

        Value v(&va, t);
        
        VectorToc toc = VectorTocMaker().apply(t);

        FlatConverter fc(toc);
        
        std::vector<double> res = fc.apply(v);

        BOOST_CHECK( res.empty() ); 
    }
    
    BOOST_TEST_CHECKPOINT("Going to StructArray");

    {
        const Type& t = *registry.get("/StructArray");
        
        StructArray sa;
        struct A a = { 10, -23, 51, 112 };
        struct A b = { -12452134, 12, 33, -1 };
        sa.A_vector.push_back(a);
        sa.A_vector.push_back(b);

        Value v(&sa, t);
        
        VectorToc toc = VectorTocMaker().apply(t);
        
        FlatConverter fc(toc);
        
        std::vector<double> res = fc.apply(v);

        BOOST_CHECK( res.empty() ); 
    }
    
    BOOST_TEST_CHECKPOINT("Going to ContainerContainer");

    {
        const Type& t = *registry.get("/ContainerContainer");
        
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

        Value v(&cc, t);
        
        VectorToc toc = VectorTocMaker().apply(t);
        
        FlatConverter fc(toc);
        
        std::vector<double> res = fc.apply(v);

        BOOST_CHECK( res.empty() ); 
    }
    
    BOOST_TEST_CHECKPOINT("unsliced ForFlatSliceTest");

    {
        const Type& t = *registry.get("/ForFlatSliceTest");

        ForFlatSliceTest ffst;
        ffst.a = 12.1;
        for ( double d = 0.0; d < 12; d+= 1.37)
            ffst.vec.push_back(d);
        ffst.str = "this is a test";
        ffst.b = 33.0;
        
        Value v(&ffst, t);
        VectorToc toc = VectorTocMaker().apply(t);

        FlatConverter fc(toc);
        
        std::vector<double> res = fc.apply(v, true);

        BOOST_CHECK( res.size() == 2 );
        BOOST_CHECK( res[0] = ffst.a);
        BOOST_CHECK( res[1] = ffst.b);

    }
}

BOOST_AUTO_TEST_CASE( test_flat_convert_with_slice ) {
        
    Registry registry;
    import_types(registry);

    BOOST_TEST_CHECKPOINT("single index");
    
    {
        registry.build("/double[3]");
        const Type& t = *registry.get("/double[3]");

        double d[] = { 1.4, -123.2, 54.8 };
     
        Value v(&d, t);

        VectorToc toc = VectorTocMaker().apply(t); 

        FlatConverter fc(toc);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(d[1]);

        fc.setSlice("1");

        BOOST_CHECK( dbl_vec == fc.apply(v, true) );
            
        std::string places = "1";

        std::vector<std::string> places_res = fc.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places );
    }
    
    BOOST_TEST_CHECKPOINT("index slice");
    
    {
        registry.build("/double[20]");
        const Type& t = *registry.get("/double[20]");

        double d[20], val = -2.0;

        for ( int i=0; i<20; i++, val+=0.6 )
            d[i] = val;
     
        Value v(&d, t);

        VectorToc toc = VectorTocMaker().apply(t); 

        FlatConverter fc(toc);
        
        fc.setSlice("[1,12,13-17:2]");

        std::vector<double> dbl_vec;
        dbl_vec.push_back(d[1]);
        dbl_vec.push_back(d[12]);
        dbl_vec.push_back(d[13]);
        dbl_vec.push_back(d[15]);
        dbl_vec.push_back(d[17]);

        BOOST_CHECK( dbl_vec == fc.apply(v, true) );
            
        std::string places = "1 12 13 15 17";

        std::vector<std::string> places_res = fc.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places );
        
        Eigen::VectorXd x(dbl_vec.size());
        for (int i =0; i<dbl_vec.size(); i++)
            x[i] = dbl_vec[i];

        BOOST_CHECK ( x == fc.getEigenVector() );
    }

    BOOST_TEST_CHECKPOINT ( "sliced structure" );
    {
        struct B b = { '0', { 100, -23, 'c', 12 } };
        
        const Type& t = *registry.get("/B");

        Value v(&b, t);

        VectorToc toc = VectorTocMaker().apply(t);
        
        FlatConverter fc(toc);
        fc.setSlice("a b.a b.c");
        
        std::vector<double> dbl_vec;
        dbl_vec.push_back(double(b.a));
        dbl_vec.push_back(double(b.b.a));
        dbl_vec.push_back(double(b.b.c));

        std::vector<double> res = fc.apply(v, true);

        BOOST_REQUIRE ( res.size() == dbl_vec.size() );

        BOOST_CHECK( dbl_vec == res );
        
        BOOST_TEST_CHECKPOINT("Testing struct B places");

        std::string places = "a b.a b.c";

        std::vector<std::string> places_res = fc.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places );
    }

    BOOST_TEST_CHECKPOINT ( "sliced string" );
    {
        const Type& t = *registry.get("/std/string");
                        
                        // 012345678901
        std::string str = "Hello world!";

        Value v(&str, t);

        VectorToc toc = VectorTocMaker().apply(t);

        FlatConverter fc(toc);
        fc.setSlice("[0,6-10]");
        
        std::vector<double> res = fc.apply(v);

        BOOST_CHECK( res.empty() ); 
    }
    
    BOOST_TEST_CHECKPOINT("sliced StructArray");

    {
        const Type& t = *registry.get("/StructArray");
        
        StructArray sa;
        struct A a = { 10, -23, 51, 112 };
        struct A b = { -12452134, 12, 33, -1 };
        sa.A_vector.push_back(a);
        sa.A_vector.push_back(b);
        sa.A_vector.push_back(b);
        sa.A_vector.push_back(a);

        Value v(&sa, t);
        
        VectorToc toc = VectorTocMaker().apply(t);

        FlatConverter fc(toc);
        fc.setSlice("A_vector.*.a");
        
        std::vector<double> res = fc.apply(v,false);
        
        BOOST_CHECK( res.empty() ); 
    }
    
    BOOST_TEST_CHECKPOINT("sliced ContainerContainer");

    {
        const Type& t = *registry.get("/ContainerContainer");
        
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

        Value v(&cc, t);
        
        VectorToc toc = VectorTocMaker().apply(t);

        FlatConverter fc(toc);
        fc.setSlice("dbl_vv.*.a dbl_vv.[0,2].dbl_vector.1");
        
        std::vector<double> res = fc.apply(v, true);

        BOOST_CHECK( res.empty() ); 
    }

    BOOST_TEST_CHECKPOINT("ForFlatSliceTest");

    {
        const Type& t = *registry.get("/ForFlatSliceTest");

        ForFlatSliceTest ffst;
        ffst.a = 12.1;
        for ( double d = 0.0; d < 12; d+= 1.37)
            ffst.vec.push_back(d);
        ffst.str = "this is a test";
        ffst.b = 33.0;
        
        Value v(&ffst, t);
        VectorToc toc = VectorTocMaker().apply(t);

        FlatConverter fc(toc);
        fc.setSlice("str b");
        
        std::vector<double> res = fc.apply(v, true);

        BOOST_CHECK( res.size() == 1 );
        BOOST_CHECK( res[0] = ffst.b);

    }

}

BOOST_AUTO_TEST_CASE( test_single_convert_multi_struct )
{
    Registry registry;
    import_types(registry);

    struct B b = { '0', { 100, -23, 'c', 12 } };
    
    const Type& t = *registry.get("/B");

    Value v(&b, t);

    VectorToc toc = VectorTocMaker().apply(t);
    
    SingleConverter sc(toc);
    
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(b.a));

    std::vector<double> res = sc.apply(v, true);

    BOOST_REQUIRE ( res.size() == dbl_vec.size() );

    BOOST_CHECK( dbl_vec == res );
    
    BOOST_TEST_CHECKPOINT("Testing struct B places");

    std::string places = "a";

    std::vector<std::string> places_res = sc.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );
}

BOOST_AUTO_TEST_CASE( test_single_convert_container )
{
    Registry registry;
    import_types(registry);

    const Type& t = *registry.get("/std/vector</int>");
    
    VectorToc toc = VectorTocMaker().apply(t);

    SingleConverter sc(toc);
    
    std::vector<int> int_vec;
    int_vec.push_back(-10);
    int_vec.push_back(22);
    int_vec.push_back(3);

    Value v(&int_vec, t); 

    std::vector<double> res = sc.apply(v);

    BOOST_CHECK( res.empty() );
}


