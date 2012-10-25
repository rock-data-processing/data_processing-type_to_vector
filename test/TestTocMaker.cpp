// \file  TestTocMaker.cpp

#include <boost/test/auto_unit_test.hpp>

#include <typelib/pluginmanager.hh>
#include <typelib/importer.hh>
#include <typelib/typemodel.hh>
#include <typelib/registry.hh>

#include "TestSuite.hpp"

#include "VectorTocMaker.hpp"
#include "Utilities.hpp"
#include "VectorToc.hpp"
#include "NumericConverter.hpp"

using namespace general_processing;
using namespace Typelib;

void import_types(Typelib::Registry& registry) {

    static const char* test_file = TEST_DATA_PATH("TestTypes.tlb");

    utilmm::config_set config;
    PluginManager::self manager;
    manager->load("tlb", test_file, config, registry);
}


BOOST_AUTO_TEST_CASE ( test_tocmaker_scalar ) {

    Typelib::Registry registry;

    import_types(registry);

    BOOST_REQUIRE(registry.has("/double"));
    
    VectorToc toc = VectorTocMaker().apply(*registry.get("/double"));

    BOOST_CHECK ( toc.mType == "/double");
    BOOST_CHECK ( toc.mSlice == "");
    BOOST_REQUIRE ( toc.size() == 1 );
    BOOST_CHECK ( toc.back().placeDescription == "");
    BOOST_REQUIRE ( toc.back().castFun != 0 );

    double val = 1.4;
    BOOST_CHECK ( toc.back().castFun(&val) == val );
    BOOST_CHECK ( toc.back().content == 0 );
    BOOST_CHECK ( PlainTocVisitor().apply(toc).size() == 1 );
    BOOST_CHECK ( PlainTocVisitor().apply(toc)[0] == "" );
}

BOOST_AUTO_TEST_CASE ( test_tocmaker_array ) {

    Typelib::Registry registry;

    import_types(registry);

    registry.build("/double[3]");

    BOOST_REQUIRE ( registry.has("/double[3]", false) );

    VectorToc toc = VectorTocMaker().apply(*registry.get("/double[3]"));

    BOOST_CHECK ( toc.mType == "/double[3]");
    BOOST_REQUIRE ( toc.size() == 3 );
    
   for ( int i = 0; i<3 ; i++) {
        BOOST_CHECK ( toc.at(i).placeDescription[0] == '0' + i);
        BOOST_REQUIRE ( toc.at(i).castFun != 0 );

        double val = 1.4;
        BOOST_CHECK ( toc.at(i).castFun(&val) == val );
        BOOST_CHECK ( toc.at(i).content == 0 );
   }

   std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
   utilmm::stringlist sl(ptoc.begin(), ptoc.end());
   BOOST_CHECK ( utilmm::join(sl) == "0 1 2" );

}

BOOST_AUTO_TEST_CASE ( test_tocmaker_struct ) {
    
    Typelib::Registry registry;

    import_types(registry);

    BOOST_REQUIRE ( registry.has("/A",false) );
    BOOST_REQUIRE ( registry.get("/A") );

    VectorToc toc = VectorTocMaker().apply(*registry.get("/A"));

    BOOST_CHECK ( toc.mType == "/A" );
    BOOST_REQUIRE ( toc.size() == 4 );

    BOOST_CHECK ( toc.at(0).placeDescription == "a");
    BOOST_REQUIRE ( toc.at(0).castFun != 0 );
    long long vall = 1<<32;
    BOOST_CHECK ( toc.at(0).castFun(&vall) == double(vall) );
    BOOST_CHECK ( toc.at(0).content == 0 );
    
    BOOST_CHECK ( toc.at(1).placeDescription == "b");
    BOOST_REQUIRE ( toc.at(1).castFun != 0 );
    int vali = 1<<20;
    BOOST_CHECK ( toc.at(1).castFun(&vali) == double(vali) );
    BOOST_CHECK ( toc.at(1).content == 0 );
    
    BOOST_CHECK ( toc.at(2).placeDescription == "c");
    BOOST_REQUIRE ( toc.at(2).castFun != 0 );
    char valc = 1<<4;
    BOOST_CHECK ( toc.at(2).castFun(&valc) == double(valc) );
    BOOST_CHECK ( toc.at(2).content == 0 );
    
    BOOST_CHECK ( toc.at(3).placeDescription == "d");
    BOOST_REQUIRE ( toc.at(3).castFun != 0 );
    short vals = 1<<12;
    BOOST_CHECK ( toc.at(3).castFun(&vals) == double(vals) );
    BOOST_CHECK ( toc.at(3).content == 0 );
   
    std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
    utilmm::stringlist sl(ptoc.begin(), ptoc.end());
    BOOST_CHECK ( utilmm::join(sl) == "a b c d" );
}

