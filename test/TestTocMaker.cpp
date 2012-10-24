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


BOOST_AUTO_TEST_CASE ( test_scalar ) {

    Typelib::Registry registry;

    import_types(registry);

    BOOST_REQUIRE(registry.has("/double"));
    
    VectorToc toc = VectorTocMaker().apply(*registry.get("/double"));

    BOOST_CHECK ( toc.mType == "/double");
    BOOST_CHECK ( toc.mSlice == "");
    BOOST_REQUIRE ( toc.size() == 1 );
    BOOST_CHECK ( toc.back().placeDescription == "");
    BOOST_CHECK ( toc.back().castFun == getCastFunction(*registry.get("/double")) );
    BOOST_CHECK ( toc.back().content == 0 );
    BOOST_CHECK ( PlainTocVisitor().apply(toc).size() == 1 );
    BOOST_CHECK ( PlainTocVisitor().apply(toc)[0] == "" );
}
