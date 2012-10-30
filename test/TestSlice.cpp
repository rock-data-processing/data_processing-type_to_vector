// \file  TestSlice.cpp

#include <boost/test/auto_unit_test.hpp>

#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#include <utilmm/stringtools.hh>

#include "TestSuite.hpp"

#include "SliceMatcher.hpp"

using namespace general_processing;

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
    

    BOOST_TEST_CHECKPOINT ("resolveIndices");
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("position");

        BOOST_CHECK(res.empty());
    }

    {
        std::vector<std::string> res = SliceStore::resolveIndices("[1,4,6]");

        std::vector<std::string> ref;
        ref.push_back("1");
        ref.push_back("4");
        ref.push_back("6");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[3-6]");

        std::vector<std::string> ref;
        ref.push_back("3");
        ref.push_back("4");
        ref.push_back("5");
        ref.push_back("6");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[3-10:3]");

        std::vector<std::string> ref;
        ref.push_back("3");
        ref.push_back("6");
        ref.push_back("9");

        BOOST_CHECK(res == ref);
    }
    
    {
        std::vector<std::string> res = SliceStore::resolveIndices("[1,3-10:3,11-13]");

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

    BOOST_TEST_CHECKPOINT( "makeDirectIndexSlice" );

    { 
        IntSet stars;
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.2.b", stars);
        
        std::vector<std::string> ref;
        ref.push_back("a.2.b");

        BOOST_CHECK(res == ref); 
        BOOST_CHECK(stars.empty());
    }

    { 
        IntSet stars;
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.[1,3-6:2].b",stars);
        
        std::vector<std::string> ref;
        ref.push_back("a.1.b");
        ref.push_back("a.3.b");
        ref.push_back("a.5.b");

        BOOST_CHECK(res == ref); 
        BOOST_CHECK(stars.empty());
    }
    
    { 
        IntSet stars;
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.[1,3-6:2].b.[10,11-13]",stars);
        
        std::vector<std::string> ref;
        ref.push_back("a.1.b.[10,11-13]");
        ref.push_back("a.3.b.[10,11-13]");
        ref.push_back("a.5.b.[10,11-13]");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        IntSet stars;
        std::vector<std::string> res = 
            SliceStore::makeDirectIndexSlice("a.*.b.*.[2,3].c.*.1",stars);
        
        std::vector<std::string> ref;
        ref.push_back("a.*.b.*.2.c.*.1");
        ref.push_back("a.*.b.*.3.c.*.1");

        BOOST_CHECK(res == ref); 
        BOOST_CHECK(stars.size() == 2);
        BOOST_CHECK(stars.count(1) == 1);
        BOOST_CHECK(stars.count(3) == 1);
    }

    BOOST_TEST_CHECKPOINT( "replaceIndicesSlices" );
     
    { 
        IntSet stars;
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.[1,2].b",stars);
        
        utilmm::stringlist ref;
        ref.push_back("a.1.b");
        ref.push_back("a.2.b");

        BOOST_CHECK(res == ref); 
    }
    
    { 
        IntSet stars;
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.[1,3-6:2].b.[10,11-13]",stars);
 
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
        IntSet stars;
        utilmm::stringlist res = 
            SliceStore::replaceIndicesSlices("a.*.[1,2].b.*",stars);
        
        utilmm::stringlist ref;
        ref.push_back("a.*.1.b.*");
        ref.push_back("a.*.2.b.*");

        BOOST_CHECK(res == ref);
        BOOST_CHECK(stars.size() == 2);
        BOOST_CHECK(stars.count(1) == 1);
        BOOST_CHECK(stars.count(4) == 1);
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
