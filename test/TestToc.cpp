// \file  TestToc.cpp

#include <boost/test/auto_unit_test.hpp>

#include <Utilities.hpp>
#include <VectorToc.hpp>

using namespace general_processing;

VectorValueInfo makeInfo(const std::string& descr, unsigned int position,
        VectorToc* content=0) {
    VectorValueInfo info;
    info.placeDescription = descr;
    info.position = position;
    info.content = VectorTocPointer(content);
    return info;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& sv) {
    std::vector<std::string>::const_iterator it = sv.begin();
    for ( ; it<sv.end(); it++ ) os << *it << std::endl;
    return os;
}

BOOST_AUTO_TEST_CASE ( test_toc_empty ) {
    VectorToc toc;

    BOOST_CHECK(toc.empty());
    BOOST_CHECK(toc.mType=="");
    BOOST_CHECK(toc.mSlice=="");
    BOOST_CHECK(PlainTocVisitor().apply(toc).empty()); 
}

BOOST_AUTO_TEST_CASE ( test_toc_single_level ) {

    std::vector<std::string> plain_toc;
    VectorToc toc;

    toc.mType = "/Test";
    toc.push_back(makeInfo("a",0));
    toc.push_back(makeInfo("b",4));
    toc.push_back(makeInfo("c",8));

    plain_toc.push_back("a");
    plain_toc.push_back("b");
    plain_toc.push_back("c");

    BOOST_CHECK( plain_toc == PlainTocVisitor().apply(toc));

    {
        VectorToc toc2;

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b",4));
        toc2.push_back(makeInfo("c",8));

        BOOST_CHECK( toc == toc2 );

        toc2.mType = "/T2";

        BOOST_CHECK ( toc != toc2 );

        toc2.mType = "/Test";
        toc2.mSlice = "a";
        
        BOOST_CHECK ( toc != toc2 );
    }
    
    {
        VectorToc toc2;

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b",4));

        BOOST_CHECK ( toc != toc2 );
        
        toc2.push_back(makeInfo("d",8));
        
        BOOST_CHECK ( toc != toc2 );
    }
    
    {
        VectorToc toc2;

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b",4));
        toc2.push_back(makeInfo("c",6));

        BOOST_CHECK ( toc != toc2 );
    }
}

BOOST_AUTO_TEST_CASE ( test_toc_container ) {
    
    std::vector<std::string> plain_toc;
    VectorToc toc;

    VectorToc* subtoc = new VectorToc;
    subtoc->mType = "/SubTest";
    subtoc->push_back(makeInfo("",0));

    toc.mType = "/Test";
    toc.push_back(makeInfo("*",0,subtoc));
    
    plain_toc.push_back("*");

    BOOST_CHECK( plain_toc == PlainTocVisitor().apply(toc));

}

BOOST_AUTO_TEST_CASE ( test_toc_level_two ) {
    
    std::vector<std::string> plain_toc;
    VectorToc toc;

    VectorToc* subtoc = new VectorToc;
    subtoc->mType = "/SubTest";
    subtoc->push_back(makeInfo("a",0));
    subtoc->push_back(makeInfo("b",4));
    subtoc->push_back(makeInfo("c",8));

    toc.mType = "/Test";
    toc.push_back(makeInfo("a",0));
    toc.push_back(makeInfo("b.*",4,subtoc));
    
    plain_toc.push_back("a");
    plain_toc.push_back("b.*.a");
    plain_toc.push_back("b.*.b");
    plain_toc.push_back("b.*.c");
    
    BOOST_CHECK( plain_toc == PlainTocVisitor().apply(toc));
    
    {
        VectorToc toc2;

        subtoc = new VectorToc;
        subtoc->mType = "/SubTest";
        subtoc->push_back(makeInfo("a",0));
        subtoc->push_back(makeInfo("b",4));
        subtoc->push_back(makeInfo("c",8));

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b.*",4,subtoc));

        BOOST_CHECK ( toc2 == toc );
    }
    
    {
        VectorToc toc2;

        subtoc = new VectorToc;
        subtoc->mType = "/SubT";
        subtoc->push_back(makeInfo("a",0));
        subtoc->push_back(makeInfo("b",4));
        subtoc->push_back(makeInfo("c",8));

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b.*",4,subtoc));

        BOOST_CHECK ( toc != toc2 );
    }
    
    {
        VectorToc toc2;

        subtoc = new VectorToc;
        subtoc->mType = "/SubTest";
        subtoc->push_back(makeInfo("a",0));
        subtoc->push_back(makeInfo("b",4));
        subtoc->push_back(makeInfo("d",8));

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b.*",4,subtoc));

        BOOST_CHECK ( toc != toc2 );
    }
    
    {
        VectorToc toc2;

        subtoc = new VectorToc;
        subtoc->mType = "/SubTest";
        subtoc->push_back(makeInfo("a",0));
        subtoc->push_back(makeInfo("b",4));

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b.*",4,subtoc));

        BOOST_CHECK ( toc != toc2 );
    }
    
    {
        VectorToc toc2;

        subtoc = new VectorToc;
        subtoc->mType = "/SubTest";
        subtoc->push_back(makeInfo("a",0));
        subtoc->push_back(makeInfo("b",4));
        subtoc->push_back(makeInfo("c",7));

        toc2.mType = "/Test";
        toc2.push_back(makeInfo("a",0));
        toc2.push_back(makeInfo("b.*",4,subtoc));

        BOOST_CHECK ( toc != toc2 );
    }
}

