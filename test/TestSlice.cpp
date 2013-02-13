// \file  TestSlice.cpp

#include <boost/test/auto_unit_test.hpp>

#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#include <utilmm/stringtools.hh>

#include "TestSuite.hpp"

#include "SliceMatcher.hpp"

using namespace type_to_vector;

BOOST_AUTO_TEST_CASE ( test_slice_methods ) {

    BOOST_TEST_CHECKPOINT ("getIndices");

    SliceStore::IndexSlice is = SliceStore::getIndices("23");
    
    BOOST_CHECK ( is.from == 23 );
    BOOST_CHECK ( is.to == 23 );
    BOOST_CHECK ( is.every == 1 );

    is = SliceStore::getIndices("12-33");

    BOOST_CHECK ( is.from == 12 );
    BOOST_CHECK ( is.to == 33 );
    BOOST_CHECK ( is.every == 1 );

    is = SliceStore::getIndices("22-101:4");
    
    BOOST_CHECK ( is.from == 22 );
    BOOST_CHECK ( is.to == 101 );
    BOOST_CHECK ( is.every == 4 );
    
    is = SliceStore::getIndices("");
    
    BOOST_CHECK ( is.from == -1 );
    
    BOOST_CHECK_THROW (SliceStore::getIndices("a"),boost::bad_lexical_cast); 

    BOOST_TEST_CHECKPOINT ("resolveIndices");
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("position",false);

        BOOST_CHECK(res.empty());
    }
    
    BOOST_CHECK_THROW (SliceStore::resolveIndices("[]",false),std::runtime_error);
    BOOST_CHECK_THROW (SliceStore::resolveIndices("[,,]",false),std::runtime_error);
         
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[,4,,6,]",false);

        std::vector<std::string> ref;
        ref.push_back("4");
        ref.push_back("6");

        BOOST_CHECK(res == ref);
    }

    {
        std::vector<std::string> res = SliceStore::resolveIndices("[1,4,6]",false);

        std::vector<std::string> ref;
        ref.push_back("1");
        ref.push_back("4");
        ref.push_back("6");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[1]",false);

        std::vector<std::string> ref;
        ref.push_back("1");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[3-6]",false);

        std::vector<std::string> ref;
        ref.push_back("3");
        ref.push_back("4");
        ref.push_back("5");
        ref.push_back("6");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[3-10:3]", false);

        std::vector<std::string> ref;
        ref.push_back("3");
        ref.push_back("6");
        ref.push_back("9");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[1,3-10:3,11-13]", false);

        std::vector<std::string> ref;
        ref.push_back("1");
        ref.push_back("3");
        ref.push_back("6");
        ref.push_back("9");
        ref.push_back("11");
        ref.push_back("12");
        ref.push_back("13");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[1,3-10:3,11-13]", true);

        std::vector<std::string> ref;
        ref.push_back("*");

        BOOST_CHECK(res == ref);
    }

    BOOST_TEST_CHECKPOINT( "makeDirectIndexSlice" );

    { 
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.2.b", false);
        
        std::vector<std::string> ref;
        ref.push_back("a.2.b");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.[2].b", false);
        
        std::vector<std::string> ref;
        ref.push_back("a.2.b");

        BOOST_CHECK(res == ref); 
    }

    { 
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.[1,3-6:2].b",false);
        
        std::vector<std::string> ref;
        ref.push_back("a.1.b");
        ref.push_back("a.3.b");
        ref.push_back("a.5.b");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.[1,3-6:2].b.[10,11-13]",false);
        
        std::vector<std::string> ref;
        ref.push_back("a.1.b.[10,11-13]");
        ref.push_back("a.3.b.[10,11-13]");
        ref.push_back("a.5.b.[10,11-13]");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.[1,3-6:2].b.[10,11-13]",true);
        
        std::vector<std::string> ref;
        ref.push_back("a.*.b.[10,11-13]");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.*.b.*.[2,3].c.*.1",false);
        
        std::vector<std::string> ref;
        ref.push_back("a.*.b.*.2.c.*.1");
        ref.push_back("a.*.b.*.3.c.*.1");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.*.b.*.[2,3].c.*.1",true);
        
        std::vector<std::string> ref;
        ref.push_back("a.*.b.*.*.c.*.1");

        BOOST_CHECK(res == ref); 
    }

    BOOST_TEST_CHECKPOINT( "replaceIndicesSlices" );
     
    { 
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.[1].b",false);
        
        utilmm::stringlist ref;
        ref.push_back("a.1.b");

        BOOST_CHECK(res == ref); 
    }
    
    BOOST_CHECK_THROW (SliceStore::replaceIndicesSlices("a.[].b",false),
            std::runtime_error);
    
    { 
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.[1,2].b",false);
        
        utilmm::stringlist ref;
        ref.push_back("a.1.b");
        ref.push_back("a.2.b");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.[1,2].b",true);
        
        utilmm::stringlist ref;
        ref.push_back("a.*.b");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.[1,3-6:2].b.[10,11-13]",false);
 
        utilmm::stringlist ref;
        ref.push_back("a.1.b.10");
        ref.push_back("a.1.b.11");
        ref.push_back("a.1.b.12");
        ref.push_back("a.1.b.13");
        ref.push_back("a.3.b.10");
        ref.push_back("a.3.b.11");
        ref.push_back("a.3.b.12");
        ref.push_back("a.3.b.13");
        ref.push_back("a.5.b.10");
        ref.push_back("a.5.b.11");
        ref.push_back("a.5.b.12");
        ref.push_back("a.5.b.13");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.[1,3-6:2].b.[10,11-13]",true);
 
        utilmm::stringlist ref;
        ref.push_back("a.*.b.*");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.*.[1,2].b.*",false);
        
        utilmm::stringlist ref;
        ref.push_back("a.*.1.b.*");
        ref.push_back("a.*.2.b.*");

        BOOST_CHECK(res == ref);
    }
    
    BOOST_TEST_CHECKPOINT( "SliceStore" );
     
    { 
        SliceStore s(""); 

        BOOST_CHECK( s.getPlaces().empty() ); 
    }
    
    { 
        SliceStore s("a"); 
        
        std::vector<std::string> ref;
        ref.push_back("a.");

        BOOST_CHECK( ref == s.getPlaces() ); 
    }
    
    { 
        SliceStore s("a b"); 
        
        std::vector<std::string> ref;
        ref.push_back("a.");
        ref.push_back("b.");

        BOOST_CHECK( ref == s.getPlaces() ); 
    }
    
    { 
        SliceStore s("a.[1,4-5]"); 
        
        std::vector<std::string> ref;
        ref.push_back("a.1.");
        ref.push_back("a.4.");
        ref.push_back("a.5.");

        BOOST_CHECK( ref == s.getPlaces() ); 
    }
    
    { 
        SliceStore s("a.[1,4-5].[1,8] b.[1,3] c"); 
        
        std::vector<std::string> ref;
        ref.push_back("a.1.1.");
        ref.push_back("a.1.8.");
        ref.push_back("a.4.1.");
        ref.push_back("a.4.8.");
        ref.push_back("a.5.1.");
        ref.push_back("a.5.8.");
        ref.push_back("b.1.");
        ref.push_back("b.3.");
        ref.push_back("c.");

        BOOST_CHECK( ref == s.getPlaces() ); 
    }
    
    { 
        SliceStore s("a.[1,4-5].[1,8] b.[1,3] c", true); 
        
        std::vector<std::string> ref;
        ref.push_back("a.*.*.");
        ref.push_back("b.*.");
        ref.push_back("c.");

        BOOST_CHECK( ref == s.getPlaces() ); 
    }
    
    { 
        SliceStore s("[1,12,13-17:2]"); 
        
        std::vector<std::string> ref;
        ref.push_back("1.");
        ref.push_back("12.");
        ref.push_back("13.");
        ref.push_back("15.");
        ref.push_back("17.");

        BOOST_CHECK( ref == s.getPlaces() ); 
    }

}

