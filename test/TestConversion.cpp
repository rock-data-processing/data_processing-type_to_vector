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

BOOST_AUTO_TEST_CASE( test_convert_scalar )
{
    Registry registry;
    import_types(registry);
    
    {
        double f = 1.4;
        Value v(&f, *registry.get("/double"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/double")); 

        ConvertToVector ctv(toc, registry);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(f);

        BOOST_CHECK( dbl_vec == ctv.apply(v) );
        
        BOOST_TEST_CHECKPOINT("Testing double places");

        std::string places = "";

        std::vector<std::string> places_res = ctv.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places );
    }

    {
        int i = 3;
        Value v(&i, *registry.get("/int"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/int")); 

        ConvertToVector ctv(toc, registry);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(i);

        BOOST_CHECK( dbl_vec == ctv.apply(v) );
    }
    
    {
        char c = 3;
        Value v(&c, *registry.get("/char"));

        VectorToc toc = VectorTocMaker().apply(*registry.get("/char")); 

        ConvertToVector ctv(toc, registry);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(c);

        BOOST_CHECK( dbl_vec == ctv.apply(v) );
    }

}

BOOST_AUTO_TEST_CASE( test_convert_array )
{
    Registry registry;
    import_types(registry);

    registry.build("/double[3]");
    const Type& t = *registry.get("/double[3]");

    double d[] = { 1.4, -123.2, 54.8 };
 
    Value v(&d, t);

    VectorToc toc = VectorTocMaker().apply(t); 

    ConvertToVector ctv(toc, registry);

    std::vector<double> dbl_vec;
    for (int i=0; i<3; i++) dbl_vec.push_back(d[i]);

    BOOST_CHECK( dbl_vec == ctv.apply(v) );
        
    BOOST_TEST_CHECKPOINT("Testing double[3] places");

    std::string places = "0 1 2";

    std::vector<std::string> places_res = ctv.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );

}

BOOST_AUTO_TEST_CASE( test_convert_struct )
{
    Registry registry;
    import_types(registry);

    struct A a = { 100, -23, 'c', 12 };
    
    const Type& t = *registry.get("/A");

    Value v(&a, t);

    VectorToc toc = VectorTocMaker().apply(t);
    
    ConvertToVector ctv(toc, registry);
    
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(a.a));
    dbl_vec.push_back(double(a.b));
    dbl_vec.push_back(double(a.c));
    dbl_vec.push_back(double(a.d));

    std::vector<double> res = ctv.apply(v);

    BOOST_REQUIRE ( res.size() == dbl_vec.size() );

    BOOST_CHECK( dbl_vec == res );
    
    BOOST_TEST_CHECKPOINT("Testing struct A places");

    std::string places = "a b c d";

    std::vector<std::string> places_res = ctv.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );
}

BOOST_AUTO_TEST_CASE( test_convert_multi_struct )
{
    Registry registry;
    import_types(registry);

    struct B b = { '0', { 100, -23, 'c', 12 } };
    
    const Type& t = *registry.get("/B");

    Value v(&b, t);

    VectorToc toc = VectorTocMaker().apply(t);
    
    ConvertToVector ctv(toc,registry);
    
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(b.a));
    dbl_vec.push_back(double(b.b.a));
    dbl_vec.push_back(double(b.b.b));
    dbl_vec.push_back(double(b.b.c));
    dbl_vec.push_back(double(b.b.d));

    std::vector<double> res = ctv.apply(v);

    BOOST_REQUIRE ( res.size() == dbl_vec.size() );

    BOOST_CHECK( dbl_vec == res );
    
    BOOST_TEST_CHECKPOINT("Testing struct B places");

    std::string places = "a b.a b.b b.c b.d";

    std::vector<std::string> places_res = ctv.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );

}

BOOST_AUTO_TEST_CASE( test_convert_container )
{
    Registry registry;
    import_types(registry);

    const Type& t = *registry.get("/std/vector</int>");
    
    VectorToc toc = VectorTocMaker().apply(t);

    ConvertToVector ctv(toc,registry);

    std::vector<int> int_vec;

    {
        Value v(&int_vec, t);
    
        std::vector<double> res = ctv.apply(v);

        BOOST_CHECK( res.empty() );
    }

    int_vec.push_back(-10);
    int_vec.push_back(22);
    int_vec.push_back(3);

    Value v(&int_vec, t);
    
    std::vector<double> dbl_vec(int_vec.begin(), int_vec.end());

    std::vector<double> res = ctv.apply(v);

    BOOST_REQUIRE( res.size() == dbl_vec.size() );

    BOOST_CHECK( res == dbl_vec );
    
}

