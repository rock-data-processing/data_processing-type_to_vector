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
    
    BOOST_TEST_CHECKPOINT("inverse slice B");
    {
        const Type& t = *registry.get("/B");

        VectorToc toc = VectorTocMaker().apply(t);
        
        BOOST_CHECK( toc.isFlat() );

        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == "a b.a b.b b.c b.d" );
        
        VectorToc toc2 = VectorTocSlicer::slice(toc,"!b.b b.c");
        
        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2) == "a b.a b.d" );
        
        VectorToc toc3 = VectorTocSlicer::slice(toc,"! a b.d");
        
        ptoc = PlainTocVisitor().apply(toc3);
        utilmm::stringlist sl3(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl3) == "b.a b.b b.c" ); 
    }

    BOOST_TEST_CHECKPOINT("slice StructArray");
    {
        const Type& t = *registry.get("/StructArray");
        
        VectorToc toc = VectorTocMaker().apply(t);
        
        BOOST_CHECK( !toc.isFlat() );
        
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == 
                "A_vector.*.a A_vector.*.b A_vector.*.c A_vector.*.d" );

        VectorToc toc2 = VectorTocSlicer::slice(toc,"A_vector.*");
        
        BOOST_CHECK( !toc2.isFlat() );

        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2) == 
                "A_vector.*.a A_vector.*.b A_vector.*.c A_vector.*.d" );
        
        VectorToc toc3 = VectorTocSlicer::slice(toc,"A_vector.*.d");
        
        ptoc = PlainTocVisitor().apply(toc3);
        utilmm::stringlist sl3(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl3) == "A_vector.*.d" );      
        
        VectorToc toc4 = VectorTocSlicer::slice(toc,"A_vector.[1-3].d");
        
        ptoc = PlainTocVisitor().apply(toc4);
        utilmm::stringlist sl4(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl4) == "A_vector.*.d" );      
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

    BOOST_TEST_CHECKPOINT("slice ForFlatSliceTest");
    {
        const Type& t = *registry.get("/ForFlatSliceTest");
        
        VectorToc toc = VectorTocMaker().apply(t);

        BOOST_CHECK( !toc.isFlat() );
        
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == "a vec.* str.* b" );
        
        VectorToc toc2 = VectorTocSlicer::slice(toc,"vec b");
        
        BOOST_CHECK( !toc2.isFlat() );
        
        ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl2(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl2) == "vec.* b");
        
        VectorToc toc3 = VectorTocSlicer::slice(toc,"a b");
        
        BOOST_CHECK( toc3.isFlat() );
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

        std::vector<double> res = ctv.applyToValue(v, true);

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
    
        std::vector<double> res = ctv.applyToValue(v);

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

        std::vector<double> res = ctv.applyToValue(v, true);

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
    
    BOOST_TEST_CHECKPOINT("conversion with sliced ForFlatSliceTest");
    {
        const Type& t = *registry.get("/ForFlatSliceTest");
        
        VectorToc toc = VectorTocMaker().apply(t);
        VectorToc toc2 = VectorTocSlicer::slice(toc,"b");

        ForFlatSliceTest ffst;
        ffst.a = 12.1;
        for ( double d = 0.0; d < 12; d+= 1.37)
            ffst.vec.push_back(d);
        ffst.str = "this is a test";
        ffst.b = 33.0;
        
        Value v(&ffst, t);

        FlatConverter fc(toc2);
        
        std::vector<double> res = fc.applyToValue(v);

        BOOST_CHECK( res.size() == 1 );
        BOOST_CHECK( res[0] = ffst.b);
    }
}

BOOST_AUTO_TEST_CASE( test_single_convert_multi_struct_slicing )
{
    Registry registry;
    import_types(registry);

    struct B b = { '0', { 100, -23, 'c', 12 } };
    
    const Type& t = *registry.get("/B");

    Value v(&b, t);

    VectorToc toc = VectorTocMaker().apply(t);
    VectorToc toc2 = VectorTocSlicer::slice(toc,"b.b b.c");
    
    SingleConverter sc(toc2);
    
    std::vector<double> dbl_vec;
    dbl_vec.push_back(double(b.b.b));

    std::vector<double> res = sc.applyToValue(v, true);

    BOOST_REQUIRE ( res.size() == dbl_vec.size() );

    BOOST_CHECK( dbl_vec == res );
    
    BOOST_TEST_CHECKPOINT("Testing struct B places");

    std::string places = "b.b";

    std::vector<std::string> places_res = sc.getPlaceVector();
    utilmm::stringlist places_list(places_res.begin(), places_res.end());

    BOOST_CHECK( utilmm::join(places_list) == places );
}

BOOST_AUTO_TEST_CASE( test_doc_slices )
{
    Registry registry;
    import_types(registry);

    const Type& t = *registry.get("/DocB");

    VectorToc toc = VectorTocMaker().apply(t);
        
    BOOST_CHECK ( toc == toc );

    VectorToc toc_ = toc;

    BOOST_CHECK ( toc == toc_ );

    std::string slices = "data|data.[0,2].b|idx data.*.b|data.*.c|! idx data.*.b|!";
    utilmm::stringlist sll = utilmm::split(slices,"|");
    StringVector sl(sll.begin(),sll.end());

    {
        VectorToc toc2 = VectorTocSlicer::slice(toc,"");

        BOOST_CHECK ( toc == toc2 );
    }

    {
        VectorToc toc2 = VectorTocSlicer::slice(toc,sl[1]);
        
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc2);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == "data.0.b data.2.b" );
    
        std::cout << utilmm::join(sl) << std::endl;
    }


}