BOOST_AUTO_TEST_CASE ( test_slice_inslice ) {

    BOOST_TEST_CHECKPOINT("general places");
    
    {
        StringVector res = SliceMatcher::createGeneralPlaces("");
        
        StringVector ref;
        ref.push_back("");

        BOOST_CHECK( res == ref );
    }
    
    {
        StringVector res = SliceMatcher::createGeneralPlaces("a");
        
        StringVector ref;
        ref.push_back("a");

        BOOST_CHECK( res == ref );
    }
    
    {
        StringVector res = SliceMatcher::createGeneralPlaces("1");
        
        StringVector ref;
        ref.push_back("1");
        ref.push_back("*");

        BOOST_CHECK( res == ref );
    }
    
    {
        StringVector res = SliceMatcher::createGeneralPlaces("a.3");
        
        StringVector ref;
        ref.push_back("a.3");
        ref.push_back("a.*");

        BOOST_CHECK( res == ref );
    }
    
    {
        StringVector res = SliceMatcher::createGeneralPlaces("a.12.b");
        
        StringVector ref;
        ref.push_back("a.12.b");
        ref.push_back("a.*.b");
        
        BOOST_CHECK( res == ref );
    }
    
    {
        StringVector res = SliceMatcher::createGeneralPlaces("a.12.b.3");
        
        StringVector ref;
        ref.push_back("a.12.b.3");
        ref.push_back("a.12.b.*");
        ref.push_back("a.*.b.3");
        ref.push_back("a.*.b.*");

        BOOST_CHECK( res == ref );
    }
    
    {
        StringVector res = SliceMatcher::createGeneralPlaces("a.12.b.3.");
        
        StringVector ref;
        ref.push_back("a.12.b.3.");
        ref.push_back("a.12.b.*.");
        ref.push_back("a.*.b.3.");
        ref.push_back("a.*.b.*.");

        BOOST_CHECK( res == ref );
    }

    BOOST_TEST_CHECKPOINT("fits a slice");
    
    { 
        SliceMatcher s(""); 
        
        BOOST_CHECK ( s.fitsASlice("a") );
        BOOST_CHECK ( s.fitsASlice("c.1") );
        BOOST_CHECK ( s.fitsASlice("b.a") );
    }
    
    { 
        SliceMatcher s("a"); 
        
        BOOST_CHECK ( s.fitsASlice("a") );
        BOOST_CHECK ( s.fitsASlice("a.1") );
        BOOST_CHECK ( !s.fitsASlice("b") );
    }
    
    { 
        SliceMatcher s("[1,12,13-17:2]"); 
        
        BOOST_CHECK ( !s.fitsASlice("0") );
        BOOST_CHECK ( s.fitsASlice("1") );
        BOOST_CHECK ( !s.fitsASlice("2") );
        BOOST_CHECK ( !s.fitsASlice("3") );
        BOOST_CHECK ( !s.fitsASlice("4") );
        BOOST_CHECK ( !s.fitsASlice("5") );
        BOOST_CHECK ( !s.fitsASlice("6") );
        BOOST_CHECK ( !s.fitsASlice("7") );
        BOOST_CHECK ( !s.fitsASlice("8") );
        BOOST_CHECK ( !s.fitsASlice("9") );
        BOOST_CHECK ( !s.fitsASlice("10") );
        BOOST_CHECK ( !s.fitsASlice("11") );
        BOOST_CHECK ( s.fitsASlice("12") );
        BOOST_CHECK ( s.fitsASlice("13") );
        BOOST_CHECK ( !s.fitsASlice("14") );
        BOOST_CHECK ( s.fitsASlice("15") );
        BOOST_CHECK ( !s.fitsASlice("16") );
        BOOST_CHECK ( s.fitsASlice("17") );
        BOOST_CHECK ( !s.fitsASlice("18") );
        BOOST_CHECK ( !s.fitsASlice("19") );
    }
    
    { 
        SliceMatcher s("a.[1,2]"); 
        
        BOOST_CHECK ( !s.fitsASlice("a") );
        BOOST_CHECK ( s.fitsASlice("a.1") );
        BOOST_CHECK ( s.fitsASlice("a.2") );
        BOOST_CHECK ( !s.fitsASlice("a.3") );
        BOOST_CHECK ( !s.fitsASlice("b") );
        BOOST_CHECK ( !s.fitsASlice("b.1") );
    }
    
    { 
        SliceMatcher s("a.*.c"); 
        
        BOOST_CHECK ( !s.fitsASlice("a") );
        BOOST_CHECK ( s.fitsASlice("a.1.c") );
        BOOST_CHECK ( s.fitsASlice("a.2.c") );
        BOOST_CHECK ( !s.fitsASlice("a.3") );
        BOOST_CHECK ( !s.fitsASlice("a.b") );
        BOOST_CHECK ( !s.fitsASlice("a.3.a") );
    }

    { 
        SliceMatcher s("a.[1,4-5].[1,8] b.[1,3] c"); 
        
        std::vector<std::string> ref;
        ref.push_back("a.1.1.");
        ref.push_back("a.1.8.");
        ref.push_back("a.4.1.");
        ref.push_back("a.4.8.");
        ref.push_back("a.5.1.");
        ref.push_back("a.5.8.");
        ref.push_back("b.1.");
        ref.push_back("b.3.");
        ref.push_back("c.");

        BOOST_CHECK( ref == s.getSlices().getPlaces() );

        for ( std::vector<std::string>::iterator it = ref.begin(); it< ref.end(); it++)
            BOOST_CHECK( s.fitsASlice(*it) );

        BOOST_CHECK ( !s.fitsASlice("a") );
        BOOST_CHECK ( !s.fitsASlice("b") );
        BOOST_CHECK ( s.fitsASlice("c") );

        BOOST_CHECK ( s.fitsASlice("c.1") );
        BOOST_CHECK ( s.fitsASlice("c.a") );
    }
    
    { 
        SliceMatcher s("a.[1,4-5].[1,8] b.[1,3] c", true); 
        
        std::vector<std::string> ref;
        ref.push_back("a.*.*.");
        ref.push_back("b.*.");
        ref.push_back("c.");

        BOOST_CHECK( ref == s.getSlices().getPlaces() );

        for ( std::vector<std::string>::iterator it = ref.begin(); it< ref.end(); it++)
            BOOST_CHECK( s.fitsASlice(*it) );

        BOOST_CHECK ( !s.fitsASlice("a") );
        BOOST_CHECK ( !s.fitsASlice("a.*") );
        BOOST_CHECK ( !s.fitsASlice("a.1") );
        BOOST_CHECK ( s.fitsASlice("a.*.*") );
        BOOST_CHECK ( s.fitsASlice("a.2.10") );
        
        BOOST_CHECK ( !s.fitsASlice("b") );
        BOOST_CHECK ( s.fitsASlice("b.*") );
        BOOST_CHECK ( s.fitsASlice("b.3") );
        BOOST_CHECK ( s.fitsASlice("b.1.c.4") );

        BOOST_CHECK ( s.fitsASlice("c") );
        BOOST_CHECK ( s.fitsASlice("c.1") );
        BOOST_CHECK ( s.fitsASlice("c.a") );
    }

    { 
        SliceMatcher s("*"); 
        
        std::vector<std::string> ref;
        ref.push_back("*.");

        BOOST_CHECK( ref == s.getSlices().getPlaces() );

        BOOST_CHECK ( !s.fitsASlice("a") );
        BOOST_CHECK ( s.fitsASlice("12") );
        BOOST_CHECK ( s.fitsASlice("3.a") );
        BOOST_CHECK ( !s.fitsASlice("b.2") );
    }

    { 
        SliceMatcher s("a.*.[1,8] b.[1,3] c"); 
        
        std::vector<std::string> ref;
        ref.push_back("a.*.1.");
        ref.push_back("a.*.8.");
        ref.push_back("b.1.");
        ref.push_back("b.3.");
        ref.push_back("c.");

        BOOST_CHECK( ref == s.getSlices().getPlaces() );

        for ( std::vector<std::string>::iterator it = ref.begin(); it< ref.end(); it++)
            BOOST_CHECK( s.fitsASlice(*it) );

        BOOST_CHECK ( !s.fitsASlice("a") );
        BOOST_CHECK ( !s.fitsASlice("b") );
        BOOST_CHECK ( s.fitsASlice("c") );

        BOOST_CHECK ( s.fitsASlice("a.12.1") );
        BOOST_CHECK ( !s.fitsASlice("a.1.b") );
        BOOST_CHECK ( !s.fitsASlice("a.1.9") );
    }

    { 
        SliceMatcher s("a.*.b.[1-7:3].*.c"); 
        
        std::vector<std::string> ref;
        ref.push_back("a.*.b.1.*.c.");
        ref.push_back("a.*.b.4.*.c.");
        ref.push_back("a.*.b.7.*.c.");

        BOOST_CHECK( ref == s.getSlices().getPlaces() );

        BOOST_CHECK ( !s.fitsASlice("a") );

        BOOST_CHECK ( s.fitsASlice("a.12.b.1.3.c") );
        BOOST_CHECK ( s.fitsASlice("a.12.b.1.10.c") );
        BOOST_CHECK ( s.fitsASlice("a.30.b.1.10.c") );
        BOOST_CHECK ( !s.fitsASlice("a.12.b.1.10.d") );
        BOOST_CHECK ( !s.fitsASlice("a.12.b.2.10.d") );
        BOOST_CHECK ( !s.fitsASlice("d.12.b.2.10.d") );
    }
}