BOOST_AUTO_TEST_CASE ( test_tocmaker_container ) {

    Typelib::Registry registry;

    import_types(registry);

    std::string type_str = "/std/vector</int>";

    BOOST_REQUIRE ( registry.has(type_str, false) );

    VectorToc toc = VectorTocMaker().apply(*registry.get(type_str));

    BOOST_CHECK ( toc.mType == "/std/vector</int32_t>" );
    BOOST_REQUIRE ( toc.size() == 1 );
    
    BOOST_CHECK ( toc.back().placeDescription == "*");
    BOOST_CHECK ( toc.back().castFun == 0 );
    BOOST_REQUIRE ( toc.back().content > 0 );

    VectorToc* subtoc = toc.back().content;
    
    BOOST_CHECK ( subtoc->mType == "/int32_t" );
    BOOST_REQUIRE ( subtoc->size() == 1 );
    
    BOOST_CHECK ( subtoc->back().content ==  0 );
    BOOST_CHECK ( subtoc->back().placeDescription == "");
    BOOST_REQUIRE ( subtoc->back().castFun > 0 );

    int val = 23;
    BOOST_CHECK ( subtoc->back().castFun(&val) == double(val) );
    
    std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
    BOOST_CHECK ( ptoc.size() == 1 );

    utilmm::stringlist sl(ptoc.begin(), ptoc.end());
    BOOST_CHECK ( utilmm::join(sl) == "*" );
}

BOOST_AUTO_TEST_CASE ( test_tocmaker_string ) {

    Typelib::Registry registry;

    import_types(registry);

    std::string type_str = "/std/string";

    BOOST_REQUIRE ( registry.has(type_str) );
    registry.build(type_str);
    BOOST_REQUIRE ( registry.has(type_str, false) );

    VectorToc toc = VectorTocMaker().apply(*registry.get(type_str));

    BOOST_CHECK ( toc.mType == type_str );
    BOOST_REQUIRE ( toc.size() == 1 );
    
    BOOST_CHECK ( toc.back().placeDescription == "*");
    BOOST_REQUIRE ( toc.back().castFun == 0 );
    BOOST_REQUIRE ( toc.back().content > 0 );

    VectorToc* subtoc = toc.back().content;
    
    BOOST_CHECK ( subtoc->mType == "/int8_t" );
    BOOST_REQUIRE ( subtoc->size() == 1 );
    
    BOOST_CHECK ( subtoc->back().placeDescription == "");
    BOOST_CHECK ( subtoc->back().content ==  0 );
    BOOST_REQUIRE ( subtoc->back().castFun > 0 );
    
    char c = 'a';
    BOOST_CHECK ( subtoc->back().castFun(&c) == double(c) );
    
    std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
    utilmm::stringlist sl(ptoc.begin(), ptoc.end());
    BOOST_CHECK ( utilmm::join(sl) == "*" );
}

namespace plain_tocs {

std::string DoubleVector = "a dbl_vector.*";
std::string VectorArray = "dbl_vector_array.0.* dbl_vector_array.1.* "
    "dbl_vector_array.2.*";
std::string StructArray = "A_vector.*.a A_vector.*.b A_vector.*.c A_vector.*.d";
std::string ContainerContainer = "dbl_vv.*.a dbl_vv.*.dbl_vector.*";

} // plain_toc

BOOST_AUTO_TEST_CASE ( test_tocmaker_advanced ) {
    
    Typelib::Registry registry;

    import_types(registry);

    BOOST_REQUIRE ( registry.has("/DoubleVector",false) );
    {
        VectorToc toc = VectorTocMaker().apply(*registry.get("/DoubleVector"));
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == plain_tocs::DoubleVector );
    }
    
    BOOST_REQUIRE ( registry.has("/VectorArray",false) );
    {
        VectorToc toc = VectorTocMaker().apply(*registry.get("/VectorArray"));
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == plain_tocs::VectorArray );
    }
    
    BOOST_REQUIRE ( registry.has("/StructArray",false) );
    {
        VectorToc toc = VectorTocMaker().apply(*registry.get("/StructArray"));
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == plain_tocs::StructArray );
    }
    
    BOOST_REQUIRE ( registry.has("/ContainerContainer",false) );
    {
        VectorToc toc = VectorTocMaker().apply(*registry.get("/ContainerContainer"));
        std::vector<std::string> ptoc = PlainTocVisitor().apply(toc);
        utilmm::stringlist sl(ptoc.begin(), ptoc.end());
        BOOST_CHECK ( utilmm::join(sl) == plain_tocs::ContainerContainer );
    }

    
}

