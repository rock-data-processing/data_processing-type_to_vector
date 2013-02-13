// \file  TestVectorBuilder.cpp

#include <boost/test/auto_unit_test.hpp>

#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#include <utilmm/stringtools.hh>

#include "TestSuite.hpp"

#include "Converter.hpp"
#include "VectorTocMaker.hpp"
#include "Utilities.hpp"
#include "VectorBuilder.hpp"

#include "TestTypes.h"

using namespace Typelib;
using namespace type_to_vector;

namespace type_to_vector {
    bool operator== (const VectorPosition& one, const VectorPosition& another) {
        return one.start == another.start && one.end == another.end;
    }
    std::ostream& operator<< ( std::ostream& os, const VectorPosition& vpos) {
        os << "from " << vpos.start << " to " << vpos.end;
        return os;
    }
}

BOOST_AUTO_TEST_CASE( test_builder ) {

    Registry registry;
    import_types(registry);

    DataVectorBuilder builder;
    
    {
        builder.push_back(VectorConversion("int"));
        VectorConversion& vcb = builder.back();

        VectorToc toc = VectorTocMaker().apply(*registry.get("/int"));

        vcb.addConverter(AbstractConverter::Pointer(new FlatConverter(toc)));
        vcb.addConverter(AbstractConverter::Pointer(new SingleConverter(toc))); 
    }
    int IntIndex = builder.size()-1;

    BOOST_TEST_CHECKPOINT( "added int conversion" );

    {
        builder.push_back(VectorConversion("B"));
        VectorConversion& vcb = builder.back();

        VectorToc toc = VectorTocMaker().apply(*registry.get("/B"));
        VectorToc toc2 = VectorTocSlicer::slice(toc, "b");

        vcb.addConverter(AbstractConverter::Pointer(new FlatConverter(toc2)));

        toc.resize(1);
        vcb.addConverter(AbstractConverter::Pointer(new SingleConverter(toc))); 
    }
    int BIndex = builder.size()-1;
    
    BOOST_TEST_CHECKPOINT( "added B conversion" );

    {
        builder.push_back(VectorConversion("Container"));
        VectorConversion& vcb = builder.back();

        VectorToc toc = VectorTocMaker().apply(*registry.get("/DoubleVector"));
        VectorToc toc2 = VectorTocSlicer::slice(toc, "!a");
        VectorToc toc3 = VectorTocSlicer::slice(toc, "a");
        
        vcb.addConverter(AbstractConverter::Pointer(new ConvertToVector(toc2, registry)));
        vcb.addConverter(AbstractConverter::Pointer(new SingleConverter(toc3))); 
    }
    int ContainerIndex = builder.size()-1;
    
    BOOST_TEST_CHECKPOINT( "added Container conversion" );

    {
        int i = 2;
        B b = { 56 , { 111, -12, 80, 23} };
        DoubleVector dv;
        dv.a = 44;
        dv.dbl_vector.push_back(1);
        dv.dbl_vector.push_back(2);
        
        builder.update(IntIndex, &i, true);
        builder.update(BIndex, &b, true);
        builder.update(ContainerIndex, &dv,  true);
        
        double ref1[] = { 2, 111, -12, 80, 23, 1, 2 };
        double ref2[] = { 2, 56, 44 };

        BOOST_CHECK( builder.getVector(0) == VectorOfDoubles(ref1, ref1+7) );
        BOOST_CHECK( builder.getVector(1) == VectorOfDoubles(ref2, ref2+3) );

        std::string sref1[] = { "int", "B.b.a", "B.b.b", "B.b.c", "B.b.d",
            "Container.dbl_vector.0", "Container.dbl_vector.1" };
        std::string sref2[] = { "int", "B.a", "Container.a" };

        BOOST_CHECK( builder.getPlaces(0) == StringVector(sref1, sref1+7) );
        BOOST_CHECK( builder.getPlaces(1) == StringVector(sref2, sref2+3) );

        BOOST_CHECK( builder.getVectorPosition(0,IntIndex) == VectorPosition(0,0) );
        BOOST_CHECK( builder.getVectorPosition(0,BIndex) == VectorPosition(1,4) );
        BOOST_CHECK( builder.getVectorPosition(0,ContainerIndex) == VectorPosition(5,6) );
        BOOST_CHECK( builder.getVectorPosition(1,IntIndex) == VectorPosition(0,0) );
        BOOST_CHECK( builder.getVectorPosition(1,BIndex) == VectorPosition(1,1) );
        BOOST_CHECK( builder.getVectorPosition(1,ContainerIndex) == VectorPosition(2,2) );
        
        BOOST_CHECK( builder.getVectorIdx("int") == IntIndex );    
        BOOST_CHECK( builder.getVectorIdx("B") == BIndex );    
        BOOST_CHECK( builder.getVectorIdx("Container") == ContainerIndex );    
        BOOST_CHECK( builder.getVectorIdx("C") == -1 );
    }
    
    {
        int i = -55;
        B b = { 22 , { 11, 2, 81, 12} };
        DoubleVector dv;
        dv.a = 45;
        
        builder.update(IntIndex, &i, true);
        builder.update(BIndex, &b, true);
        builder.update(ContainerIndex, &dv,  true);
        
        double ref1[] = { -55, 11, 2, 81, 12 };
        double ref2[] = { -55, 22, 45 };

        BOOST_CHECK( builder.getVector(0) == VectorOfDoubles(ref1, ref1+5) );
        BOOST_CHECK( builder.getVector(1) == VectorOfDoubles(ref2, ref2+3) );

        std::string sref1[] = { "int", "B.b.a", "B.b.b", "B.b.c", "B.b.d" };
        std::string sref2[] = { "int", "B.a", "Container.a" };
        
        BOOST_CHECK( builder.getPlaces(0) == StringVector(sref1, sref1+5) );
        BOOST_CHECK( builder.getPlaces(1) == StringVector(sref2, sref2+3) );

        BOOST_CHECK( builder.getVectorPosition(0,IntIndex) == VectorPosition(0,0) );
        BOOST_CHECK( builder.getVectorPosition(0,BIndex) == VectorPosition(1,4) );
        BOOST_CHECK( builder.getVectorPosition(0,ContainerIndex) == VectorPosition(5,4) );
        BOOST_CHECK( builder.getVectorPosition(1,IntIndex) == VectorPosition(0,0) );
        BOOST_CHECK( builder.getVectorPosition(1,BIndex) == VectorPosition(1,1) );
        BOOST_CHECK( builder.getVectorPosition(1,ContainerIndex) == VectorPosition(2,2) );
    }
    
    {
        int i = -11;
        B b = { 123, { 13, 21, -81, 112} };
        
        builder.update(1, IntIndex, &i, true);
        builder.update(1, BIndex, &b, true);
        
        double ref1[] = { -55, 11, 2, 81, 12 };
        double ref2[] = { -11, 123, 45 };

        BOOST_CHECK( builder.getVector(0) == VectorOfDoubles(ref1, ref1+5) );
        BOOST_CHECK( builder.getVector(1) == VectorOfDoubles(ref2, ref2+3) );

        std::string sref1[] = { "int", "B.b.a", "B.b.b", "B.b.c", "B.b.d" };
        std::string sref2[] = { "int", "B.a", "Container.a" };
        
        BOOST_CHECK( builder.getPlaces(0) == StringVector(sref1, sref1+5) );
        BOOST_CHECK( builder.getPlaces(1) == StringVector(sref2, sref2+3) );

        BOOST_CHECK( builder.getVectorPosition(0,IntIndex) == VectorPosition(0,0) );
        BOOST_CHECK( builder.getVectorPosition(0,BIndex) == VectorPosition(1,4) );
        BOOST_CHECK( builder.getVectorPosition(0,ContainerIndex) == VectorPosition(5,4) );
        BOOST_CHECK( builder.getVectorPosition(1,IntIndex) == VectorPosition(0,0) );
        BOOST_CHECK( builder.getVectorPosition(1,BIndex) == VectorPosition(1,1) );
        BOOST_CHECK( builder.getVectorPosition(1,ContainerIndex) == VectorPosition(2,2) );
    }
}