BOOST_AUTO_TEST_CASE( test_convert_string )
{
    Registry registry;
    import_types(registry);

    const Type& t = *registry.get("/std/string");

    std::string str = "Hello world!";

    Value v(&str, t);

    VectorToc toc = VectorTocMaker().apply(t);

    ConvertToVector ctv(toc,registry);
    
    std::vector<double> dbl_vec(str.begin(), str.end());

    std::vector<double> res = ctv.apply(v);

    BOOST_REQUIRE( res.size() == dbl_vec.size() );

    BOOST_CHECK( res == dbl_vec );
    
}

BOOST_AUTO_TEST_CASE( test_convert_advanced )
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

        ConvertToVector ctv(toc,registry);
        
        std::vector<double> dbl_vec;
        dbl_vec.push_back(double(dv.a));
        for (int i=0; i<dv.dbl_vector.size(); i++)
            dbl_vec.push_back(double(dv.dbl_vector[i]));
    
        std::vector<double> res = ctv.apply(v);

        BOOST_REQUIRE( res.size() == dbl_vec.size() );

        BOOST_CHECK( res == dbl_vec );
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

        ConvertToVector ctv(toc,registry);
        
        std::vector<double> dbl_vec;
        for (int j=0; j<3; j++)
            for (int i=0; i<va.dbl_vector_array[j].size(); i++)
                dbl_vec.push_back(va.dbl_vector_array[j][i]);
    
        std::vector<double> res = ctv.apply(v);

        BOOST_REQUIRE( res.size() == dbl_vec.size() );

        BOOST_CHECK( res == dbl_vec );
        
        BOOST_TEST_CHECKPOINT("Testing VectorArray places");

        std::string places = 
            "dbl_vector_array.0.0 "
            "dbl_vector_array.2.0 "
            "dbl_vector_array.2.1";

        std::vector<std::string> places_res = ctv.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end()); 
        
        BOOST_CHECK( utilmm::join(places_list) == places );
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

        ConvertToVector ctv(toc,registry);
        
        std::vector<double> dbl_vec;
        for (int i=0; i<sa.A_vector.size(); i++) {
            dbl_vec.push_back(sa.A_vector[i].a);
            dbl_vec.push_back(sa.A_vector[i].b);
            dbl_vec.push_back(sa.A_vector[i].c);
            dbl_vec.push_back(sa.A_vector[i].d);
        }
    
        std::vector<double> res = ctv.apply(v);

        BOOST_REQUIRE( res.size() == dbl_vec.size() );

        BOOST_CHECK( res == dbl_vec );
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

        ConvertToVector ctv(toc,registry);
        
        std::vector<double> dbl_vec;
        for (int i=0; i<cc.dbl_vv.size(); i++) {
            dbl_vec.push_back(cc.dbl_vv[i].a);
            for (int j=0; j<cc.dbl_vv[i].dbl_vector.size(); j++ )
                dbl_vec.push_back(cc.dbl_vv[i].dbl_vector[j]);
        }

        std::vector<double> res = ctv.apply(v);

        BOOST_REQUIRE( res.size() == dbl_vec.size() );

        BOOST_CHECK( res == dbl_vec );

        BOOST_TEST_CHECKPOINT("Testing ContainerContainer places");

        std::string places = 
            "dbl_vv.0.a "
            "dbl_vv.0.dbl_vector.0 "
            "dbl_vv.1.a "
            "dbl_vv.1.dbl_vector.0 "
            "dbl_vv.1.dbl_vector.1 "
            "dbl_vv.1.dbl_vector.2 "
            "dbl_vv.2.a "
            "dbl_vv.2.dbl_vector.0 "
            "dbl_vv.2.dbl_vector.1";

        std::vector<std::string> places_res = ctv.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places );
        
        ctv.apply(v, false);

        BOOST_CHECK( ctv.getPlaceVector().empty() );


        BOOST_TEST_CHECKPOINT("Testing Eigen::VectorXd output");

        Eigen::VectorXd x(dbl_vec.size());
        for (int i =0; i<dbl_vec.size(); i++)
            x[i] = dbl_vec[i];

        BOOST_CHECK ( x == ctv.getEigenVector() );

    }
}
