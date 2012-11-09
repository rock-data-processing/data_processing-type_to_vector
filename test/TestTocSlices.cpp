// \file  TestTocSlices.cpp

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

BOOST_AUTO_TEST_CASE( test_vectortocslicer ) {

    Registry registry;
    import_types(registry);

    BOOST_TEST_CHECKPOINT("slice A");
    {
        const Type& t = *registry.get("/A");

        VectorToc toc = VectorTocMaker().apply(t);

        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == "a b c d" );

        VectorToc toc2 = VectorTocSlicer::slice(toc,"");
        
        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2) == "a b c d" );

        toc2 = VectorTocSlicer::slice(toc,"a c");
        
        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2b(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2b) == "a c" );
        
        VectorToc toc3 = VectorTocSlicer::slice(toc2,"b");
        
        ptoc = PlainTocVisitor().apply(toc3);
        utilmm::stringlist sl3(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl3) == "" );

        toc3 = VectorTocSlicer::slice(toc2,"c");

        ptoc = PlainTocVisitor().apply(toc3);
        utilmm::stringlist sl3b(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl3b) == "c" );
    }
    
    BOOST_TEST_CHECKPOINT("slice B");
    {
        const Type& t = *registry.get("/B");

        VectorToc toc = VectorTocMaker().apply(t);

        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == "a b.a b.b b.c b.d" );
        
        VectorToc toc2 = VectorTocSlicer::slice(toc,"b");
        
        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2) == "b.a b.b b.c b.d" );
        
        VectorToc toc3 = VectorTocSlicer::slice(toc2,"b.b b.c");
        
        ptoc = PlainTocVisitor().apply(toc3);
        utilmm::stringlist sl3(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl3) == "b.b b.c" ); 
    }

    BOOST_TEST_CHECKPOINT("slice StructArray");
    {
        const Type& t = *registry.get("/StructArray");
        
        VectorToc toc = VectorTocMaker().apply(t);
        
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == 
                "A_vector.*.a A_vector.*.b A_vector.*.c A_vector.*.d" );

        VectorToc toc2 = VectorTocSlicer::slice(toc,"A_vector.*");
        
        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2) == 
                "A_vector.*.a A_vector.*.b A_vector.*.c A_vector.*.d" );
        
        VectorToc toc3 = VectorTocSlicer::slice(toc,"A_vector.*.d");
        
        ptoc = PlainTocVisitor().apply(toc3);
        utilmm::stringlist sl3(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl3) == "A_vector.*.d" ); 
    }
    
    BOOST_TEST_CHECKPOINT("slice ContainerContainer");
    {
        const Type& t = *registry.get("/ContainerContainer");
        
        VectorToc toc = VectorTocMaker().apply(t);
        
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == "dbl_vv.*.a dbl_vv.*.dbl_vector.*" );
        
        VectorToc toc2 = VectorTocSlicer::slice(toc,"dbl_vv.*.dbl_vector.*");
        
        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2) == "dbl_vv.*.dbl_vector.*");
    }
   
}

BOOST_AUTO_TEST_CASE( test_slice_convertion ) {

    Registry registry;
    import_types(registry);
    
    BOOST_TEST_CHECKPOINT("convert slice of B");
    {
        struct B b = { '0', { 100, -23, 'c', 12 } };
    
        const Type& t = *registry.get("/B");

        Value v(&b, t);

        VectorToc toc = VectorTocMaker().apply(t);
        VectorToc sliced_toc = VectorTocSlicer::slice(toc,"a b.b");
    
        ConvertToVector ctv(sliced_toc,registry);

        std::vector<double> dbl_vec;
        dbl_vec.push_back(double(b.a));
        dbl_vec.push_back(double(b.b.b));

        std::vector<double> res = ctv.apply(v, true);

        BOOST_REQUIRE ( res.size() == dbl_vec.size() );

        BOOST_CHECK( dbl_vec == res );
    
        std::string places = "a b.b";

        std::vector<std::string> places_res = ctv.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places );
    }
    
    BOOST_TEST_CHECKPOINT("convert slice of StructArray");
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
        VectorToc sliced_toc = VectorTocSlicer::slice(toc,"A_vector.*.a A_vector.*.d");

        ConvertToVector ctv(sliced_toc,registry);
        
        std::vector<double> dbl_vec;
        for (int i=0; i<sa.A_vector.size(); i++) {
            dbl_vec.push_back(sa.A_vector[i].a);
            dbl_vec.push_back(sa.A_vector[i].d);
        }
    
        std::vector<double> res = ctv.apply(v);

        BOOST_REQUIRE( res.size() == dbl_vec.size() );

        BOOST_CHECK( res == dbl_vec );
    }
    
    BOOST_TEST_CHECKPOINT("convert slice of ContainerContainer");
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
        VectorToc sliced_toc = VectorTocSlicer::slice(toc,"dbl_vv.*.a");

        ConvertToVector ctv(sliced_toc,registry);
        
        std::vector<double> dbl_vec;
        for (int i=0; i<cc.dbl_vv.size(); i++) {
            dbl_vec.push_back(cc.dbl_vv[i].a);
        }

        std::vector<double> res = ctv.apply(v, true);

        BOOST_REQUIRE( res.size() == dbl_vec.size() );

        BOOST_CHECK( res == dbl_vec );

        std::string places = 
            "dbl_vv.0.a "
            "dbl_vv.1.a "
            "dbl_vv.2.a";

        std::vector<std::string> places_res = ctv.getPlaceVector();
        utilmm::stringlist places_list(places_res.begin(), places_res.end());

        BOOST_CHECK( utilmm::join(places_list) == places ); 
    }
}