BOOST_AUTO_TEST_CASE ( test_slice_tree ) 
{
   
    SliceNode n1("place");

    BOOST_CHECK( !n1.isCountable() );
    BOOST_CHECK( !n1.isIn(12) );

    n1 = SliceNode("*");
    
    BOOST_CHECK( n1.isCountable() );
    BOOST_CHECK( n1.isIn(12) );
    
    n1 = SliceNode("[1,5,7-8,10-17:2]");
    
    BOOST_CHECK( n1.isCountable() );
    BOOST_CHECK( !n1.isIn(0) );
    BOOST_CHECK( n1.isIn(1) );
    BOOST_CHECK( n1.isIn(5) );
    BOOST_CHECK( !n1.isIn(6) );
    BOOST_CHECK( n1.isIn(7) );
    BOOST_CHECK( n1.isIn(8) );
    BOOST_CHECK( n1.isIn(10) );
    BOOST_CHECK( n1.isIn(12) );
    BOOST_CHECK( !n1.isIn(13) );
    BOOST_CHECK( n1.isIn(14) );
    BOOST_CHECK( n1.isIn(16) );
    BOOST_CHECK( !n1.isIn(17) );

    BOOST_TEST_CHECKPOINT ( "SliceTree" );
    
    SliceTree t1("start.idx start.[1,4-6].a start.[1,4-6].b start.*.c zwei");

    //std::cout << t1.toString() << std::endl;

    BOOST_CHECK( t1.fitsASlice("start.idx") );
    BOOST_CHECK( t1.fitsASlice("start.1.a") );
    BOOST_CHECK( t1.fitsASlice("start.5.a") );
    BOOST_CHECK( t1.fitsASlice("start.1.b") );
    BOOST_CHECK( t1.fitsASlice("start.4.b") );
    BOOST_CHECK( t1.fitsASlice("start.8.c") );
    BOOST_CHECK( t1.fitsASlice("zwei") );
    BOOST_CHECK( t1.fitsASlice("start.idx.2.a") );
    BOOST_CHECK( t1.fitsASlice("start.idx.val") );
    BOOST_CHECK( t1.fitsASlice("zwei.idx") );
    BOOST_CHECK( !t1.fitsASlice("drei") );
    BOOST_CHECK( !t1.fitsASlice("start.value") );
    BOOST_CHECK( !t1.fitsASlice("start.2.a") );
    BOOST_CHECK( !t1.fitsASlice("start.1.d") );
    BOOST_CHECK( !t1.fitsASlice("start.1") );

    t1 = SliceTree("a.1 a.3.b.2");

    //std::cout << t1.toString() << std::endl;
    
    BOOST_CHECK( t1.fitsASlice("a.1") );
    BOOST_CHECK( t1.fitsASlice("a.1.b.2") );
    BOOST_CHECK( !t1.fitsASlice("a.2.b.2") );
    BOOST_CHECK( !t1.fitsASlice("a.3.c") );
    BOOST_CHECK( !t1.fitsASlice("a.3") );
    BOOST_CHECK( t1.fitsASlice("a.3.b.2") );
    
    t1 = SliceTree("b.1 a.3.b.2");

    // containers
    BOOST_CHECK( t1.fitsASlice("b.*"));
    BOOST_CHECK( t1.fitsASlice("a.*.b.2"));
    BOOST_CHECK( t1.fitsASlice("a.3.b.*"));
    BOOST_CHECK( t1.fitsASlice("a.*.b.*"));
    BOOST_CHECK( !t1.fitsASlice("a.2.b.*"));
    BOOST_CHECK( !t1.fitsASlice("a.*.c"));
    BOOST_CHECK( !t1.fitsASlice("a.*.b.1"));
    
    t1 = SliceTree("! a.1 a.3.b.2");

    //std::cout << t1.toString() << std::endl;
    
    BOOST_CHECK( !t1.fitsASlice("a.1") );
    BOOST_CHECK( !t1.fitsASlice("a.1.b.2") );
    BOOST_CHECK( t1.fitsASlice("a.2.b.2") );
    BOOST_CHECK( t1.fitsASlice("a.3.c") );
    BOOST_CHECK( t1.fitsASlice("a.3") );
    BOOST_CHECK( !t1.fitsASlice("a.3.b.2") );
}
